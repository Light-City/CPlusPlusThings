//
// Created by light on 19-11-3.
//

#include <vector>

using namespace std;

// 不可以对alias template做偏特化或全特化
template<typename T>
using Vec=vector<T, allocator<T>>;  // alias template using新用法
//# define Vec<T> template<typename T> vector<T, allocator<T>> // 使用宏不行

int main() {
    Vec<int> col;
// 如果使用宏上述会被替换为template<typename int> vector<int, allocator<int>>  error 不是我们想要的
// typedef vector<int, allocator<int>> Vec;   // typedef也无法做到,没法指定模板参数

    return 0;
}