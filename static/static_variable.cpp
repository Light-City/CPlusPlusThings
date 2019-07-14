// variables inside a class 

#include<iostream> 
using namespace std; 

class GfG 
{ 
    public: 
        static int i; 

        GfG() 
        { 
            // Do nothing 
        }; 
}; 

int GfG::i = 1; 

int main() 
{ 
    GfG obj; 
    // prints value of i 
    cout << obj.i; 
} 
