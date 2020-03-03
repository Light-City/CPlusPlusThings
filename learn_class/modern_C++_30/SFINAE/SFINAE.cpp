//
// Created by light on 20-1-5.
//
#include <iostream>

using namespace std;

template<typename T>
class IsClassT {
private:
    typedef char One;
    typedef struct {
        char a[2];
    } Two;

    template<typename C>
    static One test(int C::*);

    // Will be chosen if T is anything except a class.
    template<typename C>
    static Two test(...);

public:
    enum {
        Yes = sizeof(IsClassT<T>::test<T>(0)) == 1
    };
    enum {
        No = !Yes
    };
};

struct A {
};

int main() {
    // 0不能转换为int int::*因为int不是类，所以它不能有成员指针。
    cout << IsClassT<int>::Yes << endl;
    cout << IsClassT<A>::Yes << endl;

    return 0;
}