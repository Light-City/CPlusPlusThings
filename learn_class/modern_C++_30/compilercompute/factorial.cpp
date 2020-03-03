//
// Created by light on 19-12-28.
//

#include <iostream>

using namespace std;

template<int n>
struct factorial {
    static_assert(n >= 0, "Arg must be non-negative");
    static const int value = n * factorial<n - 1>::value;
};

template<>
struct factorial<0> {
    static const int value = 1;
};

int main() {

    printf("%d\n", factorial<-1>::value);
    return 0;
}