#include<iostream>
using namespace std;
//函数异常可以抛出去由主函数来处理
void fun(int x){
    try{
        if (x==0)
            throw "异常";
    }catch(...){
        cout<<"in fun"<<endl;
        throw 1;
    }
}

int main(int argc, char const *argv[])
{

    try{
        fun(0);
    }catch(int n){
        cout<<"in main"<<endl;
    }

    system("pause");
    return 0;
}
