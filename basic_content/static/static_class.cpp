#include<iostream> 
using namespace std; 

class Apple 
{ 
    int i; 
    public: 
    Apple() 
    { 
        i = 0; 
        cout << "Inside Constructor\n"; 
    } 
    ~Apple() 
    { 
        cout << "Inside Destructor\n"; 
    } 
}; 

int main() 
{ 
    int x = 0; 
    if (x==0) 
    { 
        static Apple obj; 
    } 
    cout << "End of main\n"; 
} 

