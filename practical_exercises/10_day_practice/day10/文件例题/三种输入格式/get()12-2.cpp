//【例12-2】  用函数get和getline读取数据。
#include <iostream>
using namespace std;
int main()
{
	char a,b,c,d;
	cin.get(a);
	cin.get(b);
	c = cin.get();
	d = cin.get();
	cout<<int(a)<<','<<int(b)<<','<<int(c)<<','<<int(d)<<endl;
    system("pause");
    return 0;
}

/*
用法：a = cin.get() ?或者 ?cin.get(a)
结束条件：输入字符足够后回车
说明：这个是单字符的输入，用途是输入一个字符，把它的ASCALL码存入到a中
处理方法：与cin不同，cin.get()在缓冲区遇到[enter]，[space]，[tab]不会作为舍弃，而是继续留在缓冲区中
*/