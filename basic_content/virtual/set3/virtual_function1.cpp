#include<iostream> 
using namespace std; 

class Derived; 

class Base { 
    public: 
        virtual void fun() { cout << "Base Fun"; } 
     //   friend int main(); 
}; 

class Derived: public Base { 
    private: 
        void fun() { cout << "Derived Fun"; } 
}; 

int main() 
{ 
    Base *ptr = new Derived; 
    ptr->fun(); 
    return 0; 
}
