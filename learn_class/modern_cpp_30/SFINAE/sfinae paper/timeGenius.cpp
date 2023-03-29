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


template<bool B, class T = void> // Default template version.
struct enable_if {
}; // This struct doesn't define "type" and the substitution will fail if you try to access it.

template<class T> // A specialisation used if the expression is true.
struct enable_if<true, T> {
    typedef T type;
}; // This struct do have a "type" and won't fail on access.

template<class T>
typename enable_if<hasSerialize<T>::value, std::string>::type serialize(const T &obj) {
    return obj.serialize();
}

template<class T>
typename enable_if<!hasSerialize<T>::value, std::string>::type serialize(const T &obj) {
    return to_string(obj);
}

int main() {

    // Usage:
//    enable_if<true, int>::type t1; // Compiler happy. t's type is int.
//    enable_if<hasSerialize<B>::value, int>::type t2; // Compiler happy. t's type is int.
//
//    enable_if<false, int>::type t3; // Compiler unhappy. no type named 'type' in 'enable_if<false, int>';
//    enable_if<hasSerialize<A>::value, int>::type t4; // no type named 'type' in 'enable_if<false, int>';


    A a;
    B b;
    C c;

    // The following lines work like a charm!
    std::cout << serialize(a) << std::endl;
    std::cout << serialize(b) << std::endl;
    std::cout << serialize(c) << std::endl;

}