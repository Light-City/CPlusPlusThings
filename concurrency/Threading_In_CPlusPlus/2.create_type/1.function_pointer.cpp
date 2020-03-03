//
// Created by light on 20-1-31.
//

// 1.函数指针
#include <thread>
#include <iostream>

using namespace std;

void fun(int x) {
    while (x-- > 0) {
        cout << x << endl;
    }
}
// 注意：如果我们创建多线程 并不会保证哪一个先开始
int main() {
    std::thread t1(fun, 10);
//    std::1.thread t2(fun, 10);
    t1.join();
//    t2.join();
    return 0;
}