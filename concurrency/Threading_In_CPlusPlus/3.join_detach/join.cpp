//
// Created by light on 20-1-31.
//

// join 注意点

/**
 * 一旦线程开始，我们要想等待线程完成，需要在该对象上调用join()
 * 双重join将导致程序终止
 * 在join之前我们应该检查显示是否可以被join,通过使用joinable()
*/
#include <iostream>
#include <thread>
#include <chrono>

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
    t1.join();
    if (t1.joinable()) {
        t1.join();
    }
    cout << "main() after" << endl;
    return 0;
}