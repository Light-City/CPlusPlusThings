//
// Created by light on 20-1-6.
//
#include <iostream>

void f(std::string s); // int can't be convert into a string.
void f(double d); // int can be implicitly convert into a double, so this version could be selected, but...
void f(int i); // ... this version using the type int directly is even more close!



int main() {
    f(1); // Call f(int i);
}
