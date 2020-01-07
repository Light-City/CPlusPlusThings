//
// Created by light on 20-1-6.
//
#include <iostream>

bool f();

auto test = f(); // Famous usage, auto deduced that test is a boolean, hurray!



//                             vvv t wasn't declare at that point, it will be after as a parameter!
template<typename T>
decltype(t.serialize()) g(const T &t) {} // Compilation error

// Less famous usage:
//                    vvv auto delayed the return type specification!
//                    vvv                vvv the return type is specified here and use t!
template<typename T>
auto g(const T &t) -> decltype(t.serialize()) {} // No compilation error.


auto myFunction() // Automagically figures out that myFunction returns ints.
{
    return int();
}