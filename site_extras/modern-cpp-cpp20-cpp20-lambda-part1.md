> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp20-lambda-part1/) 的文章，由站点同步脚本导出。

# C++20 Lambda 表达式（上）

## C++20之lambda模版

C++14 支持泛型 lambda 表达式，这使得 lambda 可以接受任意类型的参数，例如：

```cpp
auto func = [](auto a, auto b) {
    return a + b;
};
```

会被展开为：

```cpp
struct Func {
    template <typename T1, typename T2>
    auto operator()(T1 a, T2 b) const {
        return a + b;
    }
};
```

但是却无法做到下面几种展开式：

第一种：相同类型

````cpp
template <class T>
auto operator()(T t1, T t2) const;
````

第二种：只有std::array

```cpp
template <class T, std::size_t N>
auto operator()(std::array<T, N> const &) const; 
```

第三种：没有推导

```
template <class T>
auto operator()() const;
```

C++14 lambdas 很好，但是 C++20 允许我们轻松实现这些情况。C++20允许lambda表达式在定义中使用模版类型参数，以下是基本的语法：

```cpp
[]<typename T>(){}
```

下面通过几个示例说明它的好处。

## 1.类型限定

像下面这种例子，C++14我们可以写出下面的代码，但是我们在使用的时候传递进来的不一定是一个vector，那么与预期的行为就会不一致，从而产生错误。

```cpp
auto print = [](auto vec) {
  for (auto x : vec) {
    std::cout << x << std::endl;
  }
};
```

C++20之后，我们可以限定其类型：

```cpp
auto print2 = []<typename T>(std::vector<T>& vec){
    //使用 std::vector<T> 执行操作
};
```

那么问题来了，出一个面试题目：你知道C++14中为了限定vector，怎么才能保证安全呢？

## 2.完美转发

在C++20中你可以非常轻松的使用完美转发，例如：

```cpp
auto lambdaFunc = []<typename T>(T&& number) {
  processFunction(std::forward<T>(number));
};
```

那么问题来了，相比于C++14来说，有什么优势？或者说C++14实现完美转发需要做什么？

> 本节完