//Eg10-5.cpp
#include<iostream>
using namespace std;

void handler(int n)throw(int,char,double){
    if(n==1) throw n;
    if(n==2) throw 'x';
    if(n==3) throw 1.1;
}
int main(){
    cout<<"Before handler..."<<endl;
    try{
        handler(1);
    }
    catch(int i){ cout<<"catch an integer..."<<endl;}
    catch(char c){cout<<"catch an char..."<<endl;}
    catch(double d){cout<<"catch an double..."<<endl;}
    system("pause");
}
