//
// Created by light on 19-11-4.
//

#include <iostream>
#include <bitset>

using namespace std;

// example 1
void printX() {

}

// 特化
template<typename T, typename... Type>
void printX(const T &firstArg, const Type &...args) {
    cout << firstArg << endl;       // 先减少一个做操作
    printX(args...);    // 其余的继续分为一个+一包进行递归知道没有了 调用printX()
}

// 如果写了下面接收任意参数,下面这个跟上面可以共存 泛化版  永远不会被调用,前面特化把它取代了
template<typename T, typename... Type>
void printX(const Type &...args) {
    printX(args...);
}


int main() {
    printX(1, "hello", bitset<16>(377), 42);
    return 0;
}