/*
一圆型游泳池如图所示，现在需在其周围建一圆型过道，并在其四周围上栅栏。栅栏价格为35元/米，过道造价为20元/平方米。
过道宽度为3米，游泳池半径由键盘输入。要求编程计算并输出过道和栅栏的造价。

图形描述：大圆嵌套小圆：
小圆在大圆中间，小圆为游泳池，大圆与小圆间隔为过道。
*/
#include<iostream>
using namespace std;
const float PI=3.14159;
const float FencePrice=35;
const float ConcretePrice=20;

class Circle
{
    private:
        float radius;
    public:
        Circle(float r);
        float Circumference() const;
        float Area() const;
};
Circle::Circle(float r)
{
    radius=r;
}

// 计算圆的周长
float Circle::Circumference() const
{
    return 2 * PI * radius;
}
        
// 计算圆的面积 
float Circle::Area() const
{
    return PI * radius * radius;
}

int main(int argc, char const *argv[])
{
    float radius;
    float FenceCost, ConcreteCost; 
    
    // 提示用户输入半径
    cout<<"Enter the radius of the pool: ";
    cin>>radius;

    // 声明 Circle 对象
    Circle Pool(radius);
    Circle PoolRim(radius + 3);       
    // 计算栅栏造价并输出
    FenceCost = PoolRim.Circumference() * FencePrice;
    cout << "Fencing Cost is ￥" << FenceCost << endl;
    
    //  计算过道造价并输出
    ConcreteCost = (PoolRim.Area() - Pool.Area())*ConcretePrice;
    cout << "Concrete Cost is ￥" << ConcreteCost << endl;
    system("pause");
    return 0;
}
