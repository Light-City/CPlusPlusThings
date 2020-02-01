//
// Created by light on 20-1-31.
//


#include <thread>
#include <iostream>

using namespace std;

// 3.functor (Funciton Object)
class Base {
public:
    void operator()(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
};

int main() {
    thread t(Base(), 10);
    t.join();
    return 0;
}