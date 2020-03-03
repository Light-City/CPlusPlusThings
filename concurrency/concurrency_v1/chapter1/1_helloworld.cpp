//
// Created by light on 19-11-5.
//

#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void hello() {
    cout << "hello world" << endl;
}

int main() {
    thread t(hello);
    t.join();       // must add this line otherwise will failed!
    // 需要注意的是线程对象执行了join后就不再joinable了，所以只能调用join一次。
    return 0;
}