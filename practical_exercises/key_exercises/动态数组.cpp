#include<iostream>
#include<cstring>


int main() {
  
  char *sPtr;        	 
  const char* s="hello";
  sPtr = new char[strlen(s)+1];
  strncpy(sPtr,s,strlen(s)); 
  std::cout << sPtr << std::endl;
  delete sPtr;
  return  0;
}
