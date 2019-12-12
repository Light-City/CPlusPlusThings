//
// Created by light on 19-12-9.
//

#include <iostream>
using namespace std;

// pImpl: Pointer-to-Implementation

class private_foo;
class foo {
public:
    foo();

    ~foo();

    void bar();

private:
    private_foo *pImpl;
};

class private_foo {
public:
    void bar() {
        cout<<"private_foo invoke bar funciton."<<endl;
    }

private:
    int m1;
    string m2;
};

foo::foo() : pImpl(new private_foo()) {
}

foo::~foo() {
}

void foo::bar() {
    pImpl->bar();
}


int main() {
    foo f;
    f.bar();
}