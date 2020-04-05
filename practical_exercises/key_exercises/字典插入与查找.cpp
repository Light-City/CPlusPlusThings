#include<iostream>
#include<map>
#include<cstring>
using namespace std;



int main(int argc, char const *argv[])
{
    map<const char*,const char*> mp;
    map<const char*,const char*>::iterator iter;
    const char key[3][20]={"img","system","ip"};
    const char value[3][20]={"d:/a.img","win7","193.68.6.3"};
    
    //make_pair插入
    for(int i=0;i<2;i++){
        mp.insert(make_pair(key[i],value[i]));
    }
    //pair<const char*,const char*>插入
    mp.insert(pair<const char*,const char*>(key[2],value[2]));

    //数组插入方式
    mp["addr"]="中国";
    //迭代器取出元素
    //循环取出元素
    for(iter=mp.begin();iter!=mp.end();iter++){
        cout<<iter->first<<"\t"<<iter->second<<endl;
    }
    char key1[20];
    cout<<"请输入按key查找：";
    cin.getline(key1,20);
    //查找元素
    for(iter=mp.begin();iter!=mp.end();iter++){
        if(strcmp(iter->first,key1)==0){
            cout<<iter->first<<"查找出来了！"<<"对应的值为："<<iter->second<<endl;
        }
    }
   
    //第一种删除方式
    //find只能用于查找数组建立的形式！并且不支持输入用cin等
    iter=mp.find("addr");
    if(iter!=mp.end()){
        cout<<iter->first<<"按照key查找出来了！"<<"对应的value为："<<iter->second<<endl;
        cout<<"开始删除元素！"<<endl;
        mp.erase(iter);
        // break;
    }
    //第二种方式删除
     //按照key删除元素
    char drop_key[20];
    //按照value删除元素
    char drop_value[20];
    cout<<"请输入按key删除：";
    cin.getline(drop_key,20);
    cout<<"请输入按value删除：";
    cin.getline(drop_value,20);
    for(iter=mp.begin();iter!=mp.end();iter++){
        if(strcmp(iter->first,drop_key)==0){
            cout<<iter->first<<"按照key查找出来了！"<<"对应的value为："<<iter->second<<endl;
            cout<<"开始删除元素！"<<endl;
            mp.erase(iter);
            break;
        }
        if(strcmp(iter->second,drop_value)==0){
            cout<<iter->second<<"value查找出来了！"<<"对应的key为："<<iter->first<<endl;
            cout<<"开始删除元素！"<<endl;
            mp.erase(iter);
            break;
        }
    }
    cout<<"------删除元素以后--------\n";
    //循环取出元素
    for(iter=mp.begin();iter!=mp.end();iter++){
        cout<<iter->first<<"\t"<<iter->second<<endl;
    }
    system("pause");
    return 0;
}
