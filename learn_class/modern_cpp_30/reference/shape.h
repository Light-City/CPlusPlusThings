#pragma once
#include <iostream>
using namespace std;

class shape {
public:
    shape() { cout << "shape" << endl; }

    ~shape() {
        cout << "~shape" << endl;
    }
};

class circle : public shape {
public:
    circle() { cout << "circle" << endl; }


    ~circle() {
        cout << "~circle" << endl;
    }
};

class triangle : public shape {
public:
    triangle() { cout << "triangle" << endl; }


    ~triangle() {
        cout << "~triangle" << endl;
    }
};

class rectangle : public shape {
public:
    rectangle() { cout << "rectangle" << endl; }

    ~rectangle() {
        cout << "~rectangle" << endl;
    }
};
