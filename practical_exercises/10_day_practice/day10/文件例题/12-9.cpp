//Eg12-12.cpp
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;
class Employee{
private:
    int number ,age;
    char name[20];
    double sal;
public:
    Employee(){}
    Employee(int num,char* Name,int Age, double Salary){
        number=num;
        strcpy(name,Name);
        age=Age;
        sal=Salary;
    }
    void display(){
        cout<<number<<"\t"<<name<<"\t"<<age<<"\t"<<sal<<endl;
    }
};

int main(){
    ofstream out("D:/Employee.dat",ios::out);	//定义随机输出文件
    Employee e1(1,"张三",23,2320);
    Employee e2(2,"李四",32,3210);
    Employee e3(3,"王五",34,2220);
    Employee e4(4,"刘六",27,1220);
    out.write((char*)&e1,sizeof(e1));		//按e1,e2,e3,e4顺序写入文件
    out.write((char*)&e2,sizeof(e2));
    out.write((char*)&e3,sizeof(e3));
    out.write((char*)&e4,sizeof(e4));

     //下面的代码将e3（即王五）的年龄改为40岁   
    Employee e5(3,"王五",40,2220);
    out.seekp(3*sizeof(e1));		//指针定位到第3（起始为0）个数据块
    out.write((char*)&e5,sizeof(e5));	//将e5写到第3个数据块位置，覆盖e3
    out.close();			//关闭文件
    system("pause");
}