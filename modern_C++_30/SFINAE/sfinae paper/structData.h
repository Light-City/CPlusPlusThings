//
// Created by light on 20-1-6.
//

#ifndef MORDEN_C_STRUCTDATA_H
#define MORDEN_C_STRUCTDATA_H
// 类型A只有to_string 方法
struct A {
};

std::string to_string(const A &) {
    return "I am A";
}

// 类型B有serialize方法
struct B {
    std::string serialize() const {
        return "I am B";
    }
};

// 类型C有个serialize数据成员与to_string方法
struct C {
    std::string serialize;
};

std::string to_string(const C &) {
    return "I am C";
}
#endif //MORDEN_C_STRUCTDATA_H
