> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp20-span/) 的文章，由站点同步脚本导出。

# C++20 std::span 设计与陷阱

[TOC]

## 0.初衷

在传统C/C++代码中，我们通常会使用一个指针+大小来访问数组，比如：

```cpp
void print(int* arr, int sz) {
	for (int i = 0; i < sz; i++) {
		// todo
	}
}
```

span可以简化这一操作，变为：

```cpp
void print(std::span<int> arr) {
	for (auto x : arr) {
		// todo
	}
}
```

那么究竟什么是span、它的使用场景以及陷阱有哪些？本节进行详细的探讨。

## 1. 何谓`span`？ 

在 C++20 中，`span` 是一个轻量级的、范围类型的容器，它提供了对**一段连续内存**的访问。`span` 作为一个视图（view），并不拥有其数据，而是对数据的一个简单引用，常用于替代传统的 C 风格数组或指针。它可以容纳任意类型的数组和标准容器，如 `std::vector`，并提供一种更安全、更现代的方式来处理这些数据。

其中几个关键点：

- 连续内存
- 不拥有数据
- 引用

`span` 的源码位于gcc的libstdc++-v3/include/std/span。

```cpp
template <typename _Type, size_t _Extent = dynamic_extent>
class span {};
```

`span` 类型包含两个重要的参数：

- `_Type`：元素类型。
- `Extent`：数组大小。如果为 `dynamic_extent`（默认值），表示视图的大小可以动态变化。

## 2. `span` 的动态与静态

`span` 有两种模式：静态和动态。

### 2.1 静态 `span`

静态 `span` 在编译时确定其大小，它的 `Extent` 是一个常量值。使用静态 `span` 时，我们需要在编译时知道数据的大小。

例如：

```cpp
int arr[5] = {1, 2, 3, 4, 5};
std::span<int, 5> s(arr);  // 静态 span，大小为 5
```

静态 `span` 的大小在编译时固定，因此可以利用编译器进行一些优化，比如边界检查的省略。但由于 `Extent` 是常量，因此静态 `span` 不能在运行时修改大小。

### 2.2 动态 `span`

动态 `span` 的大小在运行时确定，`Extent` 默认为 `dynamic_extent`。这种类型的 `span` 不要求在编译时确定大小，它可以适应任意大小的数组或容器。

例如：

```cpp
int arr[5] = {1, 2, 3, 4, 5};
std::span<int> s(arr);  // 动态 span，大小在运行时确定
```

动态 `span` 适用于那些无法在编译时确定大小的情况，它提供了更大的灵活性。

gcc源码里面实现比较简单，默认是一个非常大的整数(-1的无符号数)，真正计算的时候是在构造的时候。

```cpp
inline constexpr size_t dynamic_extent = static_cast<size_t>(-1);
```

比如：这里会使用ranges::size设置内部大小，具体见下面代码。

```cpp
std::vector<int> data{1, 2, 3};
std::span<int> s(data);
```

设置动态大小。

```cpp
span(ranges::data(__range), ranges::size(__range))
```

## 3. `subspan`

`span` 提供了一个非常有用的成员函数 `subspan`，它允许我们从一个现有的 `span` 中提取一个子视图，而无需复制数据。这非常适用于处理大数据集时，避免不必要的内存开销。

`subspan()` 方法有两个重载：

1. 一个接受 `__offset` 和 `__count` 作为参数，其中 `__offset` 是起始位置，`__count` 是子范围的大小（默认为 `dynamic_extent`，即到原 `span` 的末尾）。
2. 另一个没有参数，基于模版实现。

```cpp
constexpr auto
subspan() const noexcept
-> span<element_type, _S_subspan_extent<_Offset, _Count>()> {}

template<size_t _Offset, size_t _Count = dynamic_extent>
[[nodiscard]]
constexpr auto
subspan() const noexcept
-> span<element_type, _S_subspan_extent<_Offset, _Count>()> {}
```

示例1：

```cpp
std::span<int> s(arr);  
std::span<int> sub = s.subspan(1, 3);
```

示例2：

```cpp
std::span<int> s(arr);  
auto sub = s.subspan<2, 2>();
```

## 4. 使用陷阱

虽然 `span` 是一个非常有用的工具，但在使用时也有一些常见的陷阱需要注意：

### 4.1 不要让 `span` 引用已经释放的内存

`span` 本身并不拥有数据，它只是对数据的引用。因此，如果 `span` 引用的内存被释放（例如原数组被销毁或超出作用域），那么访问该 `span` 将导致未定义行为。

例如：

```cpp
std::span<int> create_span() {
    int arr[5] = {1, 2, 3, 4, 5};
    return std::span<int>(arr, 5);
}

// 错误：返回的 span 引用了栈上临时变量，函数返回后 arr 被销毁
auto s = create_span();
```

为了解决这个问题，应确保 `span` 引用的数据在其生命周期内有效。可以考虑将数据持有在动态分配的内存中，或者确保数据的生命周期和 `span` 的生命周期一致。

### 4.2 改变底层数组大小时的问题

`span` 对底层数据的引用是直接的，而不是复制。当底层的数据发生变化时，`span` 本身并不会感知这些变化。因此，改变底层数组或容器的大小（例如通过重新分配内存）时，原来的 `span` 可能会变得无效。

```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};
std::span<int> s(vec);  // s 引用 vec 的数据
// 改变底层容器的大小
vec.push_back(6);  // vec 大小变为 6

// 这里 s 仍然引用 vec 的数据，但 vec 可能已经重新分配了内存
// 这可能导致 s 引用失效或未定义行为
```