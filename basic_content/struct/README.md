# 一文搞懂C和C++中struct

## 关于作者：

个人公众号：

![](../img/wechat.jpg)

## 1.C中struct

- 在C中struct只单纯的用作数据的复合类型，也就是说，在结构体声明中只能将数据成员放在里面，而不能将函数放在里面。 
- 在C结构体声明中不能使用C++访问修饰符，如：public、protected、private 而在C++中可以使用。
- 在C中定义结构体变量，如果使用了下面定义必须加struct。
- C的结构体不能继承（没有这一概念）。
- 若结构体的名字与函数名相同，可以正常运行且正常的调用！例如：可以定义与 struct Base 不冲突的 void Base() {}。

完整案例：

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

最后输出：

```
1
I am Base func
```

完整代码见：[struct_func.c](./struct_func.c)

## 2.C++中struct

与C对比如下：

- C++结构体中不仅可以定义数据，还可以定义函数。
- C++结构体中可以使用访问修饰符，如：public、protected、private 。
- C++结构体使用可以直接使用不带struct。
- C++继承
- 若结构体的名字与函数名相同，可以正常运行且正常的调用！但是定义结构体变量时候只用用带struct的！

例如：

> 情形1：不适用typedef定义结构体别名

未添加同名函数前：

```c++
struct Student {
    
};
Student(){}
Struct Student s; //ok
Student s;  //ok
```

添加同名函数后：

```c++
struct Student {
    
};
Student(){}
Struct Student s; //ok
Student s;  //error
```

> 情形二：使用typedef定义结构体别名

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

> 前三种案例

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

完整代码见：[struct_func.cpp](struct_func.cpp)

> 继承案例

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

完整代码见：[ext_struct_func.cpp](./ext_struct_func.cpp)

> 同名函数

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
完整代码见：[struct_func_func.cpp](./struct_func_func.cpp)

## 3.总结

### C和C++中的Struct区别

| C                                                      | C++                                                          |
| ------------------------------------------------------ | ------------------------------------------------------------ |
| 不能将函数放在结构体声明                               | 能将函数放在结构体声明                                       |
| 在C结构体声明中不能使用C++访问修饰符。                 | public、protected、private 在C++中可以使用。                 |
| 在C中定义结构体变量，如果使用了下面定义必须加struct。  | 可以不加struct                                               |
| 结构体不能继承（没有这一概念）。                       | 可以继承                                                     |
| 若结构体的名字与函数名相同，可以正常运行且正常的调用！ | 若结构体的名字与函数名相同，使用结构体，只能使用带struct定义！ |
