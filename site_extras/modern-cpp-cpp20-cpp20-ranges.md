> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp20-ranges/) 的文章，由站点同步脚本导出。

# C++20 Ranges 库

[TOC]

ranges是C++20的主要特性之一，其中"view"是比较重要的一部分。C++20之前，标准库的算法实现是基于迭代器来实现的，例如：std::sort。

```cpp
std::sort(v.begin() + 2, v.end())
```

迭代器 + 算法能够完成一些复杂的操作，例如：我想要倒这排序：

```cpp
std::sort(v.rbegin(), v.rend())
```

但是它也伴随着一些问题：

- 容易混用两个不兼容的迭代器。
- 算法的组合能力太弱，需要存储一些中间变量

例如：现在有一个学生信息系统，我们想要计算**年龄在21-25区间且GPA >= 3.5，求取满足前面条件的学生总GPA**。C++17之前我们可以写出下面这样的伪代码：

```cpp
std::vector<Student> students
std::vector<Student> selected;
std::copy_if(students.begin(), students.end(), std::back_inserter(selected),
             [](const Student& student) {
               return student.age >= 21 && student.age <= 25 && student.gpa >= 3.5;
             });

double s = std::accumulate(
    selected.begin(), selected.end(), 0.0,
    [](double sum, const Student& student) { return sum + student.gpa; });
```

可以看到我们在过滤前面满足条件的学生信息时需要将其拷贝到selected，然后再对其求和。

C++20 引入了一种更为简洁、高效的写法，通过使用范围和管道操作符 `|` 连接多个操作，可以在不需要中间变量的情况下直接求和，例如：

```cpp
double s = 0.0;
for (const auto& student :
     students | std::views::filter([](const auto& s) {
       return s.age >= 21 && s.age <= 25 && s.gpa >= 3.5;
     }) | std::views::transform([](const auto& s) { return s.gpa; })) {
  s += student;
}
```

可以看到多个算法之间无缝衔接，减少了中间临时变量的内存分配，其可读性也大大增加，通过管道`|`将不同的操纵连接在一块。

接下来，让我们一起探讨C++20 ranges相关的内容。

## 1.基础概念

1.range

range 是一种表示一个序列的抽象概念。它可以是任何具有迭代器的容器或者是一个定义了 `begin()` 和 `end()` 函数的对象。如 `std::vector`、`std::list` 等都是范围的例子。对于数组，也可以视为范围。

2.view

view 是对 Range 的一种只读访问。它是一种惰性计算的方式，只有在需要的时候才会进行计算，这意味着它并不实际存储数据。例如： `std::views::filter` 和 `std::views::transform` 就是view的典型例子。它们允许我们对 range 进行筛选和转换，而不必实际创建新的容器。

3.algorithm

算法是对range或view进行操作的函数，例如：`std::sort`、`std::find` 等都是算法的例子。

4.`|`

管道操作符`|`，可以将视图与算法链接起来，将左侧的结果作为右侧的输入。它使得代码更为清晰、简洁。例如： `students | std::views::filter(...)` 将 `students` 范围传递给 `std::views::filter` 进行过滤操作，然后再将结果传递给后续的操作。

以上面的student计算为示例，在这个例子中我们使用了范围students通过`|`作为视图filter的输入，然后将结果作为视图transform的输入，最后返回一个范围，基于这个范围进行循环，通过累加算法求和得到结果。

```cpp
double s = 0.0;
for (const auto& student :
     students | std::views::filter([](const auto& s) {
       return s.age >= 21 && s.age <= 25 && s.gpa >= 3.5;
     }) | std::views::transform([](const auto& s) { return s.gpa; })) {
  s += student;
}
```

可以看到，使用range有如下好处：

- 提高代码可读性
- 懒惰计算： Ranges 引入了视图（view）的概念，允许懒惰计算，即在需要时才计算结果。

例如：只有在`*v.begin()`时才会去计算。

```cpp
auto v = std::views::reverse(vec);
std::cout << *v.begin() << std::endl;
```

- 减少错误： Ranges 的设计有助于减少一些传统迭代器操作中容易出现的错误，例如使用不兼容的迭代器。
- 等等。

范围概念引入了不同的概念来描述不同类型的范围。这些概念有助于在泛型编程中更好地理解和限制范围的特性。以下是一些常用的范围概念：

> https://en.cppreference.com/w/cpp/ranges

| 概念                               | 描述                              | 容器举例                                                     |
| ---------------------------------- | --------------------------------- | ------------------------------------------------------------ |
| `std::ranges::input_range`         | 可以从头到尾至少迭代一次          | std::forward_list、std::list、std::duque、std::array、std::vector |
| `std::ranges::forward_range`       | 可以从头到尾迭代多次              | std::forward_list、std::list、std::duque、std::array、std::vector |
| `std::ranges::bidirectional_range` | 迭代器也可以向后移动`--`          | std::list、std::duque、std::array、std::vector               |
| `std::ranges::random_access_range` | 你可以在常数时间内跳转到元素 `[]` | std::duque、std::array、std::vector                          |
| `std::ranges::contiguous_range`    | 元素总是连续存储在内存中          | std::array、std::vector                                      |

## 2.使用

使用这个特性比较简单，只需要引入头文件，使用接口即可。

例如：过滤一堆数字当中的偶数。

```cpp
#include <ranges>
auto evenNumbers = numbers | std::views::filter([](int x) { return x % 2 == 0; });
```

编译：指定-std即可。

```cpp
g++ -std=c++20 main.cc -o main
```

编译器支持可以阅读下面清单：

> https://en.cppreference.com/w/cpp/compiler_support/20

gcc >=10，clang >=13(partial)，>=15全面支持。