# Modern C++ Coroutine

This directory contains examples and implementations of C++20 coroutine features.

## What is Coroutine?

Coroutines are a new feature in C++20 that allow functions to be suspended and resumed later, providing a more natural way to write asynchronous code.

## Contents

1. **basic_coroutine.cpp** - Basic coroutine examples
2. **task_scheduler.cpp** - Task scheduling with coroutines
3. **async_io.cpp** - Asynchronous IO using coroutines

## Build and Run

To build and run these examples, you need a C++20 compliant compiler (e.g., GCC 10+, Clang 10+, MSVC 2019+).

```bash
# Build with GCC
g++ -std=c++20 basic_coroutine.cpp -o basic_coroutine
./basic_coroutine

# Build with Clang
clang++ -std=c++20 basic_coroutine.cpp -o basic_coroutine
./basic_coroutine

# Build with MSVC
cl /std:c++20 basic_coroutine.cpp
basic_coroutine.exe
```