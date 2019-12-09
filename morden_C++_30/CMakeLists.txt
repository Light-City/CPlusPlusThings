cmake_minimum_required(VERSION 3.14)
project(Morden_C++)

set(CMAKE_CXX_STANDARD 11)

add_executable(heap RAII/heap.cpp)
add_executable(stack RAII/stack.cpp)
add_executable(RAII RAII/RAII.cpp)
add_executable(auto_ptr smart_ptr/auto_ptr.cpp)
