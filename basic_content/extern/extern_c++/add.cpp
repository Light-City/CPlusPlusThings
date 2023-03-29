#include <iostream>
using namespace std;
extern "C" {
#include "add.h"
}
int main() {
  std::cout << add(2, 3) << std::endl;
  return 0;
}
