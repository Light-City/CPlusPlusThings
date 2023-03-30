#pragma once

class Apple {
public:
  Apple(int i);
  const int apple_number;
  void take(int num) const;
  int add(int num);
  int add(int num) const;
  int getCount() const;
};
