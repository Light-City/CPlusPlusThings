//Eg12-7.cpp
#include <iostream>
#include <fstream>
using namespace std;
int main(){
    char ch;
    ofstream out("/test.dat",ios::out|ios::binary);		//L1
    for(int i=0;i<90;i++){
        if(i>0 && (i % 30)==0)
		    out.put('\n');						
        out.put(i);							
        out.put(' ');	
        						
	}
    out.close();							    
    ifstream in("/test.dat",ios::in|ios::binary);
    while(in.get(ch))
		cout<<ch;				
	in.close();		
    system("pause");					
}
