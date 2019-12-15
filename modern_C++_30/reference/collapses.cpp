//
// Created by light on 19-12-15.
//
#include <iostream>

using namespace std;

template<typename T>
void f(T &&param) {
    static_assert(std::is_lvalue_reference<T>::value, "T& is lvalue reference");
    cout << "T& is lvalue reference" << endl;
}

template<typename T>
class Widget {
    typedef T& LvalueRefType;
    typedef T&& RvalueRefType;
public:
    void judge() {
        static_assert(std::is_lvalue_reference<LvalueRefType>::value, "LvalueRefType & is lvalue reference");
        static_assert(std::is_lvalue_reference<RvalueRefType>::value, "RvalueRefType & is lvalue reference");
        cout << "LvalueRefType and RvalueRefType is lvalue reference" << endl;
    }
    void f(LvalueRefType&& param) {

    }
};


int main() {
    int x;
    int &&r1 = 10;
    int &r2 = x;
    f(r1);
    f(r2);

    Widget<int &> w;
    w.judge();


    Widget<int> w1, w2;

    auto&& v1 = w1;                  // v1 is an auto-based universal reference being
    // initialized with an lvalue, so v1 becomes an
    // lvalue reference referring to w1.

    // 不能编译
//    decltype(w1)&& v2 = w2;          // v2 is a decltype-based universal reference, and
    // decltype(w1) is Widget, so v2 becomes an rvalue reference.
    // w2 is an lvalue, and it’s not legal to initialize an
    // rvalue reference with an lvalue, so
    // this code does not compile.
}
