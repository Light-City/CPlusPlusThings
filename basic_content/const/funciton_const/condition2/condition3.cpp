#include <iostream>
using namespace std;
int main() {
  const int num = 10;
  const int *const ptr = &num;
  cout << *ptr << endl;
}
