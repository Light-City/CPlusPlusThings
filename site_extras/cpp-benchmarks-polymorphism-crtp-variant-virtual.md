> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-benchmarks/polymorphism-crtp-variant-virtual/) 的文章，由站点同步脚本导出。

# C++ 多态：虚函数、CRTP 与 variant

## C++多态性能测试：CRTP vs std::variant vs virtual

多态是面向对象编程的一个重要概念，它使得单一接口能够代表不同的类型。C++提供了几种实现多态性的方式，本文将会讨论三种场景的多态：

1. **虚函数**：在C++中实现多态性的传统方式是使用虚函数。这涉及使用基类和派生类来实现特定的实现。
2. **std::variant**：在C++17中引入的std::variant，它实现了一种无需继承的多态性。
3. **CRTP（Curiously Recurring Template Pattern）**：CRTP是一种比较特殊的技术，它通过模板的奇特递归模式实现多态性。

测试的组合场景如下：

- 单纯crtp
- crtp + std::variant
- virtual
- std::variant + std::visit
- std::variant + std::get_if
- std::variant + std::holds_alternative

使用的编译器：

- gcc 13.2
- clang17.0

完整测试代码已放置**星球**，这里贴一下关键代码

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape
class Rectangle : public Shape
  

  
// virtual
static void BM_VirtualFunction(benchmark::State& state) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.reserve(1000);

    for (int i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            shapes.emplace_back(std::make_unique<Circle>(5.0));
        } else {
            shapes.emplace_back(std::make_unique<Rectangle>(4.0, 6.0));
        }
    }

    for (auto _ : state) {
        double totalArea = 0.0;
        for (const auto& shape : shapes) {
            totalArea += shape->area();
        }
        benchmark::DoNotOptimize(totalArea);
    }
}
BENCHMARK(BM_VirtualFunction);

static void BM_VariantVisit(benchmark::State& state) {
    std::vector<std::variant<Circle, Rectangle>> shapes;
    shapes.reserve(1000);

    for (int i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            shapes.emplace_back(Circle(5.0));
        } else {
            shapes.emplace_back(Rectangle(4.0, 6.0));
        }
    }

    for (auto _ : state) {
        double totalArea = 0.0;
        for (const auto& s : shapes) {
            totalArea += std::visit([](const auto& shape) { return shape.area(); }, s);
        }
        benchmark::DoNotOptimize(totalArea);
    }
}
static void BM_CRTPVariant(benchmark::State& state) {
    std::vector<ShapeVariant> shapes;
    shapes.reserve(1000);

    for (int i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            shapes.emplace_back(crtp_light::Circle(5.0));
        } else {
            shapes.emplace_back(crtp_light::Rectangle(4.0, 6.0));
        }
    }

    for (auto _ : state) {
        double totalArea = 0.0;
        for (const auto& s : shapes) {
            totalArea += std::visit([](const auto& shape) { return shape.area(); }, s);
        }
        benchmark::DoNotOptimize(totalArea);
    }
}
BENCHMARK(BM_CRTPVariant);

// ...
```

测试结果1：gcc编译，可以看到virtual与std::variant性能差别不大，但是与crtp差别非常大。

![](../assets/images/2024/bench/virtual/img/gcc_bm.png)

测试结果2：clang编译，总体趋势类似gcc编译，只有crtp + std::variant性能明显回退，这个可能也是由于这里用了std::visit导致。

![](../assets/images/2024/bench/virtual/img/clang_bm.png)

在A Tour of C++书中提到：

> This is basically equivalent to a virtual function call, but potentially faster. As with all claims of performance, this ‘‘potentially faster’’ should be verified by measurements when performance is critical. For most uses, the difference in performance is insignificant.

与本次测试基本符合。