/**
 * @file blackclass.cpp
 * @brief 空类的大小为1字节
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>

using namespace std;

class A{};
int main()
{
    cout<<sizeof(A)<<endl;
    return 0;
}
