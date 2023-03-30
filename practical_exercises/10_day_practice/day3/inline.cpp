/* 内联函数.cpp */
#include <iostream>
using namespace std;
//函数声明
inline double CalArea(double radius);
int main(int argc, char const *argv[]) {
  double r(3.0);
  double area;
  area = CalArea(r);
  cout << area << endl;
  
  return 0;
}
//加关键字inline
inline double CalArea(double radius) { return 3.14 * radius * radius; }