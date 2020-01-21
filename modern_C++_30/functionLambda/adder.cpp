//
// Created by light on 20-1-11.
//
#include <iostream>

using namespace std;

struct adder {
    adder(int n) : n_(n) {}

    int operator()(int x) const {
        return x + n_;
    }

private:
    int n_;
};

int main() {
    auto add_2 = adder(2);

    // x+2
    cout << add_2(3) << endl;

    auto t = bind1st(plus<int>(), 2);
    cout << t(1) << endl;
    // 上述的C++98
    binder2nd<plus<int> > a2(plus<int>(), 2);
    cout << a2(3) << endl;

    cout << [](int x) { return x * x; }(3) << endl;
    return 0;
    // lambda表达式默认就是constexpr函数
}