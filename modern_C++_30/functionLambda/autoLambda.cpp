//
// Created by light on 20-1-11.
//


#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int get_count() {
    static int count = 0;
    return ++count;
}

class task {
public:
    task(int data) : data_(data) {}

    /**
     * this 标明按引用捕获外围对象（针对 lambda 表达式定义出现在一个非静态类成员内的情况）；
     * 注意默认捕获符 = 和 & 号可以自动捕获 this（并且在 C++20 之前，在 = 后写 this 会导致出错）
     * 本例子两次都按照第二次输出(this_thread::sleep_for(100ms);
     * this 标明按引用捕获外围对象
     *
     *
     * *this 标明按值捕获外围对象（针对 lambda 表达式定义出现在一个非静态类成员内的情况；C++17 新增语法）
     * 本例子正常输出
     */
    auto lazy_launch() {
        return
                [*this, count = get_count()]()
                        mutable {
                    ostringstream oss;
                    oss << "Done work " << data_
                        << " (No. " << count
                        << ") in thread "
                        << this_thread::get_id()
                        << '\n';
                    msg_ = oss.str();
                    calculate();
                };
    }

    void calculate() {
        this_thread::sleep_for(100ms);
        cout << msg_;
    }

private:
    int data_;
    string msg_;
};

int main() {
    auto t = task{37};
    thread t1{t.lazy_launch()};
    thread t2{t.lazy_launch()};
    t1.join();
    t2.join();
}

