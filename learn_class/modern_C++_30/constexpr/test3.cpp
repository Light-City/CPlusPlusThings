//
// Created by light on 20-1-10.
//

#include <iostream>

class test3 {
public:

    int value;

    // constexpr const method - can't chanage the values of object fields and can be evaluated at compile time.
    constexpr int getvalue() const {
        return (value);
    }

    constexpr test3(int Value)
            : value(Value) {
    }
};


int main() {

    // 加不加都行
    constexpr test3 x(100); // OK. Constructor is constexpr.

    int array[x.getvalue()]; // OK. x.getvalue() is constexpr and can be evaluated at compile time.
}


