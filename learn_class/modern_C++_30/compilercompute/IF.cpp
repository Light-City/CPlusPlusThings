//
// Created by light on 19-12-28.
//

#include <iostream>

using namespace std;

// 使用template实现IF条件判断

template<bool cond,
        typename Then,
        typename Else>
struct IF;


template<typename Then,
        typename Else>
struct IF<true, Then, Else> {
    typedef Then result;
};

template<typename Then,
        typename Else>
struct IF<false, Then, Else> {
    typedef Else result;
};

// 判断奇数与偶数
template<int N>
struct isEven {
    static const auto RES = IF<N & 1 == 0, true_type, false_type>::result::value;
};

template<int nums1, int nums2>
struct Add_ {
    static const int value = nums1 + nums2;
};

template<int nums1, int nums2>
struct Sub_ {
    static const int value = nums1 - nums2;
};

// 加减
template<bool cond, int nums1, int nums2>
struct addSub {
    static const auto RES = IF<cond, Add_<nums1, nums2>, Sub_<nums1, nums2>>::result::value;
};

int main() {
    cout << isEven<10>::RES << endl;
    cout << addSub<true, 10, 2>::RES << endl;
}
