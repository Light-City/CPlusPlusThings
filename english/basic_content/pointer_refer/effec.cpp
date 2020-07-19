#include<iostream>
using namespace std;
void test1(int* p)
{
    *p = 3;    //此处应该首先判断p是否为空，为了测试的需要，此处我们没加。
    return;
}

void test2(int& p)
{
    p = 3;    //此处应该首先判断p是否为空，为了测试的需要，此处我们没加。
    return;
}

int main() {
    int a=10;
    int *p=&a;
    test1(p);
    test2(a);
    cout<<a<<endl;
    return 0;
}
