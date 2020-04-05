#include<iostream>
#include<string>
#include<map>
using namespace std;

int main(int argc, char const *argv[])
{
    string name[]={"张三","李四","王麻子"};
    double salary[]={1200,2000,1450};
    map<string,double>sal;
    map<string,double>::iterator p;
    for(int i=0;i<3;i++){
        sal.insert(make_pair(name[i],salary[i]));
    }
    sal["tom"]=6156;
    sal["bob"]=5999;
    for(p=sal.begin();p!=sal.end();p++){
        cout<<p->first<<"\t"<<p->second<<endl;
    }
    string person;
    cout<<"输入查找人员的姓名:";
    cin>>person;
    int flag=1;
    for(p=sal.begin();p!=sal.end();p++)
        if(p->first==person){
            cout<<p->second<<endl;    
            flag=0; 
        }
    if(flag)
        cout<<"没查找到对应的结果！"<<endl; 

    cout<<"输入待删除的人员的姓名:";
    cin>>person;
    map<string,double>::iterator it;
    it = sal.find(person);
    if(it!=sal.end()){
        cout<<"查找成功："<<(*it).first<<":"<<(*it).second<<endl;
        sal.erase(it);
        cout<<"删除成功"<<endl;
    }
    cout<<"删除后的结果为"<<endl;
    for(p=sal.begin();p!=sal.end();p++){
        cout<<p->first<<p->second<<endl;
    }

    system("pause");
    return 0;
}
