//
// Created by light on 20-1-6.
//

#include <iostream>
#include "structData.h"

template<typename UnnamedType>
struct container {
// Let's put the test in private.
private:
    // We use std::declval to 'recreate' an object of 'UnnamedType'.
    // We use std::declval to also 'recreate' an object of type 'Param'.
    // We can use both of these recreated objects to test the validity!
    template<typename Param>
    constexpr auto testValidity(int /* unused */)
    -> decltype(std::declval<UnnamedType>()(std::declval<Param>()), std::true_type()) {
        // If substitution didn't fail, we can return a true_type.
        return std::true_type();
    }

    template<typename Param>
    constexpr std::false_type testValidity(...) {
        // Our sink-hole returns a false_type.
        return std::false_type();
    }

public:
    // A public operator() that accept the argument we wish to test onto the UnnamedType.
    // Notice that the return type is automatic!
    template<typename Param>
    constexpr auto operator()(const Param &p) {
        // The argument is forwarded to one of the two overloads.
        // The SFINAE on the 'true_type' will come into play to dispatch.
        // Once again, we use the int for the precedence.
        return testValidity<Param>(int());
    }
};

template<typename UnnamedType>
constexpr auto is_valid(const UnnamedType &t) {
    // We used auto for the return type: it will be deduced here.
    return container<UnnamedType>();
}

// Check if a type has a serialize method.
auto hasSerialize = is_valid([](auto &&x) -> decltype(x.serialize()) {});

// Notice how I simply swapped the return type on the right?
template<class T>
auto serialize(T &obj)
-> typename std::enable_if<decltype(hasSerialize(obj))::value, std::string>::type {
    return obj.serialize();
}

template<class T>
auto serialize(T &obj)
-> typename std::enable_if<!decltype(hasSerialize(obj))::value, std::string>::type {
    return to_string(obj);
}

int main() {
    A a;
    B b;
    C c;
    auto test = is_valid([](const auto &t) -> decltype(t.serialize()) {});

    std::cout << test(a) << std::endl;
    std::cout << test(b) << std::endl;
    std::cout << test(c) << std::endl;


    // The following lines work like a charm!
    std::cout << serialize(a) << std::endl;
    std::cout << serialize(b) << std::endl;
    std::cout << serialize(c) << std::endl;

}
