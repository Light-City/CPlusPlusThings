//有复数类Complex，利用运算符重载实现复数的加、减、乘、除等复数运算。
#include<iostream>
using namespace std;
class Complex {
    private:
        double r,i;
    public:
        Complex(double R=0,double I=0):r(R),i(I){};
        Complex operator+(Complex b);
        Complex operator-(Complex b);
        Complex operator*(Complex b);
        Complex operator/(Complex b);
        void display();
};

Complex Complex::operator +(Complex b)
{
    return Complex(r+b.r,i+b.i);
}
Complex Complex::operator -(Complex b)
{
    return Complex(r-b.r,i-b.i);
}
//求复数相乘的算法
Complex Complex::operator *(Complex b)
{   
    Complex t;
    t.r=r*b.r-i*b.i;
    t.i=r*b.i+i*b.r;
    return t;
}
//求复数相除的算法
Complex Complex::operator /(Complex b) {
    Complex t;
    double x;
    x=1/(b.r*b.r+b.i*b.i);
    t.r=x*(r*b.r+i*b.i);
    t.i=x*(i*b.r-r*b.i);
    return t;
}
void Complex::display(){
    cout<<r;
    if (i>0) cout<<"+";
    if (i!=0) cout<<i<<"i"<<endl;
    //}display();
};

int main(void) {
    Complex c1(1,2),c2(3,4),c3,c4,c5,c6;
    Complex a,b(2,3);
    a=b+2;       			//正确
    // a=2+b;       			//错误   
    a.display();  
    c3=c1+c2;
    c4=c1-c2;
    c5=c1*c2;
    c6=c1/c2;
    c1.display();
    c2.display();
    c3.display();
    c4.display();
    c5.display();
    c6.display();
    system("pause");
    return 0;
}

