> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/likely-unlikely-attributes/) 的文章，由站点同步脚本导出。

# C++20 [[likely]] 与 [[unlikely]]

C++20 引入了 `[[likely]]` 和 `[[unlikely]]` 这两个可能性属性，它们会给编译器优化提示，表示对应代码路径可能/不可能被执行。

乍一看，这似乎是一组很不错的属性，通过编译器优化，从而提高性能。但是事实并不是如此。

在C++ 标准草案中明确提到：Excessive usage of either of these attributes is liable to result in performance degradation.

即：过度使用任何一个属性都可能导致性能下降。

> https://eel.is/c++draft/dcl.attr.likelihood#note-2

那么在什么情况下会比较有用，什么情况下又不需要呢？

本文谈谈自己的观点。

## 1.原则

使用时应该遵循以下原则：

1. **谨慎使用**：仅在你确定某个分支或路径在运行时极其可能或不可能时使用这些属性。

例如：下面这个判断一个value是不是大于0，如果输入的数据无法保证value很大概率/一定是大于0的，那么下面这个优化没有任何意义。

```cpp
if (value > 0) [[likely]] {
    std::cout << "Value is positive: " << value << std::endl;
} else [[unlikely]] {
    std::cout << "Value is non-positive: " << value << std::endl;
}
```

例如：标准库中使用unlikely的示例。`_Variant_storage`结构体中的成员函数使用了这个属性，无效的概率非常低，所以可能使用unlikely去标记。

```cpp
constexpr void
_M_reset()
{
  if (!_M_valid()) [[unlikely]]
    return;

  // do something
}
```

2. **标记主导路径**：只在主导执行路径上使用属性，而不是在所有可能路径上。

例如：下面一个if判断标记了两个等效的likely，那么这个意义不大。

```cpp
if (value > 10) [[likely]] {
    // 主导路径：值大于 10
    std::cout << "Value is greater than 10: " << value << std::endl;
} else if (value > 5) [[likely]] {
    // 次主导路径：值大于 5 且小于等于 10
    std::cout << "Value is greater than 5 but less than or equal to 10: " << value << std::endl;
} else [[unlikely]] {
    // 较少出现的路径：值小于等于 5
    std::cout << "Value is 5 or less: " << value << std::endl;
}
```

## 2.示例

最后举一个比较常见的例子，那便是vector的at，我们知道它会判断是否超过vector的size，这种概率比较低，所以我们可以使用unlikely，例如：

```cpp
int& at(std::vector<int>& vec, std::size_t i) {
  if (i >= vec.size()) [[unlikely]] {
      throw std::out_of_range("Out of bounds access");
  }
  return vec[i];
}
```