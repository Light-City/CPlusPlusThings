> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/cpp-string-view/) 的文章，由站点同步脚本导出。

# C++17 std::string_view

## 0.问题

string_view为c++17之后出现，其实就是leveldb中的slice，其目的在于解决内存拷贝、高效的substr。

但是，也引入了指针与引用的问题，由于string_view不拥有指向内容的所有权，对比于Rust，仅仅是借用，如果拥有内容的对象提前释放，便会出现悬挂引用问题。像Rust在编译时会分析变量的生命周期，保证借用的资源在使用时不会释放。然而C++没有检查，需要人工保证。

### 1.内部实现

string_view是一个basic_string_view类型，可以看到_Traits采用默认的`char_traits`，这个很重要，后面的一些函数都采用`char_traits`的一些成员函数执行核心操作。

```cpp
template<class _CharT, class _Traits = char_traits<_CharT> >
    class _LIBCPP_TEMPLATE_VIS basic_string_view;

typedef basic_string_view<char>     string_view;
```

basic_string_view内部成员：

```cpp
private:
    const   value_type* __data;
    size_type           __size;
```

构造兼容 string与`const char*`

```cpp
constexpr basic_string_view() noexcept;
constexpr basic_string_view(const basic_string_view&) noexcept = default;
basic_string_view& operator=(const basic_string_view&) noexcept = default;
template<class Allocator>
constexpr basic_string_view(const charT* str);
constexpr basic_string_view(const charT* str, size_type len);
```

迭代器支持：

```cpp
constexpr const_iterator begin() const noexcept;
constexpr const_iterator end() const noexcept;
constexpr const_iterator cbegin() const noexcept;
constexpr const_iterator cend() const noexcept;
const_reverse_iterator rbegin() const noexcept;
const_reverse_iterator rend() const noexcept;
const_reverse_iterator crbegin() const noexcept;
const_reverse_iterator crend() const noexcept;
```

其他核心函数：

- substr
- remove_prefix
- compare
- find
- ...

从c++20开始支持starts_with、ends_with、contains，这三个在leveldb的slice中一开始就支持了。

`char*`的string_view字面量：

```cpp
constexpr basic_string_view<char>     operator "" sv( const char *str,     size_t len ) noexcept;
constexpr basic_string_view<wchar_t>  operator "" sv( const wchar_t *str,  size_t len ) noexcept;
constexpr basic_string_view<char16_t> operator "" sv( const char16_t *str, size_t len ) noexcept;
constexpr basic_string_view<char32_t> operator "" sv( const char32_t *str, size_t len ) noexcept;
```

## 2.使用陷阱

### 2.1 `\0`

string_view不关心`\0`，会关心实际的字符个数。 

```cpp
std::string_view s = "abc\0\0def"sv;
std::cout << "s: " << s.size() << " \"" << s << "\"\n";
```

输出：s: 8 "abcdef"

```cpp
std::string_view str{"hello", 1};
std::cout << str.data() << std::endl; // 输出 hello，而不是h
```

这里预期输出hello，但是却输出了h。

### 2.2 生命周期

```cpp
string_view foo() {
    std::string s{"hello world"};
    return string_view{s};
}
```

在该示例中，s出了作用域，string_view变成了悬空引用。


!!! warning "生命周期"
    `std::string_view` 不拥有底层字符存储，务必保证被引用对象的生命周期覆盖全部使用期。