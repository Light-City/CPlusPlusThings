#ifndef CLOCK
# define CLOCK
class Clock
{
    public:
        Clock(int NewH,int NewM,int NewS);
        Clock(Clock &c);//拷贝构造函数,如果不加，编译器会自动生成一个拷贝构造函数，因此加不加都可以直接使用。
        void SetTime(int NewH,int NewM,int NewS);
        void ShowTime();
        ~Clock();//析构函数，编译器会自动产生一个默认的析构函数。
    private:
        int Hour,Minute,Second;
};

#endif


/*
#ifndef   标识符
       程序段1
#else
       程序段2
#endif
如果“标识符”未被定义过，则编译程序段1，否则编译程序段2。

*/