//
// Created by light on 19-11-3.
//
#include <iostream>
#include <list>

using namespace std;
// template template parameter 模板模板参数
template<typename T,
        template<typename U> class Container>
class XCls {
private:
    Container<T> c;
};

// alias template
template<typename T>
using Lst = list<T>;

int main() {
//    XCls<string, list> mylist1;   // error
    XCls<string, Lst> mylist2;

    return 0;
}
