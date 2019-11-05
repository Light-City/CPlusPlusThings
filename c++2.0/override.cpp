//
// Created by light on 19-11-3.
//

#include <iostream>

using namespace std;

class Base {
public:
    Base(){}
    virtual void func() {}
};
class Derivered:public Base{
    virtual void func(int) override {}  //error: ‘virtual void Derivered::func(int)’ marked ‘override’, but does not override
};
// override用于虚函数，上面的virtual void func(int)实际上不是重写父类的虚函数，而是定义一个新的虚函数，
// 我们的本意是重写虚函数，当不加overrride的时候,这样写编译器不会报错，
// 那如果像下面加上override的话，则会报错，表示告诉了编译器，我确实要重写，但写错了，没有重写，于是就报错了,
// 这样就能给我们对虚函数的重写做检查!

int main() {

}