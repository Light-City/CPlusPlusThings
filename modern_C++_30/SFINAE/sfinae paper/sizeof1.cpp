//
// Created by light on 20-1-6.
//

#include <iostream>

typedef char type_test[42];

type_test &f() {}

int main() {

    // In the following lines f won't even be truly called but we can still access to the size of its return type.
    // Thanks to the "fake evaluation" of the sizeof operator.
    char arrayTest[sizeof(f())];
    std::cout << sizeof(f()) << std::endl; // Output 42.
}