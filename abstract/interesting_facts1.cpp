/**
 * @file interesting_facts1.cpp
 * @brief 纯虚函数使一个类变成抽象类
 * @author 光城
 * @version v1
 * @date 2019-07-20
 */

#include<iostream> 
using namespace std; 


/**
 * @brief 抽象类至少包含一个纯虚函数
 */
class Test 
{ 
    int x; 
public: 
    virtual void show() = 0; 
    int getX() { return x; } 
}; 

int main(void) 
{ 
    Test t;  //error! 不能创建抽象类的对象
    return 0; 
} 

