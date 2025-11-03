# C++23 Features Demo

This directory contains examples of new features introduced in C++23.

## C++23 New Features

C++23 is the latest standard of the C++ programming language, released in 2023. It introduces several new features and improvements, including:

1. **std::expected** - A type for representing either a value or an error
2. **std::mdspan** - A multi-dimensional span
3. **Module system enhancements** - Improved support for modules
4. **std::format** improvements - More formatting options
5. **std::ranges improvements** - New range adapters and algorithms
6. **Coroutine improvements** - Better support for coroutines
7. **And more...**

## Contents

1. **std_expected.cpp** - Example of using std::expected for error handling
2. **std_mdspan.cpp** - Example of using std::mdspan for multi-dimensional arrays
3. **modules_demo.cpp** - Example of using C++23 modules
4. **format_improvements.cpp** - Example of C++23 format improvements
5. **ranges_improvements.cpp** - Example of C++23 ranges improvements

## Build and Run

To build and run these examples, you need a C++23 compliant compiler (e.g., GCC 13+, Clang 16+, MSVC 2022+).

```bash
# Build with GCC
g++ -std=c++23 std_expected.cpp -o std_expected
./std_expected

# Build with Clang
clang++ -std=c++23 std_expected.cpp -o std_expected
./std_expected

# Build with MSVC
cl /std:c++23 std_expected.cpp
std_expected.exe
```