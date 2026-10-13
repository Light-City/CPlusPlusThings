> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-idioms/lazy-evaluation-cpp/) 的文章，由站点同步脚本导出。

# C++ 延迟计算惯用法

---

在软件开发中，延迟计算(**lazy evaluation or call-by-need**)是一种强大的技术，它允许我们推迟表达式的计算，直到真正需要的时候才执行。

这种技术在处理大规模数据集或需要动态计算的情况下特别有用。

### 1.什么是延迟计算？

延迟计算是指推迟表达式的计算，直到它们真正需要的时候才执行。这种技术的优点在于可以提高程序的效率和性能，尤其是在处理大规模数据集或需要动态计算的情况下。

延迟计算的实现通常通过延迟函数调用或延迟表达式求值来实现。这意味着表达式的计算被推迟到它们被使用的时候，而不是在定义它们的时候就立即执行。

### 2.几种实现方法

在 C++ 中，我们有多种方法来实现延迟计算。下面是一些常见的方法：

1. **Lambda 表达式**：Lambda 表达式可以用来表示一个匿名函数，可以在需要的地方进行延迟计算。

```cpp
auto delayedCalculation = []() {
    std::cout << "延迟计算" << std::endl;
    return 42;
};
```

2. **std::function**：`std::function` 可以用于封装函数，使得函数的执行可以延迟到后面的某个时刻。

```cpp
std::function<int()> delayedCalculation = []() {
    std::cout << "延迟计算" << std::endl;
    return 42;
};
```

3. **std::bind**：`std::bind` 可以用于绑定函数和参数，并生成一个可调用对象，延迟执行函数调用。

```cpp
auto delayedCall = std::bind(delayedCalculation, 42);
```

4. **函数指针**：通过指向函数的指针，可以在需要的时候调用相应的函数，实现延迟计算。

```cpp
void delayedCalculation() {
    std::cout << "延迟计算" << std::endl;
}

void (*funcPtr)() = &delayedCalculation;
(*funcPtr)();
```

5. **惰性求值**：惰性求值是一种延迟计算的范式，它延迟表达式的求值直到它真正需要的时候。

```cpp
auto it = std::begin(vec);
std::cout << "第一个元素：" << *it << std::endl;
```

6. **生成器**：生成器是一种用于延迟生成序列的技术，允许按需生成序列中的元素，而不是一次性生成所有元素。

```cpp
std::vector<int> generateSequence(int n) {
    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        result.push_back(i);
    }
    return result;
}
```

除此之外，还有哪些实现呢？欢迎与我一起探讨。