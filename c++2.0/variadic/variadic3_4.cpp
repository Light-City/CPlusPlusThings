//
// Created by light on 19-11-4.
//

#include <iostream>

using namespace std;

// 参数type相同的max
int max(initializer_list<int> initializerList) {
    int res = *initializerList.begin();
    for (auto elem:initializerList)
        res = max(res, elem);
    return res;
}


// 参数type相同的maximum
int maximum(int n) {
    return n;
}

template<typename ...Args>
int maximum(int n, Args...args) {
    return std::max(n, maximum(args...));
}

int main() {
    cout << max({10, 8, 100, 1}) << endl;
    cout << maximum(57, 48, 60, 100, 20, 18) << endl;
}