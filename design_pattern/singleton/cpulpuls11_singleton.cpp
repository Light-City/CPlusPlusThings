//
// Created by light on 20-2-7.
//

#include <iostream>

using namespace std;

#include <mutex>
#include <atomic>

//C++ 11版本之后的跨平台实现
class singleton {
private:
    singleton() {}

    static mutex lock_;
    static atomic<singleton *> p;
public:
    singleton *instance();
};

mutex singleton::lock_;
atomic<singleton *> singleton::p;

/*
* std::atomic_thread_fence(std::memory_order_acquire);
* std::atomic_thread_fence(std::memory_order_release);
* 这两句话可以保证他们之间的语句不会发生乱序执行。
*/
singleton *singleton::instance() {
    singleton *tmp = p.load(memory_order_relaxed);
    atomic_thread_fence(memory_order_acquire);
    if (tmp == nullptr) {
        lock_guard<mutex> guard(lock_);
        tmp = p.load(memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new singleton();
            atomic_thread_fence(memory_order_release);
            p.store(tmp, memory_order_relaxed);
        }
    }
    return p;
}


