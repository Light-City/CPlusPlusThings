#include<iostream>

using namespace std;
struct stuff 
{
    unsigned int field1: 30;
    unsigned int       : 2;
    unsigned int field2: 4;
    unsigned int       : 0;
    unsigned int field3: 3; 
};
int main(){
    struct stuff s={1,3,5};
    cout<<s.field1<<endl;
    cout<<s.field2<<endl;
    cout<<s.field3<<endl;
    cout<<sizeof(s)<<endl;
    return 0;
}
