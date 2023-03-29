/* 递归2.cpp */
#include <iostream>
using namespace std;
int f(int n, int k);
int main(int argc, char const *argv[]) {
  cout << "请输入n与k" << endl;
  int n, k;
  cin >> n;
  cin >> k;
  cout << f(n, k) << endl;
  
  return 0;
}

int f(int n, int k) {
  if ((n == k) || (k == 0)) {
    return 1;
  } else {
    return f(n - 1, k - 1) + f(n - 1, k);
  }
}