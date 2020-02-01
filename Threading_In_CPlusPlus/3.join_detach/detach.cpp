//
// Created by light on 20-1-31.
//

// join 注意点

/**
 * 这用于从父线程分离新创建的线程
 * 在分离线程之前，请务必检查它是否可以joinable，
 * 否则可能会导致两次分离，并且双重detach()将导致程序终止
 * 如果我们有分离的线程并且main函数正在返回，那么分离的线程执行将被挂起
*/
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

void run(int count) {
    while (count-- > 0) {
        cout << count << endl;
    }
    std::this_thread::sleep_for(chrono::seconds(3));
}

int main() {
    thread t1(run, 10);
    cout << "main()" << endl;
    t1.detach();
    if(t1.joinable())
        t1.detach();
    cout << "main() after" << endl;
    return 0;
}