//
// Created by light on 20-1-7.
//

#include <iostream>
#include <array>

int sqr(int n) {
    return n * n;
}

int rand() {
    return std::rand();
}

// constant expression
constexpr int sqr1(int n) {
    return n * n;
}

constexpr int factorial(int n) {
    // n是普通的int 不能使用static_assert
//    static_assert(n>=0); //error
    // 正确方式
    if (n < 0) {
        throw std::invalid_argument(
                "Arg must be non-negative");
    }

    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    int a[sqr(3)];  // ok
    const int n1 = sqr(3);  // ok

    constexpr int n = sqr1(3); // constexpr=constant expression 常量表达式
    std::array<int, n> aa;  // ok

    std::array<int, factorial(10)> b;   // ok
    int cc[rand()];
    return 0;
}