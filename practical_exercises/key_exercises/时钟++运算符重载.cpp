#include<iostream>   
#include<cmath>   
using namespace std;   
  
/*  
 *  时钟类  
 */  
class Clock    
{   
private:    
    int Hour, Minute, Second;   
public:    
    Clock(int h=0, int m=0, int s=0);   
    void ShowTime();   
    Clock& operator ++();    
    Clock operator ++(int);   
};   
  
/*  
 *  时钟类构造函数  
 */  
Clock::Clock(int h,int m, int s)   
{   
    if(h>=0 && h<24 && m>=0 && m<60 && s>=0 && s<60)   
    {   
        Hour = h;   
        Minute =m;    
        Second= s;   
    }   
    else  
        cout<<"输入的时间格式错误!"<<endl;   
}   
  
/*  
 *  显示时间  
 */  
void Clock::ShowTime()   
{   
    cout<<Hour<<":"<<Minute<<":"<<Second<<endl;   
}   
  
/*  
 *  时间递增一秒（重载前缀++运算符）  
 */  
Clock& Clock::operator ++()    
{    
    Second++;   
    if (Second >= 60)   
    {   
        Second = Second - 60;   
        Minute++;   
        if (Minute >= 60)   
        {   
            Minute = Minute - 60;   
            Hour++;   
            Hour = Hour % 24;   
        }   
    }   
    return *this;   
}   
  
/*  
 *  时间递增一秒（重载后缀++运算符）  
 */  
Clock Clock::operator ++(int)   
{   
    Clock old = *this;   
    ++(*this);   
    return old;   
}   
  
/*  
 *  主函数  
 */  
int main()   
{   
    Clock myClock(23,59,59);   
    cout<<"初始化显示时间为:\t\t";   
    myClock.ShowTime();   
  
    cout<<"执行myClock++后的时间为:\t";   
  
    //先执行ShowTime()，输出myClock=23:59:59，   
    //再执行myClock++，此时myClock=00:00:00   
    (myClock++).ShowTime();   
  
    cout<<"执行++myClock后的时间为:\t";   
  
    //先执行++myClock，此时myClock=00:00:01   
    //再执行ShowTime()，输出myClock=00:00:01   
    (++myClock).ShowTime();   
    system("pause");
}  