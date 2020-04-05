
//【例12-2】  用函数get和getline读取数据。
#include <iostream>
using namespace std;
//cin.get(arrayname,size)  把字符输入到arrayname中，长度不超过size
int main()
{
    //get()两个参数

    //1.输入串长<size，输入串长>arraylength，会自动扩张arrayname大小，使能保存所有数据
	// char a[10];
	// cin.get(a,20);
	// cout<<a<<endl;
    // cout<<sizeof(a)<<endl;
    //2.输入串长<size，输入串长<arraylength，把串全部输入，后面补‘\0’
    // char b[10];
	// cin.get(b,20);
	// cout<<b<<endl;//12345，此时数组内数据为‘12345'\0’
    // cout<<sizeof(b)<<endl;
    //3.输入串长>size，先截取size个字符，若还是大于arraylength，则输入前arraylength-1个字符，最后补充‘\0’
    // char c[5];
	// cin.get(c,10);
	// cout<<c<<endl;
    // cout<<sizeof(c)<<endl;
    //4.输入串长>size，先截取size个字符，若小于arraylength，则把截取串放入数组中，最后补充‘\0’
    // char d[10];
	// cin.get(d,5);
	// cout<<d<<endl;
    // cout<<sizeof(d)<<endl;
    
    //get()三个参数
    /*
        用法：cin.get(arrayname,size,s) ?把数据输入到arrayname字符数组中，当到达长度size时结束或者遇到字符s时结束
        注释：a必须是字符数组，即char a[]l类型，不可为string类型；size为最大的输入长度；s为控制，遇到s则当前输入结束缓存区里的s将被舍弃

    */
    int i;
    char e[10];
    cin.get(e,8,',');
    cout<<e;
    system("pause");
    return 0;
}

