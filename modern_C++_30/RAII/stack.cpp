//
// Created by light on 19-12-9.
//
#include <iostream>
class Obj {
public:
    Obj() { puts("Obj()"); }
    ~Obj() { puts("~Obj()"); }
};
void foo(int n)
{
    Obj obj;
    if (n == 42)
        throw "life, the universe and everything";
}
// 不管是否发生了异常，obj 的析构函数都会得到执行。
int main()
{
    try {
        foo(41);
        foo(42);
    }
    catch (const char* s) {
        puts(s);
    }
}
