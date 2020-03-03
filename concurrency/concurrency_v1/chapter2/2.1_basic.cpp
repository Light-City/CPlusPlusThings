//
// Created by light on 19-11-5.
//

#include <iostream>
#include <thread>
#include <unistd.h>
#include <cassert>

using namespace std;

class background_task {
public:
    void operator()() const {
        cout << "ok" << endl;
    }
};

void do_something(int &i) {
    cout << "do_something" << endl;
}

struct func {
    int &i;

    func(int &i_) : i(i_) {}

    void operator()() {
        for (unsigned j = 0; j < 1000000; ++j) {
            do_something(i);           // 1. 潜在访问隐患：悬空引用
        }
    }
};

// 特殊情况下的等待
void f() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try {
//        do_something_in_current_thread();
    }
    catch (...) {
        t.join();  // 1
        throw;
    }
    t.join();  // 2
}

// try catch 只能捕获轻量级错误,所以如需确保线程在函数之前结束——查看是否因为线程函数使用了局部变量的引用，
// 以及其他原因——而后再确定一下程序可能会退出的途径，无论正常与否，可以提供一个简洁的机制，来做解决这个问题。

// 一种方式是使用“资源获取即初始化方式”(RAII，Resource Acquisition Is Initialization)，并且提供一个类，在析构函数中使用join()，
// std::thread支持移动的好处是可以创建thread_guard类的实例，并且拥有其线程的所有权。
class thread_guard {
    std::thread &t;
public:
    explicit thread_guard(std::thread &t_) :
            t(t_) {}

    ~thread_guard() {
        if (t.joinable()) // 1
        {
            t.join();      // 2
        }
    }

    thread_guard(thread_guard const &) = delete;   // 3
    thread_guard &operator=(thread_guard const &) = delete;
};
void f1()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
//    do_something_in_current_thread();
}    // 4
// 当线程执行到4处时，局部对象就要被逆序销毁了。因此，thread_guard对象g是第一个被销毁的，
// 这时线程在析构函数中被加入2到原始线程中。
// 即使do_something_in_current_thread抛出一个异常，这个销毁依旧会发生。


int main() {

    background_task f;
//    thread t(f);       // ok
//    t.join();
    //声明一个名为my_threadx的函数,这个函数带有一个参数(函数指针指向没有参数并返回background_task对象的函数)，返回一个std::thread对象的函数
//    thread my_thread1(background_task());

    // 针对Most Vexing Parse问题解决如下：
//    thread my_thread1((background_task())); // 多组括号
//    my_thread1.join();
//    thread my_thread2{background_task()};   // 新的初始化语法
//    my_thread2.join();

//    thread myThread([](){
//        cout<<"ok"<<endl;
//    });
//    myThread.join();
    // 后台运行线程
    std::thread t(f);
    t.detach();
    assert(!t.joinable());

    return 0;
}