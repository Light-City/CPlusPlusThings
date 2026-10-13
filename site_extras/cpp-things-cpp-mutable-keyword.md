> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-mutable-keyword/) 的文章，由站点同步脚本导出。

# C++ mutable 关键字

### 深入探讨 C++ 中的 `mutable` 关键字

在 C++ 中，`mutable` 关键字经常被认为是“穿透”常量（`const`）限制的工具。在某些情况下，虽然我们希望对一个对象施加常量保护，但还是需要对该对象的某些数据成员进行修改。本文将详细探讨 `mutable` 的使用场景和背后的设计哲学，并讨论其如何在 C++ 中解决特定的编程需求。

#### 1. `mutable` 关键字的基本用途

我们知道，当一个成员函数被 `const` 修饰时，该成员函数保证不会修改对象的状态。然而，在实际编程中，有时我们希望修改一个数据成员，但仍然希望将成员函数标记为 `const`。这就是 `mutable` 关键字的作用：

```cpp
class Foo  
{  
private:  
    mutable bool done_;  
public:  
    void doSomething() const {
        // 在 const 成员函数中修改 mutable 成员
        done_ = true;
    }  
};
```

在上面的例子中，`doSomething()` 被声明为 `const`，表明它不会改变对象的状态。然而，`done_` 被标记为 `mutable`，这意味着即使在 `const` 方法中，我们仍然可以修改 `done_`。这种设计使得我们可以在保持函数接口语义不变的情况下，管理某些需要修改的数据。

#### 2. 逻辑常量性 vs. 位常量性

C++ 中的常量性可以分为两类：**逻辑常量性** 和 **位常量性**。逻辑常量性指的是对象的状态在外部看来没有发生变化，而位常量性指的是对象在内存中的位数据保持不变。`mutable` 关键字允许我们定义逻辑常量性，即即使对象的某些内部数据变化了，但在逻辑上对象的状态并未发生可见变化。

比如在一个线程安全的类中，我们可能会使用 `boost::mutex` 来保护数据访问。为了在 `const` 方法中获取锁，我们可以将 `mutex` 声明为 `mutable`：

```cpp
class ThreadSafeClass {
private:
    mutable std::mutex mtx_;
    int data_;
public:
    int getData() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return data_;
    }
};
```

在上述例子中，虽然 `getData` 方法被声明为 `const`，但我们需要锁来保护数据访问的线程安全性。因此，我们将 `mtx_` 声明为 `mutable`，以允许在 `const` 方法中对其进行修改。这种情况下，我们说类的逻辑状态没有改变，只是为确保线程安全而进行的修改。

#### 3. 在 Lambda 表达式中的 `mutable`

从 C++11 开始，`mutable` 也可以用于 lambda 表达式，以允许修改按值捕获的变量。默认情况下，lambda 中按值捕获的变量是不可变的。如果需要在 lambda 中修改捕获的变量，可以使用 `mutable`：

```cpp
int x = 0;
auto f1 = [=]() mutable { x = 42; };  // OK: 可以修改捕获的 x
auto f2 = [=]() { x = 42; };  // Error: 按值捕获的变量不能被修改
```

这种情况下，`mutable` 允许我们在函数体内修改捕获的变量，而不会影响到外部变量的值。

#### 4. 避免使用 `const_cast`

没有 `mutable` 关键字的情况下，我们可能需要使用 `const_cast` 来处理这些特殊情况。`const_cast` 可以去掉对象的常量性，但它也有缺点：`const_cast` 可以完全移除常量性保护，让我们对对象进行任意的修改和方法调用，这会导致代码的可读性和安全性下降。

相比之下，`mutable` 关键字提供了一种更细粒度的控制方式。通过 `mutable`，我们可以在特定的数据成员上设置“例外”，允许在 `const` 方法中修改，而不必将整个对象的常量性去掉。因此，`mutable` 能够更安全地处理这些特例。

#### 5. 使用 `mutable` 的设计哲学

`mutable` 关键字的设计哲学在于保持接口的**逻辑常量性**。在大多数情况下，我们希望编写 const-correct 的代码，这意味着对象的状态在没有显式修改的情况下不会变化。`mutable` 允许我们在某些内部实现上做出让步，比如缓存、引用计数、调试信息等，而不会打破这种接口设计的一致性。

设计一个接口时，我们应该明确何时以及为什么需要修改一个对象的状态。`mutable` 关键字能让代码在逻辑层面保持不变，同时又提供了修改特定数据成员的能力。这需要设计者深思熟虑，确保 `mutable` 的使用不会破坏对象的常量性概念，只有在确实需要的情况下使用。

### 6.参考资料

> https://stackoverflow.com/questions/105014/does-the-mutable-keyword-have-any-purpose-other-than-allowing-a-data-member-to