/* 例2.cpp */
#include <iostream>
using namespace std;
class A {
public:
  void vf() { cout << "I come from class A" << endl; }
};
class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {};

int main() {
  D d;
  d.vf(); // okay
  
  return 0;
}
