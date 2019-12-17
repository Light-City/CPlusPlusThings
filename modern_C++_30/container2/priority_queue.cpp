//
// Created by light on 19-12-16.
//


#include <functional>  // std::greater
#include <iostream>    // std::cout/endl
#include <memory>      // std::pair
#include <queue>       // std::priority_queue
#include <vector>      // std::vector
#include "../container1/output_container.h"

using namespace std;

int main()
{
    priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>>
            q;
    q.push({1, 1});
    q.push({2, 2});
    q.push({0, 3});
    q.push({9, 4});
    while (!q.empty()) {
        cout << q.top() << endl;
        q.pop();
    }
}