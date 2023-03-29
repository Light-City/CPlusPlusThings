//
// Created by light on 20-2-7.
//
#include <iostream>

using namespace std;

#include <mutex>

#define barrier() __asm__ volatile ("lwsync")

// method 1 operator new + placement new
//singleton *instance() {
//    if (p == nullptr) {
//        lock_guard<mutex> guard(lock_);
//        if (p == nullptr) {
//            singleton *tmp = static_cast<singleton *>(operator new(sizeof(singleton)));
//            new(p)singleton();
//            p = tmp;
//        }
//    }
//    return p;
//}
class singleton {
private:
    singleton() {}

    static singleton *p;
    static mutex lock_;
public:
    static singleton *instance();
};

singleton *singleton::p = nullptr;

singleton *singleton::instance() {
    if (p == nullptr) {
        lock_guard<mutex> guard(lock_);
        barrier();
        if (p == nullptr) {
            p = new singleton();
        }
    }
    return p;
}

