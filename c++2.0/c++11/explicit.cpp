//
// Created by light on 19-11-3.
//

#include <iostream>

using namespace std;

// non-explicit-one-argument ctor
class Fraction {
public:
    // non-explicit-one-argument ctor 可以把别的东西转换为Fraction这种
    Fraction(int num, int den = 1) : m_numerator(num), m_denominator(den) {}

    Fraction operator+(const Fraction &f) {
        return Fraction(this->m_numerator + f.m_numerator);
    }


    void getNumAndDen() {
        cout << m_numerator << " " << m_denominator << endl;
    }

private:
    int m_numerator;    // 分子
    int m_denominator;  // 分母
};

// explicit-one-argument ctor
class Fraction1 {
public:
    // explicit-one-argument ctor 可以把别的东西转换为Fraction这种
    explicit Fraction1(int num, int den = 1) : m_numerator(num), m_denominator(den) {}

    Fraction1 operator+(const Fraction1 &f) {
        return Fraction1(this->m_numerator + f.m_numerator);
    }


    void getNumAndDen() {
        cout << m_numerator << " " << m_denominator << endl;
    }

private:
    int m_numerator;    // 分子
    int m_denominator;  // 分母
};

class P {
public:
    P(int a, int b) {
        cout << "P(int a,int b)\n";
    }

    P(int a, int b, int c) {
        cout << "non-explicit P(int a,int b,int c)\n";
    }
    explicit P(int a, int b, int c,int d) {
        cout << "explicit P(int a,int b,int c,int d)\n";
    }
};

void fp(const P &) {

}

int main() {
    Fraction f(3, 5);
    Fraction ff = f + 3; // 会将3转换为(3,1)   隐式转换
    ff.getNumAndDen();

    // 如果不想编译器进行隐式转换,可以在前面添加explicit关键字

    // c++2.0之前explicit只禁止"存在单一实参"转换
    // c++2.0之后explicit可以进制"多个实参"转换
//    Fraction1 f1(3,5);
//    Fraction1 f2 = f1+3; // 会将3转换为(3,1)   不会隐式转换    error: no match for ‘operator+’ (operand types are ‘Fraction1’ and ‘int’)
//    f2.getNumAndDen();
    P p1 = {77, 5};
    P p2 = {77, 5, 89};
//    P p3 = {77, 5, 89,99}; //error: converting to ‘P’ from initializer list would use explicit constructor ‘P::P(int, int, int, int)’
    return 0;
}
