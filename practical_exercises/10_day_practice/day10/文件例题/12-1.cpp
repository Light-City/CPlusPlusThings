//用cin输入字符串数据时，如果字符串中含有空白就不能完整输入。因为遇到空白字符时，cin就认为字符串结束了。
#include<iostream>
using namespace std;
int main(int argc, char const *argv[])
{
    char a[50];
    cout<<"please input a string:";
    cin>>a;
    cout<<a<<endl;
    system("pause");
    return 0;
}
/*
若a的内容是：
this is a string! 
就难以输入啦！
这样的数据应用输入流类的成员函数输入
*/