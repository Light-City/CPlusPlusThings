//
// Created by light on 19-12-16.
//

#include "output_container.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
  map<int, int> mp{{1, 1}, {2, 4}, {3, 9}};
  cout << mp << endl;
  vector<vector<int>> vv{{1, 1}, {2, 4}, {3, 9}};
  cout << vv << endl;
}
