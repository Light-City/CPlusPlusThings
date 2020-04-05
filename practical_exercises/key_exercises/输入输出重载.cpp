#include<iostream>
using namespace std;
#include<cstring>
class Sales{
private:
    char name[10];
    char id[18];
    int age;
public:
    Sales(char *Name,char *ID,int Age);
    friend Sales &operator<<(ostream &os,Sales &s);
    friend Sales &operator>>(istream &is,Sales &s);

};


Sales::Sales(char *Name,char *ID,int Age){
    strcpy(name,Name);
    strcpy(id,ID);
    age=Age;
}

Sales &operator<<(ostream&os,Sales &s){
    os<<s.name<<"\t"<<s.id<<"\t"<<s.age<<endl;
    return s;
}

Sales &operator>>(istream&is,Sales &s){
    cout<<"输入雇员的姓名，身份证，年龄：\n";
    is>>s.name>>s.id>>s.age;
}

int main(int argc, char const *argv[])
{
    Sales s("张三","15611",26);
    cout<<s;
    cin>>s;
    cout<<s;
    system("pause");
    return 0;
}
