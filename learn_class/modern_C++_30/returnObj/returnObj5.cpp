//
// Created by light on 19-12-22.
//


#include <iostream>

using namespace std;

// Can copy and move
class A {
public:
    A() { cout << "Create A\n"; }

    ~A() { cout << "Destroy A\n"; }

//    A(const A &) { cout << "Copy A\n"; }
//
//    A(A &&) { cout << "Move A\n"; }
    
    A(const A&&)= delete;
    A(A&&)= delete;
};

A getA_unnamed() {
    return A();
}

int main() {
    auto a = getA_unnamed();
}