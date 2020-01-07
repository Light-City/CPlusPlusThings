//
// Created by light on 20-1-6.
//

#include <iostream>
#include "structData.h"


template<class T>
struct hasSerialize {
    // 编译时比较
    typedef char yes[1];
    typedef char no[2];
    // 允许我们检查序列化确实是一种方法
    // 第二个参数必须是第一个参数的类型
    // 例如： reallyHas<int,10> 替换为 reallyHas<int,int 10> 并起作用
    // 注意：它仅适用于整数常量和指针(因此函数指针可以使用)
    // 例如：reallyHas<std::string (C::*)(), &C::serialize> 替换为
    // reallyHas<std::string (C::*)(), std::string (C::*)() &C::serialize> 并起作用
    template<typename U, U u>
    struct reallyHas;

    // std::string (C::*)() 是函数指针声明
    template<typename C>
    static yes &test(reallyHas<std::string (C::*)(), &C::serialize> * /*unused*/) {}

    //  ()()const 函数指针 -> std::string serialize() const
    template<typename C>
    static yes &test(reallyHas<std::string (C::*)() const, &C::serialize> * /*unused*/) {}

    // The famous C++ sink-hole.
    // Note that sink-hole must be templated too as we are testing test<T>(0).
    // If the method serialize isn't available, we will end up in this method.
    template<typename>
    static no &test(...) { /* dark matter */ }

    //用作测试的返回值的常数。
    //由于编译时评估的大小，因此实际上在这里完成了测试。
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

// Using the previous A struct and hasSerialize helper.
struct D : A {
    std::string serialize() const {
        return "I am a D!";
    }
};

template<class T>
bool testHasSerialize(const T & /*t*/) { return hasSerialize<T>::value; }

// 不能够判断仿函数里面的serialize
struct E {
    struct Functor {
        std::string operator()() {
            return "I am a E!";
        }
    };

    Functor serialize;
};

template<class T>
std::string serialize(const T &obj) {
    // 不加constexpr 报错：error: no member named 'serialize' in 'A'.
    if constexpr (hasSerialize<T>::value)
        return obj.serialize();
    else
        return to_string(obj);
}

int main() {
    // 检测结构体是否有serialize方法
    // Using the struct A, B, C defined in the previous hasSerialize example.
    std::cout << hasSerialize<A>::value << std::endl;
    std::cout << hasSerialize<B>::value << std::endl;
    std::cout << hasSerialize<C>::value << std::endl;


    D d;
    A &a = d; // Here we lost the type of d at compile time.
    std::cout << testHasSerialize(d) << std::endl; // Output 1.
    std::cout << testHasSerialize(a) << std::endl; // Output 0.

    E e;
    std::cout << e.serialize() << std::endl; // Succefully call the functor.
    std::cout << testHasSerialize(e) << std::endl; // Output 0.


    A a_;
    B b_;
    C c_;
    std::cout << serialize(a_) << std::endl;
    std::cout << serialize(b_) << std::endl;
    std::cout << serialize(c_) << std::endl;
}