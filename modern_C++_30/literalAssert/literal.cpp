//
// Created by light on 19-12-25.
//

#include <chrono>

#include <complex>
#include <iostream>
#include <string>
#include <thread>
#include <bitset>

using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;
using namespace std::literals::complex_literals;


struct length {
    double value;
    enum unit {
        metre,
        kilometre,
        millimetre,
        centimetre,
        inch,
        foot,
        yard,
        mile,
    };
    static constexpr double factors[] =
            {1.0, 1000.0, 1e-3,
             1e-2, 0.0254, 0.3048,
             0.9144, 1609.344};

    explicit length(double v,
                    unit u = metre) {
        value = v * factors[u];
    }


};

length operator+(length lhs,
                 length rhs) {
    return length(lhs.value +
                  rhs.value);
}

length operator "" _m(long double v) { return length(v, length::metre); }

length operator "" _cm(long double v) { return length(v, length::centimetre); }

//  可能有其他运算符
int main() {

    std::cout << "i * i = " << 1i * 1i << std::endl;
    std::cout << "Waiting for 500ms" << std::endl;
    std::this_thread::sleep_for(500ms);
    std::cout << "Hello world"s.substr(0, 5) << std::endl;

    length l1 = length(1.0, length::metre);
    length l2 = length(1.0, length::centimetre);
    std::cout << l2.value << std::endl;
    std::cout << (l1 + l2).value << std::endl;

    // 1.0_m + 10.0_cm
    std::cout << (1.0_m + 1.0_cm).value << std::endl;

    // 二进制字面量
    unsigned mask = 0b1101;
    // 以十进制打印
    std::cout << mask << std::endl;

    // 打印二进制字面量
    std::cout << std::bitset<4>(mask) << std::endl;

    // 数字分隔符
    unsigned mk = 0b111'000'000;
    double pi = 3.141'5926;
    return 0;
}
