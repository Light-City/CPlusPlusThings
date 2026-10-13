> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp20-constexpr-comma/) 的文章，由站点同步脚本导出。

# C++20 constexpr 与逗号表达式

### C++20那些事之constexpr与comma expr

C++20 引入了多项新特性，进一步增强了编译时能力和代码安全性。本文将深入探讨两项重要的变更：**constexpr 函数中的异常处理**以及**下标运算符中逗号运算符的弃用**。

**注：懒人版，本节的代码示例与相关文档已更新在星球，感兴趣加入即可。**

#### 1. constexpr 函数中支持 try-catch 异常处理的变更

在 C++20 之前，`constexpr` 函数中不能使用 `try-catch` 块，这一限制阻碍了一些标准库容器如 `std::array` 的编译时处理能力。C++20 对此进行了改进，**允许在 `constexpr` 函数中使用 `try-catch` 块，并且在常量表达式的上下文中忽略这些异常处理结构**。当在常量表达式计算中抛出异常时，编译器会报错。

这项改动的主要目的是让标准库中的容器能够支持 `constexpr`，例如 `std::array`，同时为将来在编译时支持更多异常处理提供了基础。

**示例**：

```cpp
constexpr int SafeDivide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero!");  // 当除数为0时抛出异常
    }
    return a / b;
}

constexpr int ComputeQuotient(int x) {
    try {
        return SafeDivide(100, x);  // 尝试除法运算
    } catch (const std::runtime_error&) {
        return -1;  // 捕获除零异常并返回 -1
    }
}
```

上面代码在C++20之前编译不过，那么C++20对这个行为是什么呢，可以猜一下下面两个结果：

```cpp
static_assert(ComputeQuotient(25) == 4); 
static_assert(ComputeQuotient(0) == -1); 
```

#### 2. 下标运算符中的逗号运算符弃用

C++20 的另一项变更是**弃用在下标运算符中使用逗号运算符**。在早期的 C++ 版本中，表达式 `array[x, y]` 中，逗号运算符会使 `y` 作为下标传递给数组或容器，而 `x` 被忽略。然而，这种用法会导致代码不易理解，并且妨碍未来多维数组的实现。因此，自 C++20 起，**下标运算符中的逗号运算符被弃用**，除非使用括号明确指定优先级，如 `array[(x, y)]`。

**示例代码**：

```cpp
// arr[i, j] = 10;  // C++17 中合法，C++20 中弃用
arr[(i, j)] = 10;  // 使用括号以明确逗号的优先级
```

在这个例子中，`arr[(i, j)]` 使用了括号以明确逗号运算符的优先级，确保 `j` 作为下标传递给数组。在 C++17 中，`arr[i, j]` 是允许的，但在 C++20 中，这样的用法会触发弃用警告。通过这种方式可以确保代码的行为一致，并符合 C++20 的新标准。

如果使用了逗号运算符编译器将会警告：

```cpp
common.cc:9:10: warning: top-level comma expression in array subscript is deprecated [-Wcomma-subscript]
    9 |     arr[i, j] = 10;  // C++17 中合法，C++20 中弃用
      |          ^
```

### 总结

C++20 中的这些变更，特别是 `constexpr` 函数中的异常处理支持和下标运算符中逗号运算符的弃用，都是为了提升编译时能力和代码的可读性。如果你的代码依赖这些旧特性，建议及时进行调整，以适应 C++20 的新标准。