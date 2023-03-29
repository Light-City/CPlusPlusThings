//
// Created by light on 19-11-2.
//

#include <iostream>
using namespace std;

void f(int i) {
   cout<<"void f(int i)" <<endl;
}

void f(void *p) {
    cout<<"void f(void *p)" <<endl;
}
int main() {
    f(0);

    // #ifndef __cplusplus
    // #define NULL ((void *)0)
    // #else   /* C++ */
    // #define NULL 0

    //  c语言中将NULL定义为空指针，而在c++中直接定义为0，这是因为C++是强类型的，void *是不能隐式转换成其他指针类型的。

    if(NULL==0)  cout<<"NULL==0"<<endl;
    // f(NULL); // ambiguous,因为NULL==0所以存在二义性 指针也可以是个int的地址
    // c++11 空指针nullptr
    f(nullptr); //typedef decltype(nullptr)	nullptr_t;
    // nullptr_t为nullptr的类型
    nullptr_t  nl;  // 使用nullptr_t定义的任何变量都具有nullptr一样的行为
    f(nl);

    return 0;
}
