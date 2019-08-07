/**
 * @file classic_practice.cpp
 * @brief g++ -o classic_practice classic_practice.cpp -std=c++11
 * @author 光城
 * @version v1
 * @date 2019-08-07
 */

#include <iostream>
using namespace std;
/**
 * @brief namespace解决作用域不受限
 */
namespace Color 
{
    enum Type
    {
        RED=15,
        YELLOW,
        BLUE
    };
};

/**
 * @brief 上述如果 using namespace Color 后，前缀还可以省去，使得代码简化。
 * 不过，因为命名空间是可以随后被扩充内容的，所以它提供的作用域封闭性不高。
 * 在大项目中，还是有可能不同人给不同的东西起同样的枚举类型名。
 * 更“有效”的办法是用一个类或结构体来限定其作用域。
 *
 * 定义新变量的方法和上面命名空间的相同。
 * 不过这样就不用担心类在别处被修改内容。
 * 这里用结构体而非类，一是因为本身希望这些常量可以公开访问，
 * 二是因为它只包含数据没有成员函数。
 */
struct Color1
{
    enum Type
    {
        RED=102,
        YELLOW,
        BLUE
    };
};

/**
 * @brief C++11的枚举类
 * 下面等价于enum class Color2:int
 */
enum class Color2
{
    RED=2,
    YELLOW,
    BLUE
};

enum class Color3:char;  // 前向声明

// 定义
enum class Color3:char 
{
    RED='r',
    BLUE
};

int main() 
{
    // 定义新的枚举变量
    Color::Type c = Color::RED;
    cout<<c<<endl;  
    /**
     * 上述的另一种方法：
     * using namespace Color; // 定义新的枚举变量
     * Type c = RED;
     */
    Color1 c1;
    cout<<c1.RED<<endl;

    Color1::Type c11 = Color1::BLUE;
    cout<<c11<<endl;
    
    Color2 c2 = Color2::RED;
    cout << static_cast<int>(c2) << endl;
    
    char c3 = static_cast<char>(Color3::RED);
    cout<<c3<<endl;
    return 0;
}
