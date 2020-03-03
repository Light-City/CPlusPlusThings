//
// Created by light on 20-2-7.
//

#include <iostream>

using namespace std;

class singleton {
private:
    static singleton *p;
    singleton() {}
public:
    singleton *instance();
};

singleton *singleton::instance() {
    static singleton p;
    return &p;
}


