#include <iostream>
#include <malloc.h>

using namespace std;
#define f1() cout<<"f1()"<<endl;
#define f2() cout<<"f2()"<<endl;

#define fun() {f1();f2();}
#define fun1() \
    do{ \
        f1();\
        f2();\
    }while(0)

int f() {
    int *p = (int *)malloc(sizeof(int));
    *p = 10;
    cout<<*p<<endl;
#ifndef DEBUG
    int error=1;
#endif
    if(error)
        goto END;

    // dosomething
END:
    cout<<"free"<<endl;
    free(p);
    return 0;
}

int ff() {

    int *p = (int *)malloc(sizeof(int));
    *p = 10;
    cout<<*p<<endl;

    do{
#ifndef DEBUG
        int error=1;
#endif
        if(error)
            break;
        //dosomething
    }while(0);

    cout<<"free"<<endl;
    free(p);
    return 0;
}


int fc()
{
    int k1 = 10;
    cout<<k1<<endl;
    do{
        int k1 = 100;
        cout<<k1<<endl;
    }while(0);    
    cout<<k1<<endl;
}

int main() {

    if(1>0)
        fun();

    if(2>0)
        fun1();

    f();
    ff();
    fc();
    return 0;
}
