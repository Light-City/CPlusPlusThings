#include "apple.h"
#include <iostream>

Apple::Apple(int i) : apple_number(i) {}
int Apple::add(int num) {
  take(num);
  return 0;
}
int Apple::add(int num) const {
  take(num);
  return 0;
}
void Apple::take(int num) const {
  std::cout << "take func " << num << std::endl;
}
int Apple::getCount() const {
  take(1);
  //    add(); //error
  return apple_number;
}