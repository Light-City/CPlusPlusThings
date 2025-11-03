# STL Performance Test

This directory contains performance tests for STL containers and algorithms.

## Introduction

The Standard Template Library (STL) is a powerful set of C++ template classes that provide general-purpose classes and functions with templates that implement many popular and commonly used algorithms and data structures.

However, the performance of different STL containers and algorithms can vary significantly depending on the use case. This directory contains performance tests to help you choose the right container or algorithm for your specific needs.

## Contents

1. **vector_vs_list.cpp** - Performance comparison between vector and list
2. **sort_algorithms.cpp** - Performance comparison between different sort algorithms
3. **find_algorithms.cpp** - Performance comparison between different find algorithms
4. **map_vs_unordered_map.cpp** - Performance comparison between map and unordered_map
5. **string_performance.cpp** - Performance tests for string operations

## Build and Run

To build and run these examples, you need a C++ compiler (e.g., GCC, Clang, MSVC).

```bash
# Build with GCC
g++ -std=c++17 vector_vs_list.cpp -o vector_vs_list
./vector_vs_list

# Build with Clang
clang++ -std=c++17 vector_vs_list.cpp -o vector_vs_list
./vector_vs_list

# Build with MSVC
cl /std:c++17 vector_vs_list.cpp
vector_vs_list.exe
```