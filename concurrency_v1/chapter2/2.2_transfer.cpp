//
// Created by light on 19-11-5.
//
#include <iostream>
#include <thread>

using namespace std;

class X {
public:
    void do_length_work() {};
};

void process_big_object(std::unique_ptr<X>);

int main() {
    X my_x;
    thread t(&X::do_length_work, &my_x); // 1


    std::unique_ptr<X> p(new X);
    p->do_length_work();
    std::thread tt(process_big_object,std::move(p));
    //std::thread实例的可移动且不可复制性。不可复制保性证了在同一时间点，
    // 一个std::thread实例只能关联一个执行线程；可移动性使得程序员可以自己决定，哪个实例拥有实际执行线程的所有权。
    return 0;
}