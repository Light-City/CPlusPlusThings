//
// Created by light on 20-1-6.
//

#include <iostream>
#include "structData.h"


template<typename T, typename=std::string>
struct hasSerialize : std::false_type {

};
template<typename T>
struct hasSerialize<T, decltype(std::declval<T>().serialize())> : std::true_type {

};

template<class T>
std::string serialize(const T &obj) {
    // 不加constexpr 报错：error: no member named 'serialize' in 'A'.
    if constexpr (hasSerialize<T>::value)   // c++17特性
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