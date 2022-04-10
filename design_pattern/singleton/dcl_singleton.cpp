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

    // 实现一个内嵌垃圾回收类
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if(singleton::p)
                delete singleton::p;
        }
    };
    static CGarbo Garbo; // 定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数从而释放单例对象
};

singleton *singleton::p = nullptr;
singleton::CGarbo Garbo;
std::mutex singleton::lock_;

singleton* singleton::instance() {
    if (p == nullptr) {
        lock_guard<mutex> guard(lock_);
        if (p == nullptr)
            p = new singleton();
    }
    return p;
}
