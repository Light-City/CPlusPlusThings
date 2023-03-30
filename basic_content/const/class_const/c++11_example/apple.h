#pragma once

class Apple {
public:
  Apple(int i);
  // 使用c++11标准编译
  static const int apple_number = 10;
  // const int apple_number=10;
  void take(int num) const;
  int add(int num);
  int add(int num) const;
  int getCount() const;
};
