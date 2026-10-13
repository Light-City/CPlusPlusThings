> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/std-function-cpp/) 的文章，由站点同步脚本导出。

# C++ std::function 详解

最近在写C++代码时用到了`is_function`，然后顺便看了一下源码实现，发现了一些问题，以前咱们学习的是三个点`...`，那六个点`......`你知道是啥吗？

同样，如果有一段这样的代码：

```
template<typename... Args>
struct foo(Args......);
```

你知道是啥意思？

下面我们一起来深入细节！

## 1.六个点

六个点我们通常叫做two ellipsis operators，我们可以将六个点进行拆分，于是得到下面三种情况

```
template <typename... Args> void foo1(Args......)
template <typename... Args> void foo2(Args... ...)
template <typename... Args> void foo3(Args..., ...)
```

这下就好看了，我们来看std::is_function的代码。

## 2.is_function实现细节

`std::is_function`的实现涉及到主模板和两个偏特化：

```cpp
// 主模板，假设提供的类型不是函数类型
template<typename T>
struct is_function : std::false_type {};

// 偏特化，用于正常的函数类型
template<typename Ret, typename... Args>
struct is_function<Ret(Args...)> : std::true_type {};

// 偏特化，用于变参函数类型
template<typename Ret, typename... Args>
struct is_function<Ret(Args......)> : std::true_type {};
```

- 主模板假设提供的类型不是函数类型，并将`std::false_type`作为结果。
- 第一个偏特化用于正常的函数类型，其中`Ret`表示返回类型，`Args...`表示参数类型。如果提供的类型匹配这种函数签名，则`std::true_type`作为结果。
- 第二个偏特化用于变参函数类型，其中的省略号表示函数具有可变数量的参数。同样，如果提供的类型匹配这种函数签名，则`std::true_type`作为结果。

## 3.使用示例

最后给出一个`std::is_function`的使用示例：

```cpp
#include <iostream>
#include <type_traits>

int func(int x, int y) { return x + y; }
int vararg_func(int x, ...) { return x; }

int main() {
    std::cout << std::boolalpha;
    std::cout << "Is func a function? " << std::is_function<decltype(func)>::value << std::endl;
    std::cout << "Is vararg_func a function? " << std::is_function<decltype(vararg_func)>::value << std::endl;
    return 0;
}
```

这将输出：

```
Is func a function? true
Is vararg_func a function? true
```

你学会了吗，本节完~