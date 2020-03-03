//
// Created by light on 19-12-16.
//

#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <string>
#include "../container1/output_container.h"
#include <tuple>

using namespace std;

int main() {
    set<int> s{1, 1, 1, 2, 3, 4};
    cout << s << endl;

    multiset<int, greater<int>> ms{1, 1, 1, 2, 3, 4};
    cout << ms << endl;


    map<string, int> mp{
            {"one",   1},
            {"two",   2},
            {"three", 3},
            {"four",  4}
    };

    cout << mp << endl;


    mp.insert({"four", 4});
    cout << mp << endl;

    cout << (mp.find("four") == mp.end()) << endl;

    cout << (mp.find("five") == mp.end()) << endl;

    mp["five"] = 5;

    cout << mp << endl;


    multimap<string, int> mmp{
            {"one",   1},
            {"two",   2},
            {"three", 3},
            {"four",  4}
    };

    cout << mmp << endl;

    mmp.insert({"four", -4});

    cout << mmp << endl;

    cout << (mp.find("four")->second) << endl;
    cout << (mp.lower_bound("four")->second) << endl;

    cout << (mp.upper_bound("four")->second) << endl;
    cout << ((--mp.upper_bound("four"))->second) << endl;

    multimap<string, int>::iterator
            lower, upper;
    std::tie(lower, upper) =
            mmp.equal_range("four");
    cout << (lower != upper) << endl;   // 检测区间非空

    cout << lower->second << endl;
    cout << (--upper)->second << endl;

}