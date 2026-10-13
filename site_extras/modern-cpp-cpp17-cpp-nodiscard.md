> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/cpp-nodiscard/) 的文章，由站点同步脚本导出。

# C++17 [[nodiscard]] 属性

自C++17引入[[nodiscard]]属性以来，我们在编写API时有了一种更强大的工具，用于标记那些在调用时不应该被忽略的函数返回值。在本文中，我们将深入探讨[[nodiscard]]的用法，并注意一些潜在的陷阱。

## 什么是[[nodiscard]]？

[[nodiscard]]是一种函数属性，它告诉编译器函数的返回值不应该被忽略。这在那些依赖于返回值的函数中尤为有用，例如谓词函数或工厂函数。

## 如何使用[[nodiscard]]？

在类、构造函数以及枚举上使用[[nodiscard]]都是可能的。下面是一些示例：

```cpp
class [[nodiscard]] A {};

class B {
public:
    [[nodiscard]] B() = default;
};

enum class [[nodiscard]] State {
    Default,
    Foo,
    Bar
};
```

在上述例子中，编译器将在返回值被忽略时发出警告，有助于我们及早发现潜在的bug。

## 警惕：[[nodiscard]]的陷阱

尽管[[nodiscard]]可以有效地防止返回值被无意中忽略，但我们需要注意一些潜在的陷阱。一个常见的陷阱是使用[[nodiscard]]的函数返回一个类似于作用域守卫的对象，以在作用域结束时执行清理操作（比如提交事务）。

```cpp
[[nodiscard]] ScopeGuard createScopeGuard() {
    // Implementation of a scope guard
}

// 在这里，用户可以选择忽略返回值，导致作用域立即销毁，从而失去清理效果
std::ignore = createScopeGuard();
```

尽管[[nodiscard]]表明函数返回值不应该被忽略，但用户仍然可以通过`std::ignore`等手段明确地忽略返回值，从而绕过清理操作。

## 完整示例

```cpp
#include <tuple> // std::ignore

class [[nodiscard]] A {};
class B {
public:
    [[nodiscard]] bool isOk() const { return true; }
};

// no need for nodiscard, A is nodiscard
A foo() {
    return A();
}

// B is not nodiscard, mark it explicitly
[[nodiscard]] B bar() {
    return B();
}

int main()
{    
    foo(); // warning: ignoring temporary created by a constructor declared with 'nodiscard' attribute
    bar(); // warning

    std::ignore = foo(); // ok
    auto b = bar(); // ok
    b.isOk(); // warning
    if (b.isOk()) {} // ok
}
```