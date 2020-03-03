//
// Created by light on 20-1-6.
//
#include <iostream>

constexpr int factorial(int n) {
    return n <= 1 ? 1 : (n * factorial(n - 1));
}

struct testStruct : std::true_type {
}; // Inherit from the true type.

int main() {
    int i = factorial(5); // Call to a constexpr function.
    // Will be replace by a good compiler by:
    // int i = 120;
    std::cout << i << std::endl;


    constexpr bool testVar = testStruct(); // Generate a compile-time testStruct.
    bool test = testStruct::value; // Equivalent to: test = true;
    std::cout << test << std::endl;
    test = testVar; // true_type has a constexpr converter operator, equivalent to: test = true;
    std::cout << test << std::endl;
}
