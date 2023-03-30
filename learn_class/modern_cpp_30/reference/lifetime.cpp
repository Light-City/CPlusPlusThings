//
// Created by light on 19-12-15.
//

#include <iostream>
#include "shape.h"

using namespace std;

class result {
public:
    result() { puts("result()"); }

    ~result() { puts("~result()"); }
};

result process_shape(const shape &shape1, const shape &shape2) {
    puts("process_shape()");
    return result();
}


class Base {
public:
    Base() {
        cout << "Base()" << endl;
    }

    ~Base() {
        cout << "~Base()" << endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        cout << "Derived()" << endl;
    }

    ~Derived() {
        cout << "~Derived()" << endl;
    }
};

string f() { return "abc"; }

void g() {
    const string &s = f();       // still legal?
    cout << s << endl;
}

Derived factory() {
    return Derived();
}

int main() {
    process_shape(circle(), triangle());
    cout << endl;
    // 临时对象延迟
//    result &&r = process_shape(circle(), triangle());
    // 临时对象延迟只对rvalue有用，而对xvalue无用！
//    result &&r = std::move(process_shape(circle(), triangle()));

//   const Base &b1 = factory();

    Base *b1 = new Derived;
    delete b1;
    cout<<endl;
    Derived d;
    Base &b2 =d;
}