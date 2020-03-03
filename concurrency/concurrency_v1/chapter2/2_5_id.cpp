//
// Created by light on 19-11-5.
//

#include <iostream>
#include <thread>

using namespace std;


// 线程的通用标识符
std::thread::id master_thread;

void do_master_thread_work() {
    cout << "master" << endl;
}

void do_common_work() {
    cout << "common" << endl;
}

void some_core_part_of_algorithm() {
    if (std::this_thread::get_id() == master_thread) {
        do_master_thread_work();
    }
    do_common_work();
}

int main() {
    std::cout << std::this_thread::get_id() << endl;
    thread t(some_core_part_of_algorithm);
    t.join();
    return 0;
}