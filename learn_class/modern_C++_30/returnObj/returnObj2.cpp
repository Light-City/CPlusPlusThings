//
// Created by light on 19-12-22.
//

// nrvo example

#include <iostream>

using namespace std;

// Can copy and move
class A {
public:
    A() { cout << "Create A\n"; }

    ~A() { cout << "Destroy A\n"; }

    A(const A &) { cout << "Copy A\n"; }

    A(A &&) { cout << "Move A\n"; }


    A& operator=(const A&a)
    {
        std::cout << "copy assignment" << std::endl;
        return *this;
    }

    A& operator=(A &&a) {
        cout << "move assignment\n";
        return *this;
    }
};

// 编译器可以优化 返回值移动出去
A getA_named() {
    A a;
    return a;
}

int main() {
//    cout<<"拷贝"<<endl;
//    auto a = getA_named();

    cout<<"赋值"<<endl;
    A aa;
    aa=getA_named();
}