#include<iostream>
using namespace std;


class Employee 
{ 
    public: 
        virtual void raiseSalary() 
        {
            cout<<0<<endl; 
        }

        virtual void promote() 
        { /* common promote code */ } 
}; 

class Manager: public Employee { 
    virtual void raiseSalary() 
    {   
        cout<<100<<endl;    
    } 

    virtual void promote() 
    { /* Manager specific promote */ } 
}; 
class Engineer: public Employee { 
    virtual void raiseSalary() 
    {
        cout<<200<<endl;    
    }

    virtual void promote() 
    { /* Manager specific promote */ } 
}; 

// Similarly, there may be other types of employees 
// We need a very simple function to increment salary of all employees 
// Note that emp[] is an array of pointers and actual pointed objects can 
// be any type of employees. This function should ideally be in a class  
// like Organization, we have made it global to keep things simple 
void globalRaiseSalary(Employee *emp[], int n) 
{ 
    for (int i = 0; i < n; i++) 
        emp[i]->raiseSalary(); // Polymorphic Call: Calls raiseSalary()  
    // according to the actual object, not  
    // according to the type of pointer                                  
} 
int main(){
    Employee *emp[]={new Manager(),new Engineer};
    globalRaiseSalary(emp,2); 
    return 0;
}
