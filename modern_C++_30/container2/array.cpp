//
// Created by light on 19-12-16.
//

#include <iostream>
#include <map>  // std::map
#include "../container1/output_container.h"

using namespace std;
#define ARRAY_LEN(a) \
  (sizeof(a) / sizeof((a)[0]))


void test(int a[8]) {
    cout << ARRAY_LEN(a) << endl;
}


void test1(int arr[]) {
    //      不能编译
//     std::cout << std::size(arr)
//               << std::endl;
}


typedef char mykey_t[8];
typedef std::array<char, 8> mykey_t1;

int main() {
    int a[8];
    test(a);


    // C++17 直接提供了一个 size 方法，可以用于提供数组长度，
    int arr[] = {1, 2, 3, 4, 5};
    std::cout << "The array length is "
              << std::size(arr)
              << std::endl;
    // 并且在数组退化成指针的情况下会直接失败
    test1(arr);


    std::map<mykey_t, int> mp;
    mykey_t mykey{"hello"};
//    mp[mykey] = 5;
    //  轰，大段的编译错误


    std::map<mykey_t1, int> mp1;
    mykey_t1 mykey1{"hello"};
    mp1[mykey1] = 5;    // ok
    cout << mp1 << endl;
}