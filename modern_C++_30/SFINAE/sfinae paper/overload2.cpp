//
// Created by light on 20-1-6.
//
#include <iostream>

std::string f(...) // Variadic functions are so "untyped" that...
{
    return "...";
}

template<typename T>
std::string f(const T &t)// ...this templated function got the precedence!
{

    return "T";
}

int main() {
    std::cout << f<int>(1) << std::endl;
}