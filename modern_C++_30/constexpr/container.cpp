//
// Created by light on 19-12-16.
//

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "output_container.h"


using namespace std;

int main() {
    map<int, int> mp{
            {1, 1},
            {2, 4},
            {3, 9}};
    cout << mp << endl;
    vector<vector<int>> vv{
            {1, 1},
            {2, 4},
            {3, 9}};
    cout << vv << endl;

    pair<int, int> p{1, 2};
    cout << p << endl;

    set<int> s{1, 2, 3};
    cout << s << endl;

    vector<char> v{'a', 'b'};
    cout << v << endl;
    set<char *> vs{"a", "b"};
    cout << vs << endl;

    map<int, char *> mm{
            {1,   "23"},
            {2, "234hi"}
    };
    cout << mm << endl;

}
