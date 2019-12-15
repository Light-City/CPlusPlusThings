//
// Created by light on 19-12-15.
//

#include <iostream> // std::cout/endl
#include <utility> // std::move
using namespace std;
class Obj {
public:
    Obj()
    {
        cout << "Obj()" << endl;
    }
    Obj(const Obj&)
    {
        cout << "Obj(const Obj&)"
             << endl;
    }
    Obj(Obj&&)
    {
        cout << "Obj(Obj&&)" << endl;
    }
};
Obj simple()
{
    Obj obj;
// 简单返回对象；一般有 NRVO
    return obj;
}
Obj simple_with_move()
{
    Obj obj;
// move 会禁止 NRVO
    return std::move(obj);
}
Obj complicated(int n)
{
    Obj obj1;
    Obj obj2;
    // 有分支，一般无 NRVO
    if (n % 2 == 0) {
        return obj1;
    } else {
        return obj2;
    }
}
int main()
{
    cout << "*** 1 ***" << endl;
    auto obj1 = simple();
    cout << "*** 2 ***" << endl;
    auto obj2 = simple_with_move();
    cout << "*** 3 ***" << endl;
    auto obj3 = complicated(42);
}