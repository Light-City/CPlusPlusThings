//
// Created by light on 20-1-11.
//

#include <map>
#include <iostream>
#include <functional>

using namespace std;

int main() {

    map<string, function<int(int, int)>>
            op_dict{
            {"+",
                    [](int x, int y) {
                        return x + y;
                    }},
            {"-",
                    [](int x, int y) {
                        return x - y;
                    }},
            {"*",
                    [](int x, int y) {
                        return x * y;
                    }},
            {"/",
                    [](int x, int y) {
                        return x / y;
                    }},
    };
}