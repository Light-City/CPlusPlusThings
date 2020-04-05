#include<iostream>
#include<cstring>
using namespace std;


template<class T> T compareMax(T t1,T t2){
    return t1>t2?t1:t2;
}

template<> char* compareMax<char *>(char *s1,char *s2){
    return strcmp(s1,s2)>=0?s1:s2;
}
int main(int argc, char const *argv[])
{
    cout<<compareMax(1,2)<<endl;
    cout<<compareMax("asda","qweq")<<endl;
    system("pause");
    return 0;
}
