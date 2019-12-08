#include <iostream>

using namespace std;

class bar {

};

// java 程序员风格
void foo() {
    cout << "method 1" << endl;
    bar *ptr = new bar();
    delete ptr;
}

bar *make_bar() {
    bar *ptr = nullptr;
    try {
        ptr = new bar();
    } catch (...) {
        delete ptr;
        throw;
    }
    return ptr;
}
// 独立出函数  分配和释放不在一个函数里
void foo1() {
    cout << "method 2" << endl;
    bar *ptr = make_bar();
    delete ptr;
}


int main() {
    foo();
    foo1();
    return 0;
}