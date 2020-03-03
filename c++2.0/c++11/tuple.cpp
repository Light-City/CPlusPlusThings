//
// Created by light on 19-11-4.
//

#include <iostream>
#include <tuple>

using namespace std;

// tuple使用
int main() {
    tuple<int, int, string> t, t1, t2;
    t = make_tuple(1, 2, "qwe");
    t1 = make_tuple(3, 2, "qwe");
    t2 = make_tuple(3, 2, "qwe");
    int a = get<0>(t);
    get<0>(t) = get<1>(t);   // 可以修改
    cout << a << endl;
    cout << (get<0>(t) > get<1>(t) ? "true" : "false") << endl; // 比较大小
    cout << (t1 == t2 ? "true" : "false") << endl;
    typedef tuple<int, int, int, string> T;
    cout << tuple_size<T>::value << endl;   // meta programming 处理类型
    cout << tuple_size<T>::value << endl;
    tuple_element<1, T>::type a1 = 10;
    cout << a1 << endl;
    return 0;
}