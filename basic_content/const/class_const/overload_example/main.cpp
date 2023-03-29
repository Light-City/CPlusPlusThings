#include "apple.h"
#include <iostream>
using namespace std;

int main() {
  Apple a(2);
  cout << a.getCount() << endl;
  a.add(10);
  //     const Apple b(3);
  //     b.add(); // error
  return 0;
}
