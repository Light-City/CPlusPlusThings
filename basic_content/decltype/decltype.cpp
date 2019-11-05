/**
 * @file decltype.cpp
 * @brief g++ -o decltype decltype.cpp -std=c++11
 * @author 光城
 * @version v1
 * @date 2019-08-08
 */

#include <iostream>
#include <vector>
using namespace std;
/**
 * 泛型编程中结合auto，用于追踪函数的返回值类型
 */
template <typename T>

auto multiply(T x, T y)->decltype(x*y)
{
    return x*y;
}

int main()
{
    int nums[] = {1,2,3,4};
    vector<int> vec(nums,nums+4);
    vector<int>::iterator it;

    for(it=vec.begin();it!=vec.end();it++)
        cout<<*it<<" ";
    cout<<endl;


    using nullptr_t = decltype(nullptr);
    nullptr_t  nu;
    int * p =NULL;
    if(p==nu)
        cout<<"NULL"<<endl;


    typedef decltype(vec.begin()) vectype;

    for(vectype i=vec.begin();i!=vec.end();i++)
        cout<<*i<<" ";
    cout<<endl;

    /**
     * 匿名结构体
     */
    struct
    {
        int d ;
        double b;
    }anon_s;

    decltype(anon_s) as; // 定义了一个上面匿名的结构体

    cout<<multiply(11,2)<<endl;

    return 0;
}
