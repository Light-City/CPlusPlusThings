#include<iostream>
#include<cstring>

using namespace std;
class Employee{
    public:
        Employee(const char *name,const char *id){
            strcpy(Name,name);
            strcpy(Id,id);
        }
        char* getName(){
            return Name;
        }
        char* getId(){
            return Id;
        }
        void display(){
            cout<<Name<<"\t"<<Id<<endl;
        }

    private:
        char Name[20];
        char Id[20];
};


class Manager:public Employee{
    public:
        //直接调用构造方法传递，基类构造方法有参数，派生类必须通过构造方法，在初始化列表中传递参数
        Manager(const char *name,const char *id,int week):Employee(name,id){
            WeeklySalary=week*1000;
        }
        void display(){
            cout<<"经理："<<getName()<<"\t"<<getId()<<"\t"<<WeeklySalary<<endl;
        }
    private:
        int WeeklySalary;
};

class SaleWorker:public Employee{
    public:
        SaleWorker(const char*name,const char *id,int profit,int x):Employee(name,id){
            workerMoney=baseMoney+x*0.05*profit;
        }
        void display(){
            cout<<"销售员："<<getName()<<"\t"<<getId()<<"\t"<<workerMoney<<endl;
        }

    private:
        float baseMoney=800.0;
        float workerMoney;
};
class HourWorker:public Employee{
    public:
        HourWorker(const char*name,const char *id,int h):Employee(name,id){
            TotalMoney=h*hourMoney;
        }
        void display(){
            cout<<"小时工："<<getName()<<"\t"<<getId()<<"\t"<<TotalMoney<<endl;
        }
    private:
        float hourMoney=100.0;
        float TotalMoney;
};



int main(int argc, char const *argv[])
{
    cout<<"请输入工作周：";
    int week;
    cin>>week;
    Manager m("小王","11111111",week);
    m.display();
    cout<<"请输入销售利润：";
    int profit;
    cin>>profit;
    cout<<"请输入销售件数：";
    int x;
    cin>>x;
    SaleWorker s("小李","222222",profit,x);
    s.display();
    cout<<"请输入工作小时：";
    int hour;
    cin>>hour;
    HourWorker h("小何","333333",hour);
    h.display();
    system("pause");
    return 0;
}
