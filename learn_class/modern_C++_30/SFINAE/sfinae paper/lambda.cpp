//
// Created by light on 20-1-6.
//
#include <iostream>
#include "structData.h"
// Equivalent to:
struct l5UnamedType {
    template<typename T>
    auto operator()(T &t) const -> decltype(t.serialize()) // /!\ This signature is nice for a SFINAE!
    {
        return t.serialize();
    }
};


void fun(A a,B b,C c) {

    // ***** Simple lambda unamed type *****
    auto l4 = [](int a, int b) { return a + b; };
    std::cout << l4(4, 5) << std::endl; // Output 9.

    // Equivalent to:
    struct l4UnamedType {
        int operator()(int a, int b) const {
            return a + b;
        }
    };

    l4UnamedType l4Equivalent = l4UnamedType();
    std::cout << l4Equivalent(4, 5) << std::endl; // Output 9 too.



    // ***** auto parameters lambda unnamed type *****

    // b's type is automagically deduced!
    auto l5 = [](auto &t) -> decltype(t.serialize()) { return t.serialize(); };

    std::cout << l5(b) << std::endl; // Output: I am a B!
//    std::cout << l5(a) << std::endl; // Error: no member named 'serialize' in 'A'.


    l5UnamedType l5Equivalent = l5UnamedType();

    std::cout << l5Equivalent(b) << std::endl; // Output: I am a B!
//    std::cout << l5Equivalent(a) << std::endl; // Error: no member named 'serialize' in 'A'.

}
int main() {
    A a;
    B b;
    C c;
    auto l1 = [](B &b) { return b.serialize(); }; // Return type figured-out by the return statement.
    auto l3 = [](B &b) -> std::string { return b.serialize(); }; // Fixed return type.
    auto l2 = [](B &b) -> decltype(b.serialize()) { return b.serialize(); }; // Return type dependant to the B type.

    std::cout << l1(b) << std::endl; // Output: I am a B!
    std::cout << l2(b) << std::endl; // Output: I am a B!
    std::cout << l3(b) << std::endl; // Output: I am a B!

    fun(a,b,c);
}