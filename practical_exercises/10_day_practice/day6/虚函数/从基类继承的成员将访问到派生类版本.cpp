#include <iostream>
using namespace std;
class B{
public: 
    void f(){ g(); } 
    virtual void g(){ cout << "B::g"; }
};
class D : public B{
public: 
    void g(){ cout << "D::g\n"; }
};
int main(){
    D d;
    d.f();
    system("pause");
    return 0;
}
