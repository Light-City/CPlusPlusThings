//
// Created by light on 20-1-6.
//

#include <boost/hana.hpp>
#include <iostream>
#include <string>
#include "structData.h"
using namespace std;

namespace hana = boost::hana;
// 检查类型是否有一个serialize方法
auto hasSerialize = hana::is_valid([](auto &&x) -> decltype(x.serialize()) {});

// 序列化任意对象
template<typename T>
std::string serialize(T const &obj) {
    return hana::if_(hasSerialize(obj),
                     [](auto &x) { return x.serialize(); },
                     [](auto &x) { return to_string(x); }
    )(obj);
}



int main() {
    A a;
    B b;
    C c;
    std::cout << serialize(a) << std::endl;
    std::cout << serialize(b) << std::endl;
    std::cout << serialize(c) << std::endl;
}


