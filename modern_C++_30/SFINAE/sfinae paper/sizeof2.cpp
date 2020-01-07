#include <iostream>

//
// Created by light on 20-1-6.
//

typedef char yes; // Size: 1 byte.
typedef yes no[2]; // Size: 2 bytes.

// Two functions using our type with different size.
yes &f1() {}
no &f2() {}

int main() {
    std::cout << sizeof(f1()) << std::endl;
    std::cout << sizeof(f2()) << std::endl;
    std::cout << (sizeof(f1()) == sizeof(f2())) << std::endl; // Output 0.
}