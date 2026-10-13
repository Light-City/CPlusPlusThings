> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/spaceship-operator/) 的文章，由站点同步脚本导出。

# C++20 三路比较运算符 <=>

## 三种排序

三向比较运算符`<=>`又称为宇宙飞船运算符，返回值有三种排序，下面从gcc源码角度来深入聊一下。

注：懒人版，本节示例已更新星球。

用法：`<=>`

```cpp
#include <compare>
#include <iostream>

int main() {
   auto c1 = 1.1 <=> 2.2;
   auto c2 = -1 <=> 1;
   std::cout << typeid(c1).name() << std::endl;
   std::cout << typeid(c2).name() << std::endl;
}

```

输出：

```cpp
St16partial_ordering
St15strong_ordering
```

1. `1.1 <=> 2.2`：这是对两个浮点数进行比较。对于浮点数，标准库返回的是`std::partial_ordering`，因为浮点数有NaN值的存在，这使得它们之间的比较不是严格的全序。`std::partial_ordering`允许结果是`<`、`>`、`==`或者`unordered`。

我们可以使用gcc代码静态变量来查看partial_ordering到底是小于、大于、等于、无序：

```cpp
static const partial_ordering less;
static const partial_ordering equivalent;
static const partial_ordering greater;
static const partial_ordering unordered;
```

如果我们强制转换便会报错：

```cpp
comp.cc:6:36: error: conversion from 'std::partial_ordering' to non-scalar type 'std::strong_ordering' requested
    6 |   std::strong_ordering c1 = 1.1 <=> 2.2;
```

	2. `-1 <=> 1`：这是对两个整数进行比较。对于整数，标准库返回的是`std::strong_ordering`，因为整数的比较是严格的全序关系，没有不确定的情况。`std::strong_ordering`只会返回`<`、`>`或`==`。

同理，可以使用下面4个静态成员。

```cpp
static const strong_ordering less;
static const strong_ordering equal;
static const strong_ordering equivalent;
static const strong_ordering greater;
```

因此，`c1`的类型是`std::partial_ordering`，而`c2`的类型是`std::strong_ordering`，这也解释了为什么输出的类型不一样。

除了partial_ordering、strong_ordering之外，`<=>`还可能返回weak_ordering，静态成员为：

```cpp
static const weak_ordering less;
static const weak_ordering equivalent;
static const weak_ordering greater;
```

## equal与equivalent

在上面的三种ording可以看到对于等于出现了两个：equal与equivalent，这两者有什么区别呢？

这里直接看cppreference，两者之间没有区别。然而，std::strong_ordering 是唯一定义相等的类别。所有其他类别仅定义等价。也正好验证了上面的静态成员，对于string_ordering有两个，其他的只有equivalent。

如果比较在被比较的项目之间产生**强排序**，则等价和相等是相同的。之所以有两个词来表示它，是因为这对于其他类型的排序是不一样的。**弱**和**部分**根本**没有相等性**；它们**只提供等价性**。

等价意味着两个对象可以比较相等。相等意味着更强的东西；如果它们比较相等，则可以在任何 const 用法中用一个替换另一个：只要 a == b 为真，f(a) == f(b) 的属性就为真，其中 f 表示仅读取可通过参数的公共 const 成员访问的比较突出状态的函数。如果类型的比较允许相等（这是强排序的要求），那么它也允许等价。因此，对于强排序比较，它们是相同的。

> https://en.cppreference.com/w/cpp/utility/compare/strong_ordering

## 为什么使用三向比较运算符？

三向比较运算符（<=>）使得在一次操作中就能够确定两个值的排序关系，而传统的比较运算符则需要多次比较。

**传统比较运算符的限制：**

- 如果 `a == b` 为假，你无法知道 `a < b` 还是 `a > b`。
- 如果 `a != b` 为真，你无法知道 `a < b` 还是 `a > b`。
- 如果 `a < b` 为假，你无法知道 `a == b` 还是 `a > b`。
- 如果 `a > b` 为假，你无法知道 `a == b` 还是 `a < b`。
- 如果 `a <= b` 为真，你无法知道 `a == b` 还是 `a < b`。
- 如果 `a >= b` 为真，你无法知道 `a == b` 还是 `a > b`。

这些限制表明，传统的比较运算符不能一次性确定两个值的完全关系，而是需要多次比较来获得完整的结果。

> 本节完