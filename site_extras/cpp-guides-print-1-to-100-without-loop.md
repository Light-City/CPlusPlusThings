> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-guides/print-1-to-100-without-loop/) 的文章，由站点同步脚本导出。

# C++ 无循环打印 1-100

给大家设定一个条件：不使用循环，请打印出1-100个数字。

如果使用循环很简单了，那么你能想到多少种方法呢？

今天来给大家用一些C++的骚操作来写出这种代码，欢迎留言与转发～

这道题目其实非常简单，但却涵盖了很多面试考点，其中包括：

- 模版特化、便特化，如何编写递归模版
- CRTP
- std::copy、std::iota、std::transform如何使用，各自算法实现
- 类静态变量
- 等等

## 1.递归

1-100可以拆为`5*5*4`，用三个函数去打印它，函数a cout一次，然后变量加1，函数b层层嵌套a重复5次，函数c层层嵌套b重复5次，最后在main函数中对c层层嵌套4次。

```cpp
#include <iostream>

int a(int n) {
  std::cout << n++ << std::endl;
  return n;
}

int b(int n) { return a(a(a(a(a(n))))); }

int c(int n) { return b(b(b(b(b(n))))); }

int main() {
  c(c(c(c(1))));
  return 0;
}
```

## 2.模版重载

使用模版与特化来处理，当迭代100时只输出100，否则输出I，并且+1递归处理。

```cpp
#include <iostream>
template <int I>
struct JustPrint : public JustPrint<I + 1> {
  JustPrint() : JustPrint<I + 1>() { std::cout << I << " "; }
};

template <>
struct JustPrint<100> {
  JustPrint<100>() { std::cout << 1 << " "; }
};

int main() { JustPrint<1> j; }
```

## 3.CRTP

方式3基于方式2，将其改造为crtp编程风格，是不是有点看不懂了，嘿嘿。

下面struct可以被替换为class，这里我用类来描述。其实就是两个类，第一个类Print为父类，打印每次的I，并调用子类，子类PrintDerived即成Print，并将自己传递给父类，子类是一个重载类，控制递归终止。

```cpp
#include <iostream>

template <typename Derived, int I>
struct Print {
  Print() {
    std::cout << I << " ";
    Derived::next();
  }
};

template <int I>
struct PrintDerived : public Print<PrintDerived<I>, I> {
  static void next() { PrintDerived<I + 1>(); }
};
template <>
struct PrintDerived<100> : public Print<PrintDerived<100>, 100> {
  static void next() {}
};

int main() {
  PrintDerived<1> j;
  return 0;
}
```

## 4.类静态变量

创建一个Test数组，元素为100个，静态变量每次累加。

```cpp
#include <iostream>
using namespace std;

class Test {
  static int i;

 public:
  Test() { cout << ++i << endl; }
};
int Test::i;
int main() {
  Test a[100];
  return 0;
}
```

## 5.使用算法库

使用`std::for_each`简单粗暴。

```cpp
int main() {
  std::vector<int> l(100);
  std::iota(l.begin(), l.end(), 1);
  std::for_each(l.begin(), l.end(), [](int& n) { std::cout << n << std::endl; });
  return 0;
}
```

## 6.C++20 ranges

使用C++20的std::views，在std::copy时打印。

```cpp
#include <iostream>
#include <iterator>
#include <ranges>
int main() {
  auto numbers = std::views::iota(1, 100 + 1);
  std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
```

这个方式继续改造，还有一种实现，使用std::transform。

```cpp
auto range = std::views::transform(std::ranges::iota_view{1, 101}, [](const auto& x) {
  std::cout << x << std::endl;
  return x;
});
std::vector<int> a(range.begin(), range.end());
```