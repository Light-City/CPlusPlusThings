//
// Created by light on 19-11-2.
//

#include <iostream>
#include <vector>

using namespace std;

int main() {
    int value[]{1, 2, 3};       // initializer_list<T> 会关联一个array<T,n> 里面元素被编译器逐一分解传给函数
    vector<int> v{2, 3, 5};
    return 0;
}