#include <iostream>
using namespace std;
class BasicException
{
public:
    virtual string Where() { return "BasicException..."; }
};
class FileSysException : public BasicException
{
public:
    virtual string Where() { return "FileSysException..."; }
};
class FileNotFound : public FileSysException
{
public:
    virtual string Where() { return "FileNotFound..."; }
};
class DiskNotFound : public FileSysException
{
public:
    virtual string Where() { return "DiskNotFound..."; }
};
int main()
{
    try
    {
        //        .....  //程序代码
        DiskNotFound err;
        throw &err;
    }
    catch (BasicException *p)
    {
        cout << p->Where() << endl;
    }
}
