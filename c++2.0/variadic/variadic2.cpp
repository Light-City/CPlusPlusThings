//
// Created by light on 19-11-4.
//

#include <iostream>

using namespace std;

void print(const char *s) {
    while (*s) {
        if (*s == '%' && *(++s) != '%')
            throw std::runtime_error("invalid format string: missing arguments");
        std::cout << *s++;
    }
}

template<typename T, typename... Args>
void print(const char *s, T value, Args... args) {
    while (*s) {
        if (*s == '%' && *(++s) != '%') {
            std::cout << value;
            print(++s, args...); // 即便当 *s == 0 也会产生调用，以检测更多的类型参数。
            return;
        }
        std::cout << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
}

int main() {
    int *pi = new int;
    print("%d %s %p %f\n", 15, "Acc", pi, 3.14);
    return 0;
}