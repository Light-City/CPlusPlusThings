/**
 * @file using_typedef.cpp
 * @brief g++ -o using_typedef using_typedef.cpp -std=c++11
 * 取代typedef，使用using来定义别名
 * @author 光城
 * @version v1
 * @date 2019-08-07
 */

#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> V1;
using V2 = vector<int>;


int main() 
{
    int nums1[] = {1,2,3,4,5,6};
    V1 vec1(nums1,nums1+sizeof(nums1)/sizeof(int));
    int nums2[] = {5,7,6};
    V2 vec2(nums2,nums2+sizeof(nums2)/sizeof(int));
    
    for(auto i:vec1)
        cout<<i<<" ";
    cout<<endl;

    for(auto i:vec2)
        cout<<i<<" ";
    cout<<endl;

    return 0;
}
