//Eg10-10.cpp
#include <iostream>
using namespace std;
const int MAX=3;
class Full{};	//L1  堆栈满时抛出的异常类
class Empty{};	//L2  堆栈空时抛出的异常类
class Stack{
    private:
        int s[MAX];
        int top;
    public:
        void push(int a);
        int  pop();
        Stack(){top=-1;}
};
void Stack::push(int a){
    if(top>=MAX-1) 
        throw Full();			
    s[++top]=a;
}
int Stack::pop(){
    if(top<0)   throw Empty();	
    return s[top--];
}
int main(){
    Stack s;
    try{
        s.push(10);        s.push(20);        s.push(30);
        //s.push(40);	//L5  将产生栈满异常
        cout<<"stack(0)= "<<s.pop()<<endl;
        cout<<"stack(1)= "<<s.pop()<<endl;
        cout<<"stack(2)= "<<s.pop()<<endl;
        cout<<"stack(3)= "<<s.pop()<<endl;		//L6
    }
    catch(Full){    cout<<"Exception: Stack Full"<<endl;    }
    catch(Empty){	   cout<<"Exception: Stack Empty"<<endl; }
    system("pause");
    return 0;
}
