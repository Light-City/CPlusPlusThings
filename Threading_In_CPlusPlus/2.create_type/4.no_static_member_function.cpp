//
// Created by light on 20-1-31.
//


#include <thread>
#include <iostream>

using namespace std;

// 4.Non-static member function
class Base {
public:
    void fun(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
};

int main() {
    Base b;
    thread t(&Base::fun,&b, 10);
    t.join();
    return 0;
}