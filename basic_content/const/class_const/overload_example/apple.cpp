#include "apple.h"
#include <iostream>

int Apple::apple_number = 10;

Apple::Apple(int i) { apple_number = i; }
int Apple::add() {
  take(1);
  return 0;
}
int Apple::add(int num) const {
  take(num);
  return num;
}
void Apple::take(int num) const {
  std::cout << "take func " << num << std::endl;
}
int Apple::getCount() const {
  take(1);
  add(); // error
  return apple_number;
}