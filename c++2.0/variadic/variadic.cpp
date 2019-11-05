//
// Created by light on 19-11-2.
//

// variadic template(c++11) 数量不定的模板参数
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;
// 参数个数  参数个数逐一递减
// 参数类型  参数类型也逐一递减

// (3)
// 需要重载一个终止函数
template<typename T>
void _hash(size_t &seed, const T &val) {
    cout << "hash over " << val << endl;
}

// (2)
// 展开函数, 把参数分为一个普通参数和一个参数包，调用一次，参数包大小就减一
template<typename T,typename... Args>
void _hash(size_t &seed, const T& val,const Args &... args) {
    cout << "parameter " << val<< endl;
    // 递归调用自己
    _hash(seed, args...);       // 上面如果不给T参数,会自己调用自己成死循环
}


// 泛化版 (1)
template<typename ...Args>
size_t _hash(const Args &... args) {
    cout << "hash start " << endl;
    size_t seed = 10;
    // 递归调用自己
    _hash(seed, args...);
    return seed;
}


template<typename ...Args>
class A {
private:
    int size = 0;    // c++11 支持类内初始化
public:
    A() {
        size = sizeof...(Args);
        cout << size << endl;
    }
};

int main(void) {
    size_t f = 10;
    _hash("asdas", 2, 3, 4);
    A<int, string, vector<int>> a;    // 类型任意

    // Tuple就是利用这个特性(变长参数模板)
    tuple<int, string> t = make_tuple(1, "hha");
    int firstArg = get<0>(t);
    string secondArg = get<1>(t);
    cout << firstArg << " " << secondArg << endl;
    return 0;
}


