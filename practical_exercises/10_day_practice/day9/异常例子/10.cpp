//Eg10-11.cpp
#include <iostream>
using namespace std;
const int MAX=3;
class Full{
    int a;
    public:
        Full(int i):a(i){}
        int getValue(){return a;}
};
class Empty{};
class Stack{
    private:
        int s[MAX];
        int top;
    public:
        Stack(){top=-1;}
        void push(int a){
            if(top>=MAX-1)  
                throw Full(a);			
            s[++top]=a;
        }
        int pop(){
            if(top<0)
                throw Empty();
            return s[top--];
        }
};
int main(){
    Stack s;
    try{
        s.push(10);  
        s.push(20);
        s.push(30);  
        s.push(40);
    }
    catch(Full e){	
        cout<<"Exception: Stack Full..."<<endl;
        cout<<"The value not push in stack:"<<e.getValue()<<endl; 
    }
    system("pause");
}
