//
// Created by light on 19-12-16.
//
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

class Obj1 {
public:
    Obj1()
    {
        cout << "Obj1()\n";
    }
    Obj1(const Obj1&)
    {
        cout << "Obj1(const Obj1&)\n";
    }
    Obj1(Obj1&&)
    {
        cout << "Obj1(Obj1&&)\n";
    }
};

class Obj2 {
public:
    Obj2()
    {
        cout << "Obj2()\n";
    }
    Obj2(const Obj2&)
    {
        cout << "Obj2(const Obj2&)\n";
    }
    Obj2(Obj2&&) noexcept
    {
        cout << "Obj2(Obj2&&)\n";
    }
};

int main()
{
    vector<int> v;

    int nums = 20;
    for (int i = 0; i < nums; ++i) {
        v.push_back(i + 1);
        cout << "v_size: " << v.size() << "\t v_capacity: " << v.capacity() << endl;
    }
    // 头两个在已有空间上成功构造。第三个时发现空间不足，系统会请求更大的空间，大小由实现决定（比如两倍）。
    // 有了足够的空间后，就会在新空间的第三个的位置构造（第三个obj1），成功之后再把头两个拷贝或移动过来。
    vector<Obj1> v1;
//    v1.reserve(2);
    v1.emplace_back();
    v1.emplace_back();
    v1.emplace_back();
    v1.emplace_back();

    vector<Obj2> v2;
    v2.reserve(2);
    v2.emplace_back();
    v2.emplace_back();
    v2.emplace_back();
}