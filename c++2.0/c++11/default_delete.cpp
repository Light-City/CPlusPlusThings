//
// Created by light on 19-11-3.
//

#include <iostream>
#include <complex>

using namespace std;

class Zoo {

public:
    Zoo(int a, int b) : a(a), b(b) {
        cout << "with param ctor \n";
    }

    Zoo() = default;

    Zoo(const Zoo &) = delete;  // copy ctor

    Zoo(Zoo &&) = default;      // move ctor

    Zoo &operator=(const Zoo &) = default;  // copy assignment

    Zoo &operator=(const Zoo &&) = delete;  // move assignment

    // 一般函数可以使用=default或=delete?
//    void fun1()= default; // error: ‘void Zoo::fun1()’ cannot be defaulted
    void fun1() = delete; // compile ok
    // 析构函数可以使用=default或=delete?
//    ~Zoo()= delete;   // ok,但是造成使用Zoo object出错
    ~Zoo() = default;

    // 上述得出=delete可以使用在任何函数身上,而=default不可以使用在普通函数上
    // 上述会联想到=0,=0只能用在virtual funciton上

private:
    int a, b;
};

class Empty {
};

// 等价于  编译器给出的默认版本函数都是public且inline
class Empty1 {
public:
    Empty1() {}

    Empty1(const Empty1 &rhs) {}

    ~Empty1() {}
};

// no-copy
// copy ctor与copy assignment都delete掉
struct NoCopy {
    NoCopy() = default;

    NoCopy(const NoCopy &) = delete;

    NoCopy &operator=(const NoCopy &) = delete;

    ~NoCopy() = default;
};

// no-dtor
struct NoDtor {
    NoDtor() = default;

    ~NoDtor() = delete;
};

// 对no-copy的改进 将copy ctor与copy assignment 放到private ,但是可以对member与friend调用
// boost::noncopyable 就用到了PrivateCopy
class PrivateCopy {
private:
    PrivateCopy(const PrivateCopy &) {};

    PrivateCopy &operator=(const PrivateCopy &) {};

public:
    PrivateCopy() = default;

    ~PrivateCopy() {};
};

// 继承了的都拥有PrivateCopy性质
class Foo : public PrivateCopy {

};

int main() {
    Zoo z;
    Zoo z1(1, 2);
//    Zoo z2=z;   // error copy ctor delete
    z = z1;
    complex<int> a;     // 内部不带指针的可以不用写 big three 没有析构 有copy ctor与copy assignmetn 都是使用=default,没有自己写
    string s;       // 内部带指针的有big five

//    NoDtor n;  //error no-dtor 不能够自动delete
//    NoDtor *p=new NoDtor;   // ok
//    delete p;   // error no-dtor 不能够delete
    Foo foo;
    Foo foo1;
//    foo = foo1;   // 继承了的都拥有PrivateCopy性质
    return 0;
}