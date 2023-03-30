//
// Created by light on 19-11-3.
//

#include <iostream>
#include <vector>

using namespace std;

// 第一种使用
// type alias
// 等价于typedef void(*func)(int, int);
using fun=void (*)(int, int);

void example(int, int) {}

// 第二种使用
template<typename T>
struct Container {
    using value_type=T;  // 等价于typedef T value_type;
};

template<typename Cn>
void func2(const Cn &cn) {
    typename Cn::value_type n;
}

// 第三种使用
// alias template
template<typename CharT>
using string=basic_string<CharT, char_traits<CharT>>;
// 等价于 typedef basic_string<char>    string;

// 综上:type alias等价于typedef,没有什么不同
// using 的所有用法拓展如下:
// 1.using-directives
// using namespace std; or using std::cout;
// 2. using-declaration
// protected:
//    using _Base::_M_alloc;
// 3.c++2.0特性 type alias and alias template
int main() {
    fun fn = example;   // 函数指针
    return 0;
}
