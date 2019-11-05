class Apple
{
private:
    int people[100];
public:
    Apple(int i);
    static int ap; //在类实现文件中定义并初始化
    static const int apple_number;
    void take(int num) const;
    int add(int num);
    int add(int num) const;
    int getCount() const;
};

