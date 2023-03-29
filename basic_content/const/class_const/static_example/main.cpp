#include "apple.h"
#include <iostream>
int main() {
  Apple a(2);
  std::cout << a.getCount() << std::endl;
  std::cout << a.ap << std::endl;
  a.add(10);
  const Apple b(3);
  b.add(100);
  return 0;
}
