# Understand C and C + + struct



![](../img/wechat.jpg)

## 1.Struct In C

- In C, struct is only used as the composite type of data, that is, in the structure declaration, only data members can be placed inside, but not functions 
- C + + access modifiers cannot be used in C structure declarations,such as：public、protected、private. But they can be used in C + +
- Define the structure variable in C. If you use the following definition, you must add struct
- The structure of C cannot be inherited (there is no such concept)
- If the structure name is the same as the function name, it can run normally and call normally! For example, you can define void base() that does not conflict with struct base{}。

Example：

```c
#include<stdio.h>

struct Base {            // public
    int v1; 
//    public:      //error
        int v2; 
    //private:
        int v3; 
    //void print(){       // c中不能在结构体中嵌入函数
    //    printf("%s\n","hello world");
    //};    //error!
};

void Base(){
    printf("%s\n","I am Base func");
}
//struct Base base1;  //ok
//Base base2; //error
int main() {
    struct Base base;
    base.v1=1;
    //base.print();
    printf("%d\n",base.v1);
    Base();
    return 0;
}
```

Finally Output：

```
1
I am Base func
```

Code：[struct_func.c](./struct_func.c)

## 2.C++ struct

The comparison with C is as follows：

- In C + + structure, not only data can be defined, but also functions can be defined.
- Access modifiers can be used in C + + structures, such as ：public、protected、private 。
- C + + structure can be used directly without struct.
- C + + inheritance
- If the structure name is the same as the function name, it can run normally and call normally！However, when defining the structure variable, we only use the structure with struct！

Example：

> Case 1：

Before adding a function with the same name：

```c++
struct Student {
    
};
Student(){}
Struct Student s; //ok
Student s;  //ok
```

After adding a function with the same name：

```c++
struct Student {
    
};
Student(){}
Struct Student s; //ok
Student s;  //error
```

> Case 2：

```c++
typedef struct Base1 {         
    int v1;
//    private:   //error!
        int v3;
    public:     //显示声明public
        int v2;
    void print(){       
        printf("%s\n","hello world");
    };    
}B;
//void B() {}  //error! 符号 "B" 已经被定义为一个 "struct Base1" 的别名
```

> The first three cases

```c++
#include<iostream>
#include<stdio.h>

struct Base {         
    int v1;
//    private:   //error!
        int v3;
    public:     //显示声明public
        int v2;
    void print(){       
        printf("%s\n","hello world");
    };    
};

int main() {
    struct Base base1;  //ok
    Base base2; //ok
    Base base;
    base.v1=1;
    base.v3=2;
    base.print();
    printf("%d\n",base.v1);
    printf("%d\n",base.v3);
    return 0;
}
```

Code ：[struct_func.cpp](struct_func.cpp)

> Inheritance cases

```c++
#include<iostream>
#include<stdio.h>
struct Base {         
    int v1;
//    private:   //error!
        int v3;
    public:   //显示声明public
        int v2;
    virtual void print(){       
        printf("%s\n","Base");
    };    
};
struct Derived:Base {         

    public:
        int v2;
    void print(){       
        printf("%s\n","Derived");
    };    
};
int main() {
    Base *b=new Derived();
    b->print();
    return 0;
}
```

Code：[ext_struct_func.cpp](./ext_struct_func.cpp)

> Functions with the same name

```c++
#include<iostream>
#include<stdio.h>

struct Base {         
    int v1;
//    private:   //error!
        int v3;
    public:     //显示声明public
        int v2;
    void print(){       
        printf("%s\n","hello world");
    };    
};

typedef struct Base1 {         
    int v1;
//    private:   //error!
        int v3;
    public:     //显示声明public
        int v2;
    void print(){       
        printf("%s\n","hello world");
    };    
}B;
void Base(){
    printf("%s\n","I am Base func");
}
//void B() {}  //error! 符号 "B" 已经被定义为一个 "struct Base1" 的别名
int main() {
    struct Base base;  //ok
    //Base base1;  // error!
    base.v1=1;
    base.v3=2;
    base.print();
    printf("%d\n",base.v1);
    printf("%d\n",base.v3);
    Base();
    return 0;
}
```
Code：[struct_func_func.cpp](./struct_func_func.cpp)

## 3.Conclusion

### The difference of structure between C and C + +

| C                                                      | C++                                                          |
| ------------------------------------------------------ | ------------------------------------------------------------ |
| You cannot place a function in a structure declaration | Function can be declared in structure                                       |
| C + + access modifiers cannot be used in C structure declarations.            | public、protected、private They can be used in C + +                 |
| Define the structure variable in C. If you use the following definition, you must add struct  | You can do without struct                            |
|Not inheritance                       | you could use inheritance                                                     |
| If the structure name is the same as the function name, it can run normally and call normally | If the structure name is the same as the function name, the structure can only be defined with struct |
