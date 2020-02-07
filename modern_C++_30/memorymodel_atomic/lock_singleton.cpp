//
// Created by light on 20-2-7.
//
#include <iostream>
using namespace std;

#include <mutex>

class singleton {
private:
    singleton() {}
    static singleton *p;
    static mutex lock_;
public:
    static singleton *instance();
};

singleton *singleton::p = nullptr;

singleton* singleton::instance() {
    lock_guard<mutex> guard(lock_);
    if (p == nullptr)
        p = new singleton();
    return p;
}
