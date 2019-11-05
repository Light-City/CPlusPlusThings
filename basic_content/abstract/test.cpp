/**
 * @file test.cpp
 * @brief C++中的纯虚函数(或抽象函数)是我们没有实现的虚函数！我们只需声明它!通过声明中赋值0来声明纯虚函数！
 * 纯虚函数：没有函数体的虚函数
 * @author 光城
 * @version v1
 * @date 2019-07-20
 */



/**
 * @brief 抽象类
 */
class Test 
{    
    // Data members of class 
public: 
    // Pure Virtual Function 
    virtual void show() = 0; 

    /* Other members */
}; 
