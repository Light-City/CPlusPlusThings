# 跨平台代码示例

本目录包含跨平台C++代码示例，展示如何使用条件编译和跨平台库编写可在Windows、Linux和macOS上运行的代码。

## 跨平台开发简介

跨平台开发是指编写可在多个操作系统上运行的代码。为了实现这一点，开发人员需要使用跨平台库和条件编译来处理不同操作系统之间的差异。

### 条件编译

条件编译是一种在编译时根据预定义的宏来选择不同代码路径的技术。例如，可以使用以下预定义宏来检测操作系统：

- `_WIN32`：Windows
- `__linux__`：Linux
- `__APPLE__`：macOS

### 跨平台库

跨平台库是一种提供统一接口的库，可以在多个操作系统上运行。例如，Boost库和Qt库都是跨平台库。

## 目录内容

1. **file_system.cpp** - 跨平台文件系统操作示例
2. **thread_example.cpp** - 跨平台线程操作示例
3. **networking.cpp** - 跨平台网络操作示例

## 构建和运行

要构建和运行这些示例，您需要一个C++编译器（例如GCC、Clang、MSVC）。

### Windows

使用MSVC构建：

```bash
cl /std:c++17 file_system.cpp
file_system.exe
```

### Linux

使用GCC构建：

```bash
g++ -std=c++17 file_system.cpp -o file_system
./file_system
```

### macOS

使用Clang构建：

```bash
clang++ -std=c++17 file_system.cpp -o file_system
./file_system
```

## 跨平台开发最佳实践

1. **使用跨平台库**：优先使用跨平台库，而不是直接调用操作系统API。
2. **使用条件编译**：使用条件编译来处理不同操作系统之间的差异。
3. **避免使用平台特定的API**：尽量避免使用平台特定的API，除非您确实需要。
4. **测试多个平台**：确保您的代码在所有目标平台上都能正常运行。
5. **使用CMake**：使用CMake来管理跨平台构建。