> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-string-abi-compatibility/) 的文章，由站点同步脚本导出。

# C++ string ABI 兼容性

### 项目实战问题：避坑 C++ String ABI 兼容性问题

[TOC]

为什么很多人不喜欢C++ string，反而要自己造轮子呢？其中一个问题就是C++ string abi问题，你遇到过吗？

之前使用conan与跨平台库的时候发生了C++ string abi兼容问题，本节将会引入ABI的背景与编译器相关的内容，讲讲如何避免这个坑，如果你遇到过，欢迎留言～

#### 1. ABI 变更的背景

**GCC 5.1**之后加强了对 C++11 的支持，并引入了一些新特性。在这一版本中，libstdc++ 引入了新的库 ABI，更新了 `std::string` 和 `std::list` 的实现。这些变化符合 C++11 标准，后者禁止使用 Copy-On-Write 字符串，并要求链表能够跟踪其大小。

实际项目中在使用 Conan 时，开发者可以通过调整 `compiler.libcxx` 来选择使用的 ABI：

- **libstdc++**：使用旧 ABI。
- **libstdc++11**：使用新 ABI。

选择适当的 ABI 可以确保与其他依赖库的兼容性，特别是在使用不同版本的编译器或库时。

#### 2. 编译器的默认设置

在使用 GCC 进行编译时，如果没有显式地指定 `--with-default-libstdcxx-abi=gcc4-compatible`，则 GCC 默认使用新的 C++11 ABI。这意味着：

- 如果你的代码或依赖库是在旧 ABI 下编译的，那么当你尝试链接时，可能会遇到未定义的符号错误。

例如，编译以下代码时：

```cpp
// lib.cc
#include <string>
void foo(const std::string& s) {}
```

使用命令：

```bash
g++-14 -D_GLIBCXX_USE_CXX11_ABI=0 -c lib.cc
```

将生成符号 `__Z3fooRKSs`，这是旧 ABI 的格式。

然而，当你编译并链接另一个使用新 ABI 的文件时，可能会出现如下错误：

```bash
g++-14 main.cc lib.o
```

```
Undefined symbols for architecture x86_64:
  "foo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)", referenced from:
      _main in ccbWGoYh.o
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
```

#### 3. 解决方案

为了确保编译通过，有以下两种解决方案：

**方法 1: 手动指定 ABI**

在编译所有源文件时，确保手动添加 `-D_GLIBCXX_USE_CXX11_ABI=0` 来兼容旧的库，例如：

```bash
g++-14 -D_GLIBCXX_USE_CXX11_ABI=0 main.cc lib.o
```

**方法 2: 使用新的 ABI 编译所有文件**

如果你愿意使用新的 C++11 ABI，可以在编译库文件时使用 `-D_GLIBCXX_USE_CXX11_ABI=1`，这样两者符号就会匹配：

```bash
g++-14 -D_GLIBCXX_USE_CXX11_ABI=1 -c lib.cc
g++-14 main.cc lib.o
```

此时，链接时符号将会匹配，不再出现未定义符号的错误。

#### 4. 符号比较

通过 `nm` 命令查看不同 ABI 下的符号，可以直观地看到二者的差异：

```bash
g++-14 -D_GLIBCXX_USE_CXX11_ABI=0 -c lib.cc
nm lib.o | grep foo  
# 输出: 0000000000000000 T __Z3fooRKSs

g++-14 -D_GLIBCXX_USE_CXX11_ABI=1 -c lib.cc
nm lib.o | grep foo  
# 输出: 0000000000000000 T __Z3fooRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
```

#### 5. 小结

ABI是C++升级以及编译过程中遇到的一个非常大的坑，希望可以通过这篇文章讲明白这些内容。