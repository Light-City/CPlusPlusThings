//设计一个时间类Time，它能够完成秒钟的自增运算。

#include<iostream>
using namespace std;

class Time{
    private:
        int hour,minute,second;
    public:
        Time(int h,int m, int s);
        Time operator++();
        //友元重载需要参数
        friend Time operator--(Time &t);
        void display(); 
};

Time::Time(int h, int m, int s){
    hour=h;
    minute=m;
    second=s;
    if (hour>=24)
        hour=0;
    if (minute>=60)
        minute=0;
    if (second>=60)
        second=0;
}

Time Time::operator++(){
    ++second;
    if (second>=60){
        second=0;
        ++minute;
        if(minute>=60){
            minute=0;
            ++hour;
            if(hour>=24)
                hour=0;
        }
    }
    return *this;       
}
Time operator--(Time &t){
    --t.second;
    if (t.second>=60){
        t.second=0;
        ++t.minute;
        if(t.minute>=60){
            t.minute=0;
            ++t.hour;
            if(t.hour>=24)
                t.hour=0;
        }
    }
    return t;  
}
void Time::display(){
    cout<<hour<<":"<<minute<<":"<<second<<endl;
}
int main(int argc, char const *argv[])
{
    Time t1(23,59,59);
    t1.display();
    ++t1; //隐式调用
    t1.display();
    t1.operator++(); //显式调用
    t1.display();
    Time t2(24,60,60);
    t2.display();
    ++t2;
    t2.display();
    --t2;
    t2.display();
    system("pause");
    return 0;
}
