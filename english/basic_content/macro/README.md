# Story about Macro



![](../img/wechat.jpg)

## 1.The macro contains special symbols

Several type：`#`，`##`，`\`

### 1.1 String operator（#）

**Using a # before macro parameter,The preprocessor converts this parameter into an array of characters**，In other words：**# is “stringlize”，The#, which appears in the macro definition, is to convert the following parameter into a string

**。

**Attention：It can only be used in macro definitions that have passed in parameters, and must be placed before the parameter name in the macro definition body.**

For example：

```c++
#define exp(s) printf("test s is:%s\n",s)
#define exp1(s) printf("test s is:%s\n",#s)
#define exp2(s) #s 
int main() {
    exp("hello");
    exp1(hello);

    string str = exp2(   bac );
    cout<<str<<" "<<str.size()<<endl;
    /**
     * Ignore spaces before and after the passed in parameter name
     */
    string str1 = exp2( asda  bac );
    /**
     * 当传入参数名间存在空格时，编译器将会自动连接各个子字符串，
     * 用每个子字符串之间以一个空格连接，忽略剩余空格。
     */
    cout<<str1<<" "<<str1.size()<<endl;
    return 0;
}
```

The above code gives the basic use and space handling rules，The space handling rules are as follows：

- Ignore spaces before and after the passed in parameter name

```c++
string str = exp2(   bac );
cout<<str<<" "<<str.size()<<endl;
```

Output：

```
bac 3
```

- When there are spaces between the input parameter names, the compiler will automatically connect each substring with a space between each substring, ignoring the remaining spaces.

```c++
string str1 = exp2( asda  bac );
cout<<str1<<" "<<str1.size()<<endl;
```

Output：

```
asda bac 8
```

### 1.2 Symbolic join operator（##）

**“##” It's a separate connection. Its function is to separate and then force the connection.Converts multiple parameters defined by a macro to an actual parameter name.**

Attention：

**（1）When use ## connecting parameters，##The space before and after is optional**

**（2）Actual parameter name after connection，Must be an actual parameter name or a macro definition known to the compiler**

**（3）If ## the parameter itself is a macro，## will prevent the macro from expanding.**

ex：

```c++

#define expA(s) printf("前缀加上后的字符串为:%s\n",gc_##s)  //gc_s必须存在
// 注意事项2
#define expB(s) printf("前缀加上后的字符串为:%s\n",gc_  ##  s)  //gc_s必须存在
// 注意事项1
#define gc_hello1 "I am gc_hello1"
int main() {
    // 注意事项1
    const char * gc_hello = "I am gc_hello";
    expA(hello);
    expB(hello1);
}
```

### 1.3 Continuation operator（\） 

**When the defined macro cannot be expressed completely in one line, you can use "\" to indicate the next line to continue the macro definition **

**Leave a space before \ **

```c++
#define MAX(a,b) ((a)>(b) ? (a) \
   :(b))  
int main() {
    int max_val = MAX(3,6);
    cout<<max_val<<endl;
}
```

From：[sig_examp.cpp](sig_examp.cpp)

## 2.do{...}while(0)

### 2.1 Avoid semantic misinterpretation

Such as ：

```
#define fun() f1();f2();
if(a>0)
	fun()
```

When this macro is expanded, it will be：

```
if(a>0)
	f1();
	f2();
```


In order to solve this problem, when writing code, usually can adopt

`{}`。

ex：

```c++
#define fun() {f1();f2();}
if(a>0)
	fun();
// 宏展开
if(a>0)
{
    f1();
    f2();
};
```

However, you will find that there is a semicolon after the macro is expanded, so the actual syntax is not correct.(Although the compiler runs well, there is no semicolon).

### 2.2 Avoid using goto to control flow

In some functions, we may need to do some cleaning before the return statement, such as releasing the memory space requested by malloc at the beginning of the function. Using goto is always a simple method:


```c++
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
```

However, because go to does not conform to the structure of software engineering and may make the code difficult to understand, many people do not advocate using goto. At this time, we can use do {...} while (0) to do the same thing


```c++
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
```

The main part of function using do{...}while(0),using break instead of goto.The subsequent cleaning is after while. Now we can achieve the same effect, and the readability and maintainability of the code are better than the goto code above

### 2.3 Avoid warnings caused by macros


Due to the limitation of different architectures in the kernel，Empty macros are often used. While compiling，These empty macros give warning. In order to avoid warning，we could use do{...}while(0) to define empty macro：

```
#define EMPTYMICRO do{}while(0)
```

### 2.4 **Define a single function block to perform complex operations**

If you have a complex function. There are many variables，And you don't want to add new functions。You could use do {...}while(0)，Write your code in it. You can define variables without considering the repetition of variable names and functions.
This should mean that a variable is used in multiple place(But the meaning of each is different)，We can narrow down the scope in each do while，for example：

```c++
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
```

From：[do_while.cpp](do_while.cpp)

Article：<https://www.cnblogs.com/lizhenghn/p/3674430.html>

