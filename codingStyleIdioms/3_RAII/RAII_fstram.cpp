//
// Created by light on 19-12-9.
//
#include <iostream>
#include <fstream>

// RAII 资源获取即初始化,例2

// C ++保证在对象超出范围时调用析构函数，而不管控制如何离开该范围。
// 即使抛出异常，所有本地对象也会超出范围，因此它们的相关资源将被清除。

void foo() {
    std::fstream file("bar.txt"); // open a file "bar.txt"
    if (rand() % 2) {
        // if this exception is thrown, we leave the function, and so
        // file's destructor is called, which closes the file handle.
        throw std::exception();
    }
    // if the exception is not called, we leave the function normally, and so
    // again, file's destructor is called, which closes the file handle.
}

int main() {
    try {
        foo();
    } catch (std::exception) {
        puts("exception!");
    }
}