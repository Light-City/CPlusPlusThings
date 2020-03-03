//
// Created by light on 20-1-31.
//


#include <thread>
#include <iostream>

using namespace std;

// 4.Non-static member function
class Base {
public:
    static void fun(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
};

int main() {
    thread t(&Base::fun, 10);
    t.join();
    return 0;
}