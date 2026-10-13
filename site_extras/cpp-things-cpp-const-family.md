> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-const-family/) 的文章，由站点同步脚本导出。

# C++ const 全家桶

大纲

- const
- constexpr
- C++20
  - consteval
  - constinit

**注：本文所有代码示例已放星球。**

## 1.const

`const` 是我们在学习 C++/C 时最早接触的关键字之一，通常作为 `cv-qualifier` 使用，表示对象不可变。它并不保证 **编译时评估**，具体何时评估由编译器决定。例如：

```cpp
const int a = 1; // 编译时
const auto x = getdata(); // 运行时
```

const有时可以用于编译时常量表达式，但是有一些例外，例如：

```cpp
const double count_1 = 3.3;
std::array<double, static_cast<int>(count_1)> moreDoubles1 {1.1, 2.2, 3.3};
```

此时便会报错：the value of 'count_1' is not usable in a constant expression

但是这样是正常的：

```cpp
const int count = 3;
std::array<double, count> doubles {1.1, 2.2, 3.3};
```

本质在于编译时表达式求值，于是C++11引入了constexpr。

## 2.constexpr

constexpr 可以用来指定变量的值可以出现在常量表达式，此时只需要将前面例子的const变为constexpr，便可以正常运行。

```cpp
constexpr double count_2 = 3.3;
std::array<double, static_cast<int>(count_2)> moreDoubles2 {1.1, 2.2, 3.3}; 
```

`constexpr` 并不一定只能在编译时执行。它的本质含义是 **可以** 在编译时计算，但如果在运行时使用 `constexpr` 变量或函数，它也可以在运行时执行。

例如：result1与result2完全不一样，result1是编译时计算，但是result2是运行时计算的，抛个疑问：**你知道为什么吗？**

所以并不是指定了constexpr的函数它就一定是编译时计算的。

```cpp
constexpr int compileTimeSum(int a, int b) {
    return a + b;
}

int main() {
    constexpr int result1 = compileTimeSum(3, 5); 
    int x = 10;
    int result2 = compileTimeSum(x, 20);  
    return 0;
}
```

那么谁能保证呢？于是C++20来了。

## 3.C++20

### 3.1 consteval

consteval解决了上面的问题，它只能用于函数，并强制所有调用在编译时进行。如果我们此时将上面的constexpr改为consteval，此时result2会发生什么呢？

那便是报错：call to consteval function '**compileTimeSum(x, 20)**' is not a constant expression

```cpp
consteval int compileTimeSum(int a, int b) {
    return a + b;
}

int main() {
    constexpr int result1 = compileTimeSum(3, 5); 
    int x = 10;
    int result2 = compileTimeSum(x, 20);  
    return 0;
}
```

如果使用了consteval使用在变量上，也会报错：**error:** a variable cannot be declared '**consteval**'。

> https://en.cppreference.com/w/cpp/language/consteval

### 3.2 constinit

constinit可以强制静态或线程局部变量进行常量初始化。

- 不能与 constexpr、consteval 一起使用
- 一个非类型说明符，作用是assert
- 保证变量是静态初始化
- 只能用于 static 或者 thread_local

例如：下面会编译报错：error: 'constinit' can only be applied to a variable with static or thread storage duration

```cpp
constinit int a = 100;  
int main() {  
      ++a; 
      constinit auto b = 100;  // error
      // b has thread storage duration
      constinit thread_local auto b = 100;
}
```