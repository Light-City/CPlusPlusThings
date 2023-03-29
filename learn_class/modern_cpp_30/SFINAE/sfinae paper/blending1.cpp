//
// Created by light on 20-1-6.
//

#include <iostream>
#include "structData.h"


template<class T>
struct hasSerialize {
    // We test if the type has serialize using decltype and declval.
    // decltype将根据逗号表达式选择最后一个类型作为返回值
    // 如果C有serialize将调用当前版本,没有serialize,则失败,此时编译时不报错,继续寻找重载函数,也就是SFINAE.
    template<typename C>
    static constexpr decltype(std::declval<C>().serialize(), bool()) test(int /* unused */) {
        // We can return values, thanks to constexpr instead of playing with sizeof.
        return true;
    }

    template<typename C>
    static constexpr bool test(...) {
        return false;
    }

    // int is used to give the precedence!
    static constexpr bool value = test<T>(int());
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
    A a;
    B b;
    C c;

    // The following lines work like a charm!
    std::cout << serialize(a) << std::endl;
    std::cout << serialize(b) << std::endl;
    std::cout << serialize(c) << std::endl;

}