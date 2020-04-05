//12-6.cpp
#include<iostream>
#include<fstream>
using namespace std;
int main(int argc, char const *argv[])
{
    fstream ioFile;
    ioFile.open("./a.dat",ios::out);
    ioFile<<"张三"<<"  "<<76<<" "<<98<<" "<<67<<endl;	//L3
    ioFile<<"李四"<<"  "<<89<<" "<<70<<" "<<60<<endl;
    ioFile<<"王十"<<"  "<<91<<" "<<88<<" "<<77<<endl;
    ioFile<<"黄二"<<"  "<<62<<" "<<81<<" "<<75<<endl;
    ioFile<<"刘六"<<"  "<<90<<" "<<78<<" "<<67<<endl;
    ioFile.close();
    ioFile.open("./a.dat",ios::in|ios::binary);
    char name[10];
    int chinese,math,computer;
    cout<<"姓名\t"<<"英语\t"<<"数学\t"<<"计算机\t"<<"总分"<<endl; 
    ioFile>>name;										
    while(!ioFile.eof())	{								
        ioFile>>chinese>>math>>computer;					
        cout<<name<<"\t"<<chinese<<"\t"<<math<<"\t"<<computer<<"\t"<<chinese+math+computer<<endl;		
        ioFile>>name;
    }
    ioFile.close();										
    system("pause");
    return 0;
}
