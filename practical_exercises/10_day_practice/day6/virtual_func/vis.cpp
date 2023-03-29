/* 从基类继承的成员将访问到派生类版本.cpp */
#include <iostream>
using namespace std;
class B {
public:
  void f() { g(); }
  virtual void g() { cout << "B::g"; }
};
class D : public B {
public:
  void g() { cout << "D::g\n"; }
};
int main() {
  D d;
  d.f();
  
  return 0;
}
