# Things about decltype

## About Author：


![](../img/wechat.jpg)

## 1.Basic
The syntax of decltype is:

```
decltype (expression)
```

Brackets are essential here.The function of decltype is "the type of query expression".Therefore, the effect of the above statement is to return the type of the expression expression.Note that decltype only "queries" the type of the expression and does not "evaluate" the expression.


### 1.1 Deduce expression type

```
int i = 4;
decltype(i) a; //The result is int. The type of a is int
```

### 1.2 Used with using / typedef to define types.

```c++
using size_t = decltype(sizeof(0));//sizeof(a)的返回值为size_t类型
using ptrdiff_t = decltype((int*)0 - (int*)0);
using nullptr_t = decltype(nullptr);
vector<int >vec;
typedef decltype(vec.begin()) vectype;
for (vectype i = vec.begin; i != vec.end(); i++)
{
//...
}
```

like auto, improves the readability of the code.


### 1.3 Reuse anonymous types

In C + +, we sometimes encounter some anonymous types, such as:

```c++
struct 
{
    int d ;
    doubel b;
}anon_s;
```

With decltype, we can reuse this anonymous structure:
```c++
decltype(anon_s) as ;//Defines an anonymous structure above
```

### 1.4 In generic programming, auto is used to trace the return value type of function


```c++
template <typename T>
auto multiply(T x, T y)->decltype(x*y)
{
	return x*y;
}
```

Code：[decltype.cpp](decltype.cpp)

## 2.Discriminant rules



For decltype (E), the results are affected by the following conditions:
If e is a marker expression or class member access expression without parentheses, decltype (E) of is the type of entity named by E.In addition, if e is an overloaded function, it will result in compilation errors.
Otherwise, assume that the type of E is t, and if e is a dying value, then decltype (E) is t&&
Otherwise, assume that the type of E is t, and if e is a dying value, then decltype (E) is t&&
Otherwise, assuming that the type of E is t, decltype (E) is t.

Markers are defined by programmers except for keywords, literals and other tags that the compiler needs to use. The expression corresponding to a single marker is a marker expression. For example:

```c++
int arr[4]
```
Then arr is a marker expression，ranther than arr[3]+0

Example：

```c++
int i = 4;
int arr[5] = { 0 };
int *ptr = arr;
struct S{ double d; }s ;
void Overloaded(int);
void Overloaded(char);// reload function
int && RvalRef();
const bool Func(int);

//Rule 1: derivation is its type
decltype (arr) var1; //int 标记符表达式

decltype (ptr) var2;//int *  标记符表达式

decltype(s.d) var3;//doubel 成员访问表达式

//decltype(Overloaded) var4;//重载函数。编译错误。

//规则二：将亡值。推导为类型的右值引用。

decltype (RvalRef()) var5 = 1;

//规则三：Lvalue, derived as a reference to the type

decltype ((i))var6 = i;     //int&

decltype (true ? i : i) var7 = i; //int&  A conditional expression returns an lvalue

decltype (++i) var8 = i; //int&  ++i返回i的左值。

decltype(arr[5]) var9 = i;//int&. []操作返回左值

decltype(*ptr)var10 = i;//int& *操作返回左值

decltype("hello")var11 = "hello"; //const char(&)[9]  字符串字面常量为左值，且为const左值。


//Rule 4: if none of the above is true, this type is derived

decltype(1) var12;//const int

decltype(Func(1)) var13=true;//const bool

decltype(i++) var14 = i;//int i++返回右值
```

From：https://www.cnblogs.com/QG-whz/p/4952980.html
