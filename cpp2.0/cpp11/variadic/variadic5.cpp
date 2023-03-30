//
// Created by light on 19-11-4.
//

#include <iostream>
#include <tuple>
#include <bitset>

using namespace std;

// tuple递归调用

// 得出这种打印[7,5....,42]
// 需要知道有几个以及现在操作的是第几个   sizeof...()

// cout<< make_tuple(7.5,string("hello"),bitset<16>(377),47);


template<int IDX, int MAX, typename... Args>
struct print_tuple {
    static void print(ostream &os, const tuple<Args...> &t) {
        os << get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
        print_tuple<IDX + 1, MAX, Args...>::print(os, t);
    }
};

// 偏特化
template<int MAX, typename... Args>
struct print_tuple<MAX, MAX, Args...> {
    static void print(ostream &os, const tuple<Args...> &t) {
    }
};

template<typename ... Args>
ostream &operator<<(ostream &os, const tuple<Args...> &t) {
    os << "[";
    print_tuple<0, sizeof...(Args), Args...>::print(os, t);
    return os << "]";
}


int main() {
    cout << make_tuple(7.5, string("hello"), bitset<16>(377), 47) << endl;
}