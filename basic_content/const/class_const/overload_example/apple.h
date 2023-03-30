#pragma once

class Apple {
public:
  Apple(int i);
  static int apple_number;
  void take(int num) const;
  int add();
  int add(int num) const;
  int getCount() const;
};
