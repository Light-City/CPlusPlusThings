//
// Created by light on 20-1-7.
//

#include <iostream>
#include <vector>
#include <array>

using namespace std;

// C++17 内联变量
struct magic {
//    static const int number = 42;  // error
    static constexpr int number = 42; // ok
// 类的静态constexpr成员变量默认就是内联的。
//    static inline const int number = 42; // ok
//  const常量和类外面的constexpr变量是不默认内联，需要手工加inline关键字才会变成内联
};

// 内联变量之前用这个方法,称为one definition rule，有了内联变量及模板，则不受这条规则限制!
// const int magic::number = 42;  // ok


// C++14 变量模板
// 之前我们需要用类静态数据成员来表达的东西，使用变量模板可以更简洁地表达。
namespace my {
    template<class T>
    inline constexpr bool
            is_trivially_destructible_v =
            std::is_trivially_destructible<
                    T>::value;
}
// 不重要析构
class A {
};

// 重要析构
class B {
    ~B() {}
};

int main() {
    std::cout << magic::number << std::endl;

    std::vector<int> v;
    v.push_back(magic::number); // undefined reference to `magic::number'
    std::cout << v[0] << std::endl;
    // 不重要的析构
    std::cout << is_trivially_destructible_v<A> << std::endl;
    std::cout << is_trivially_destructible_v<B> << std::endl;

    // constexpr变量仍是const

    constexpr int a = 42;
    const int &b = a;
    // 上述const不能去掉,否则报如下错误：
    // binding reference of type ‘int&’ to ‘const int’ discards qualifiers


    // constexpr 构造函数和字面类型
    // C++14 放开了 constexpr函数里的循环
    // C++20 放开了 try...catch语句 和 asm声明
    // constexpr函数里不能使用goto语句

    // 一个有意思的情况是一个类的构造函数。如果一个类的构造函数里面只包含常量表达式、
    // 满足对 constexpr 函数的限制的话（这也意味着，里面不可以有任何动态内存分配），
    // 并且类的析构函数是平凡的，那这个类就可以被称为是一个字面类型。
    std::array<int, 5> ay;


    constexpr string_view sv{"hi"};
    constexpr pair pr{sv[0], sv[1]};
    constexpr array aa{pr.first, pr.second};
    constexpr int n1 = aa[0];
    constexpr int n2 = aa[1];
    cout << n1 << ' ' << n2 << '\n';

}