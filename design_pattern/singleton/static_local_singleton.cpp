//
// Created by light on 20-2-7.
// 在C++11标准下，《Effective C++》提出了一种更优雅的单例模式实现，使用函数内的 local static 对象。
// 这种方法也被称为Meyers' Singleton。
//

#include <iostream>

using namespace std;

class singleton {
private:
    singleton() {}
public:
    static singleton &instance();
};

singleton &singleton::instance() {
    static singleton p;
    return p;
}


