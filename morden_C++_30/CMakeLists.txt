cmake_minimum_required(VERSION 3.14)
project(Morden_C++)

set(CMAKE_CXX_STANDARD 11)

add_executable(heap_stack_RAII RAII/heap_stack.cpp)
add_executable(RAII RAII/RAII.cpp)