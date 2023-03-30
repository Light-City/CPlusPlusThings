//
// Created by light on 19-12-14.
//

#include <iostream>
#include <functional>

using namespace std;

// xvalue
int&& f(){
    return 3;
}

struct As
{
    int i;
};

As&& ff(){
    return As();
}

int main() {
    // lvalue
    int x = 0;
    cout << "(x).addr = " << &x << endl;
    cout << "(x = 1).addr = " << &(x = 1) << endl;
    cout << "(++x).addr = " << &++x << endl;
    cout << "(cout << ' ').addr=" << &(cout << ' ') << endl;
    cout << "(\"hello\").addr=" << &("hello") << endl;
    // rvalue
    cout<<true<<endl;
    // xvalue
    f(); // The expression f() belongs to the xvalue category, because f() return type is an rvalue reference to object type.
    static_cast<int&&>(7); // The expression static_cast<int&&>(7) belongs to the xvalue category, because it is a cast to an rvalue reference to object type.
    std::move(7); // std::move(7) is equivalent to static_cast<int&&>(7).

    ff().i; // The expression f().i belongs to the xvalue category, because As::i is a non-static data member of non-reference type, and the subexpression f() belongs to the xvlaue category.


    return 0;
}