//Eg10-7.cpp
#include<iostream>
using namespace std;
//内部再次throw异常的时候，函数不要带throw()
void Errhandler(int n)
{
    try{
        if(n==1) throw n;
        cout<<"all is ok..."<<endl;
    }
    catch(int n){
        cout<<"catch an int exception inside..."<<n<<endl;
        throw n;                                     //再次抛出本catch捕获的异常
    }
}
int main(){
    try{
        Errhandler(1); 
    }
    catch(int x){ cout<<"catch int an exception in main..."<<x<<endl; }
    cout<<"....End..."<<endl;
    system("pause");
    return 0;
}
