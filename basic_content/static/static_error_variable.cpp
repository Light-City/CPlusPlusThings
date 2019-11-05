// variables inside a class 

#include<iostream> 
using namespace std; 

class Apple 
{ 
    public: 
        static int i; 

        Apple() 
        { 
            // Do nothing 
        }; 
}; 

int main() 
{ 
    Apple obj1; 
    Apple obj2; 
    obj1.i =2; 
    obj2.i = 3; 

    // prints value of i 
    cout << obj1.i<<" "<<obj2.i; 
} 
