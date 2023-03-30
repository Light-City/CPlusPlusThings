//
// Created by light on 19-9-11.
//

#include <iostream>

using namespace std;
struct A {
#define p "hello"
};

class C {
public:
    // static const 可以直接在类内部初始化
    // no-const static 只能在外面初始化
    static const int NUM = 3; //声明
    enum con {
        NUM1 = 3
    };
};

#define MAX(a,b) ((a) > (b) ? (a) : (b))
template<typename T>
inline int Max(const T& a, const T& b){
    return (a>b ? a:b);
}
const int C::NUM;   // 定义
int main() {
    cout << p << endl; // macro is global
    C c;
    cout << &c.NUM << endl;     // 未定义的引用,需要定义

    cout << C::NUM1 << endl;
//    cout << &C:NUM1 << endl;  //error enum no address

    int a=5, b=0;
    cout<<MAX(++a, b)<<endl;              // a被增加两次
    cout<<MAX(++a, b+10)<<endl;           // a被累加一次
    a=5,b=0;
    cout<<Max(++a,b)<<endl;
}


/**
const 有地址,enum与#define没有地址
 1.const 定义的实际是一个变量,const只限定它不能被修改,所有变量都可在程序运行时获取其地址
 2.enum类型中的枚举项只是enum类型声明的一部分,它不是定义出来的变量,所以不能取地址
 3.#define出来的是宏,它是预处理的东西,预处理后的编译阶段已经不存在,所以也不可能获取宏的地址

 */