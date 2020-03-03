//
// Created by light on 20-1-5.
//

#include <iostream>

using namespace std;

// 使用template实现while循环

template<bool condition,
        typename Body>
struct WhileLoop;

template<typename Body>
struct WhileLoop<true, Body> {
    typedef typename WhileLoop<
            Body::cond_value,
            typename Body::next_type>::type
            type;
};

template<typename Body>
struct WhileLoop<false, Body> {
    typedef
    typename Body::res_type type;
};

template<typename Body>
struct While {
    typedef typename WhileLoop<Body::cond_value, Body>::type type;
};
template<typename Body>
using While_t = WhileLoop<Body::cond_value, Body>;

namespace my {
    template<class T, T v>
    struct integral_constant {
        static const T value = v;
        typedef T value_type;
        typedef integral_constant<T, v> type;
    };
}
template<int result, int n>
struct SumLoop {

    // 循环的条件
    static const bool cond_value =
            n != 0;

    // 循环后的结果
    static const int res_value =
            result;

    // 循环时的状态
    typedef my::integral_constant<
            int, res_value>
            res_type;

    // 循环执行一次时的状态
    typedef SumLoop<result + n, n - 1>
            next_type;
};

template<int n>
struct Sum {
    typedef SumLoop<0, n> type;
};

template<int n>
using Sum_t = SumLoop<0, n>;

int main() {
    cout << While<Sum<6>::type>::type::value << endl;
    cout << While_t<Sum_t<6>>::type::value << endl;
    return 0;
}
