//
// Created by light on 19-12-27.
//
#include<iostream>
#include <vector>
#include <cstring>


using namespace std;

// 函数模板
template<typename T>
bool isEqual(T t1, T t2) {
    cout << "函数模板" << endl;
    return t1 == t2;
}

// 函数模板全特化
template<>
bool isEqual(const char *t1, const char *t2) {
    cout << "函数模板全特化" << endl;
    return strcmp(t1, t2);
}

// 函数模板偏特化   C++不支持函数模板偏特化 可以重载解决
//template<typename T>
//bool isEqual<T, int>(T a, double b) {
//    cout << "函数模板偏特化" << endl;
//    return a == b;
//}

// 改为重载   下面是重载!!! 而不是偏特化!
template<typename T>
bool isEqual(T a, double b) {
    cout << "函数重载" << endl;
    return a == b;
}

/// =============================================================

// 类模板
template<class T>
class comp {
public:
    bool isEqual(T t1, T t2) {
        cout << "类模板" << endl;
        return t1 == t2;
    }
};

// 类模板全特化
template<>
class comp<const char *> {
public:
    bool isEqual(const char *t1, const char *t2) {
        cout << "类模板全特化" << endl;
        return strcmp(t1, t2);
    }
};

// 类模板偏特化
template<typename T>
class comp<T *> {
public:
    bool isEqual(T *t1, T *t2) {
        cout << "类模板偏特化" << endl;
        return strcmp(t1, t2);
    }
};

int main() {
    isEqual(1, 2);       // 实例化的时候做类型检查
    isEqual(1, 2.1);
//    isEqual(1.1, 2.1); // 模糊的!
    isEqual("qwqw", "asd");
    comp<int> c;
    c.isEqual(10, 11);
    comp<const char *> cc;
    cc.isEqual("he", "lo");
    comp<char *> p;
    p.isEqual("he", "ll");
}
