> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/cpp17-top-17-features/) 的文章，由站点同步脚本导出。

# C++17 十七项高级特性

### 全面盘点17个C++17的高级特性



C++17是目前比较常用的版本之一，今天花时间来梳理一下17个重要特性，所有的特性也不止这么点。

[TOC]

### 1. 并行算法

C++17引入了许多并行版本的标准库中的算法。这些算法可以并行执行，因此在多核系统上可能会带来显著的性能提升。

之前写过一篇全面介绍这个特性的文章，可以看这篇：

例子：

```cpp
#include <algorithm>
#include <vector>
#include <execution>
int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::sort(std::execution::par, v.begin(), v.end());
}
```

在此例子中，`std::sort`是并行执行的，以并行方式对向量`v`的元素进行排序。

### 2. If Initializers

C++17中的If初始化器是一项特性，它允许在if语句中直接初始化变量。这种初始化方式在一定程度上可以提高代码的可读性和简洁性。

在传统的C++中，我们通常会这样初始化变量：

```cpp
int x;
if (condition) {
    x = 42;
} else {
    x = 24;
}
```

而在C++17中，可以使用if初始化器来简化这个过程，使代码更加紧凑：

```cpp
if (bool condition = /* some condition */) {
    int x = 42;
} else {
    int x = 24;
}
```

在这个例子中，我们将变量`x`的初始化直接放在if语句中。变量`condition`在if语句中被定义和初始化，然后在if语句块中可用。这种方式更加直观和简洁，尤其是在简单的条件初始化时。

### 3. 类模板参数推导（CTAD）

CTAD 让编译器从函数参数中自动推导出模板参数。这使得在不必显式指定模板参数的情况下更容易地使用模板。

往期对这个特性的全面阐述文章：

例子：

```
template <typename T>
void foo(T t) {
    // ...
}

int main() {
    foo(42);  // 编译器推导出T的类型为int
}
```

在此例子中，当调用`foo(42)`时，编译器推导出`T`的类型是`int`.

### 4. template `<auto>`

模板关键词被引入为非类型模板参数的占位符。它允许在模板中表示任何类型的值。

例子：

```
template<auto value>
struct constant {
    static constexpr auto get_value() { return value; }
};

// 用法
static_assert(constant<42>::get_value() == 42);
```

### 5. std::optional 和 std::variant

`std::optional` 和 `std::variant` 是C++17中引入的两个新类型。`std::optional` 表示一个可能存在也可能不存在的值，`std::variant` 代表一个类型安全的联合，可以保存不同类型的值。

例子：

```
#include <optional>
#include <variant>

int main() {
    std::optional<int> opt = 42;
    std::variant<int, double> var = 3.14;
}
```

在这个例子中，`opt`是包含值42的可选整数，`var`是包含值3.14的变体。

### 6. 折叠表达式

在C++17中，折叠表达式提供了一种简洁的方式，用于对参数包执行二元操作。它们允许在不需要显式递归或迭代的情况下执行诸如求和、乘法或连接参数包中元素的操作。

例如：

```cpp
#include <iostream>

template<typename T>
T sum(T t) {
    return t;
}

// 使用折叠表达式的递归情况
template<typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}

int main() {
    int total = sum(1, 2, 3, 4, 5);
    std::cout << "总和: " << total << std::endl;
    
    return 0;
}

```

递归`sum`函数中的折叠表达式`(first + ... + args)`对参数包中的每个元素应用了加法操作。

### 7. 结构化绑定

结构化绑定允许你将对象分解成其构成元素，类似于你可能会用到的元组拆包。

往期文章：

例子：

```
#include <tuple>
#include <string>
int main() {
    std::tuple<int, std::string, double> t(42, "hello", 3.14);
    auto [i, s, d] = t;  // i = 42, s = "hello", d = 3.14
}
```

在此例子中，结构化绑定`[i, s, d]`将元组`t`分解成其构成元素。

### 8.模板模板参数

例如：在C++17中，语法 `template<template<class...>typename bob> struct foo {}` 声明了一个名为 `foo` 的模板，它接受一个名为 `bob` 的模板模板参数。模板模板参数 `bob` 本身接受任意数量的模板类型参数。

```cpp
template <template<class...> typename bob>
struct foo {
    template<typename T>
    void bar(const bob<T>& arg) {
        std::cout << "size: " << arg.size() << std::endl;
    }
};

int main() {
    foo<std::vector> f;
    std::vector<int> vec = {1, 2, 3, 4, 5};
    f.bar(vec);

    return 0;
}
```

在 `main` 函数中，我们使用 `std::vector` 实例化了 `foo`，将其作为 `bob` 的模板参数。这使我们能够创建一个通用的结构 `foo`，可以与任何接受任意数量类型参数的模板一起工作，例如 `std::vector`、`std::list` 或用户定义的模板。

### 9. 内联变量

C++17允许在类的定义内部定义变量为内联的，这可以帮助减小二进制大小，可能通过防止变量在多个转换单元中的重复副本来提高性能。

例子：

```cpp
class MyClass {
public:
    inline static int inlineVar = 42;
};

int main() {
    int localVar = MyClass::inlineVar;
}
```

在这里，`inlineVar`是`MyClass`的内联静态成员变量。

### 10. 属性改进

C++17提供新的属性，并改进了已有的属性，允许开发人员为编译器提供更多的代码行为信息。

例子：

```cpp
[[nodiscard]] int get_sum(int a, int b) {
    return a + b;
}

int main() {
    auto result = get_sum(1, 2);
    // 编译器可能会警告‘result’未使用
}
```

在此例子中，`[[nodiscard]]`是可以应用于函数的属性，表示其返回值不应该被调用者丢弃。

### 11. 嵌套命名空间

C++17通过折叠表达式增强了变参模板，使得在处理参数包时的代码更为简洁和表达明了。

例子：

```cpp
// 外部命名空间
namespace outer {
    // 内部命名空间
    namespace inner {
        void foo() {
            std::cout << "在内部命名空间中" << std::endl;
        }
    }
}

outer::inner::foo();

```

嵌套命名空间定义提供了一种将代码层次化组织的方式，提高了可读性和可维护性，尤其是在大型项目中。它们还通过提供更加结构化的命名空间层次结构来帮助避免命名冲突。

### 12. 字面量改进

C++17增强了字面量，包括对整数和浮点字面量的改进，以及对真和假字面量的支持。

例子：

```
auto num = 123_456; // Underscore in integer literals  
auto pi = 3.1415_f; // Suffix for floating-point literals
```

### 13. constexpr Lambda

C++17允许lambda函数成为constexpr，如果它们满足条件，就可以在需要编译时评估的上下文中使用，例如：

```cpp
constexpr auto lambda = [](int x) { return x * 2; };
static_assert(lambda(5) == 10);
```

在这个例子中，`lambda`是一个constexpr lambda，它接受一个整数x作为参数，然后返回x的两倍。`static_assert`检查在编译时，`lambda(5)`的值是否等于10。

### 14. 捕获*this

在lambda中捕获*this变得更加简单，允许直接访问包含对象的成员。

```cpp
class Foo {
    int data = 42;
public:
    auto member_lambda() {
        return [*this] { std::cout << data << std::endl; };
    }
};

// 使用
Foo f;
f.member_lambda()(); // 输出 "42"
```

### 15. 扩展的if和switch语句

`if`或`switch`语句中的条件现在可以是任何表达式，不仅限于布尔条件。这使得控制流更加灵活，例如使用结构化绑定时：

```cpp
if (const auto [it, inserted] = map.insert({"foo", bar}); inserted) {
    // ...
}
```

在此例子中，`if`语句检查`inserted`变量是否为真，但条件还包括结构化绑定的赋值。

### 16. 泛化的基于范围的for循环

此改进支持不同于起始迭代器类型的标志或结束迭代器，这有助于处理以空终止的循环和其他类似情况。例如：

```cpp
for (auto it = my_container.begin(); it != my_container.end(); ++it) {
   // ...
}
```

在此例子中，`my_container`可能是使用不同类型的结束迭代器的容器，但循环仍然可以正确工作。

### 17. if constexpr

此特性通过允许编译器在编译时评估条件，从而实现更通用的代码。如果条件为真，则编译的代码中包含`if`块内的代码；否则，它将被丢弃。这可以通过在运行时删除不必要的分支来简化代码。例如：

```cpp
if constexpr (std::is_same_v<T, int>) {
   // 用于int的特定代码
} else {
   // 用于其他类型的代码
}
```

在这个例子中，`if constexpr`语句检查类型`T`是否为`int`，并相应地包含适当的代码。

### 总结

本节就先写这么多，其他内容下一节进行阐述。