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

    A(const A &) { cout << "Copy A\n"; }

    A(A&&)= delete;
};

A getA_duang() {
    A a1;
    A a2;
    if (rand() > 42) { 
        return a1; 
    }
    else { 
        return a2; 
    }
}

int main() {
    auto a = getA_duang();
}