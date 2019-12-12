#include <iostream>
#include <mutex>
#include <fstream>
using namespace std;
// RAII 资源获取即初始化,例1

enum class shape_type {
    circle,
    triangle,
    rectangle,
};


class shape {
public:
    shape() { cout << "shape" << endl; }

    virtual void print() {
        cout << "I am shape" << endl;
    }

    virtual ~shape() {}
};

class circle : public shape {
public:
    circle() { cout << "circle" << endl; }

    void print() {
        cout << "I am circle" << endl;
    }
};

class triangle : public shape {
public:
    triangle() { cout << "triangle" << endl; }

    void print() {
        cout << "I am triangle" << endl;
    }
};

class rectangle : public shape {
public:
    rectangle() { cout << "rectangle" << endl; }

    void print() {
        cout << "I am rectangle" << endl;
    }
};

// 利用多态 上转 如果返回值为shape,会存在对象切片问题。
shape *create_shape(shape_type type) {
    switch (type) {
        case shape_type::circle:
            return new circle();
        case shape_type::triangle:
            return new triangle();
        case shape_type::rectangle:
            return new rectangle();
    }
}

class shape_wrapper {
public:
    explicit shape_wrapper(shape *ptr = nullptr) : ptr_(ptr) {}

    ~shape_wrapper() {
        delete ptr_;
    }

    shape *get() const {
        return ptr_;
    }

private:
    shape *ptr_;
};


void foo() {
    shape_wrapper ptr(create_shape(shape_type::circle));
    ptr.get()->print();
}

int main() {

    // 第一种方式
    shape *sp = create_shape(shape_type::circle);
    sp->print();
    delete sp;

    // 第二种方式 RAII
    foo();

    return 0;
}
