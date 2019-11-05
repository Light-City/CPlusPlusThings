#include<iostream>
#include<stdio.h>
using namespace std;
struct Base {         
    int v1;
//    private:   //error!
        int v3;
    public:   //显示声明public
        int v2;
    virtual void print(){       
        printf("%s\n","Base");
    };    
    Base(){cout<<"Base construct"<<endl;};
    virtual ~Base(){cout<<"Base deconstruct"<<endl;};
};

struct Derived:Base {         

    Derived(){cout<<"Derived construct"<<endl;};
    virtual ~Derived(){cout<<"Derived deconstruct"<<endl;};
    public:
        int v2;
    void print(){       
        printf("%s\n","Derived");
    };    
};

int main() {
    Base *b=new Derived();
    b->print();
    delete b;
    return 0;
}
