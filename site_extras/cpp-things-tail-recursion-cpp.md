> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/tail-recursion-cpp/) 的文章，由站点同步脚本导出。

# C++ 尾递归

尾递归优化是一种特殊的优化技术，适用于递归函数，它可以极大地减少递归调用的栈空间占用。通过尾递归优化，编译器可以在执行尾递归时避免创建新的栈帧，直接复用当前的栈帧，从而降低栈溢出的风险，并提高性能。

[TOC]

### 什么是尾递归？
递归调用在返回时，**没有其他操作**需要执行，这样的递归称为**尾递归**。在这种情况下，编译器可以用一个循环结构替代递归，避免多次函数调用的开销。

### 非尾递归的例子（斐波那契数列）
你提供的 `fibonacci_non_tail()` 函数是一个标准的非尾递归实现：

```cpp
int fibonacci_non_tail(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci_non_tail(n - 1) + fibonacci_non_tail(n - 2);
}
```

在这个实现中，递归调用 `fibonacci_non_tail(n - 1)` 和 `fibonacci_non_tail(n - 2)` 之后，还需要将它们的结果相加，因此这不是尾递归。每次调用都需要保存上下文信息，以便在递归返回后继续计算，这导致栈空间快速增长。

### 尾递归优化的示例
`fibonacci_tail()` 使用了尾递归的方式：

```cpp
int fibonacci_tail_helper(int n, int a, int b) {
    if (n == 0) {
        return a;
    }
    return fibonacci_tail_helper(n - 1, b, a + b);
}

int fibonacci_tail(int n) {
    return fibonacci_tail_helper(n, 0, 1);
}
```

这里的 `fibonacci_tail_helper()` 是尾递归。你可以看到，递归调用 `fibonacci_tail_helper(n - 1, b, a + b)` 后，函数不再需要做任何其他工作，直接返回递归调用的结果。因此，递归调用位于函数的“尾部”，没有额外的操作需要在递归调用后执行。

### 尾递归优化的优势
- **减少栈空间使用**：由于不需要保存递归调用的上下文，编译器可以复用栈帧，避免栈空间膨胀。
- **防止栈溢出**：尾递归优化允许递归深度达到非常大的值而不会发生栈溢出。
- **更高效**：在支持尾递归优化的编译器中，尾递归的性能接近于迭代实现，消除了递归函数调用的开销。

### 性能测试

最后针对上面做了一个benchmark，发现尾递归优化在这个场景下非常有用，可以看到性能的差异很大！