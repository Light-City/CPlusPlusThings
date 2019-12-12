//
// Created by light on 19-12-9.
//

#include <iostream>

using namespace std;

class A {
public:
    A(int a) : _a(a), _p(nullptr) {     // 初始化列表

    }

private:
    int _a;
    int *_p;
};

int main() {
    A aa(10);
    return 0;
}
