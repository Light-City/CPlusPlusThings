//函数模板参数可以是类属参数，也可以包括普通类型的参数
#include<iostream>
using namespace std;


template <class T>
//实现降序
void sort(T *a, int n){
    for (int i=0;i<n;i++){
        int p=i;
        for (int j=i;j<n;j++)
            if(a[p]<a[j])
                p=j;
        T t=a[i];	
        a[i]=a[p];		
        a[p]=t;
    }
}
template <class T>
void display(T& a,int n) {
    for(int i=0;i<n;i++)
        cout<<a[i]<<"\t"<<endl;
}


int main(int argc, char const *argv[])
{
    int a[]={1,41,2,5,8,21,23};
    char b[]={'a','x','y','e','q','g','o','u'};
    sort(a,7);
    sort(b,8);
    display(a,7);
    display(b,8);
    system("pause");
    return 0;
}
