
#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
using namespace std;

class Person{
    public:
        Person(){}
        Person(char *name,char *id,int math,int chinese,int english){
            strcpy(Name,name);
            strcpy(Id,id);
            Math=math;
            Chinese=chinese;
            English=english;
            Sum=Math+Chinese+English;
        }

        void display(){
            cout<<Name<<"\t"<<Id<<"\t"<<Math<<"\t"<<Chinese<<"\t"<<English<<"\t"<<Sum<<endl;
        }
    private:
        char Name[20];
        char Id[20];
        int Math;
        int Chinese;
        int English;
        int Sum;
};

int main(int argc, char const *argv[])
{
    char ch;
    char Name[20],Id[20];
    int Math,Chinese,English;
    fstream ioFile;
    ioFile.open("d:/per.dat",ios::out|ios::app);
    cout<<"---------建立学生档案信息----------\n";
    do{
        cout<<"请输入姓名：";
        cin>>Name;
        cout<<"请输入身份证号：";
        cin>>Id;
        cout<<"请输入数学成绩：";
        cin>>Math;
        cout<<"请输入汉语成绩：";
        cin>>Chinese;
        cout<<"请输入英语成绩：";
        cin>>English;
        Person per(Name,Id,Math,Chinese,English);
        ioFile.write((char *)&per,sizeof(per));
        cout<<"您是否继续建档？(Y/y)  ";
        cin>>ch;
    }while(ch=='y'||ch=='Y');
    ioFile.close();
    ioFile.open("d://per.dat",ios::in);
    Person p;
    ioFile.read((char*)&p,sizeof(p));
    vector<Person> v;
    vector<Person>::iterator vt;
    while(!ioFile.eof()){
        v.push_back(p);
        ioFile.read((char*)&p,sizeof(p));
    }
    ioFile.close();
    cout<<"---------输出档案信息-----------"<<endl;
    for(vt=v.begin();vt!=v.end();vt++){
        (*vt).display();
    }
    system("pause");
    return 0;
}
