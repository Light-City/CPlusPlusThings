


## 1.The Definition Of const

Const type is that people use type specifier **const** signiture type to demonstrate，const variables or objects can not be updated。

## 2. Effect Of Const

（1）define variable

```
const int a=100;
```

(2) check type

The difference between const variable and define variable: ~~<u>**Constants have types, which can be checked by the compiler; the define macro definition has no data type, it is just a simple string replacement, and cannot be checked for security
**</u>~~ Thanks for point this bug out.
> https://github.com/Light-City/CPlusPlusThings/issues/5

Only variables of type integer or enumeration are defined by `const`. In other cases, it is only a 'const' qualified variable and should not be confused with constants.

（3）prevent modification, protect and increase program robustness


```
void f(const int i){
    i++; //error!
}
```
（4）Save space and avoid unnecessary memory allocation

From the compile point of view, the variables can be defined by const only give the corresponding memory address. Meanwhile the #define gives an immediate number.

## 3.const object is the file local variable by default


<p><font style="color:red">Attention：not const will be set as extern by default.For the const variable to be accessible in other files, it must be explicitly specified as extern in the file

</font></p>
> Access of variables not modified by const in different files


```
// file1.cpp
int ext
// file2.cpp
#include<iostream>
/**
 * by 光城
 * compile: g++ -o file file2.cpp file1.cpp
 * execute: ./file
 */
extern int ext;
int main(){
    std::cout<<(ext+10)<<std::endl;
}

```

You can find that variables that are not modified by const do not need to be explicitly declared by extern! Const constants need to be explicitly declared extern and need to be initialized! Because constants cannot be modified after they are defined, they must be initialized

## 4.Define Variable

```
const int b = 10;
b = 0; // error: assignment of read-only variable ‘b’
const string s = "helloworld";
const int i,j=0 // error: uninitialized const ‘i’
```

There are two errors above. First, B is a constant and cannot be changed! Second: I is a constant and must be initialized! (since constants cannot be modified after definition, they must be initialized. )

## 5.Pointer and Const
There are four kinds of pointer related consts

```
const char * a; //A pointer to a const object, or a pointer to a constant.
char const * a; //same as above
char * const a; //Const pointer to the type object. Or constant pointer, const pointer.
const char * const a; //Const pointer to const object

```
Conclusion: If *const* is on the left of `*`, const is used to modify the variable that the pointer points to, that is, the pointer points to a constant. If *const* is on the right of `*`, then the const is used to modify the pointer itself, that is, the pointer itself is a constant.

The specific method is as follows:
(1) pointer to const:
```
const int *ptr;
*ptr = 10; //error
```
ptr is a pointer to a const object of type int. Const defines the int type, that is, the type of object PTR points to, not the PTR itself.Therefore, ptr do not need to be assigned an initial value. However, the value of the object in question cannot be modified by ptr. Besides, we can not use void `*` pointer to save the address of const object

```
const int p = 10;
const void * vp = &p;
void *vp = &p; //error

```

Another key point is: we allow the address of a non const object to be assigned to a pointer to a const object.

Example:
```
const int *ptr;
int val = 3;
ptr = &val; //ok
```
We can't change the val value through the ptr pointer, even if it points to a non const object

```
int *ptr1 = &val;
*ptr1=4;
cout<<*ptr<<endl;
```
We cannot modify the underlying object with a pointer to the const object

However, if the pointer points to a non const object, the object it refers to can be modified in other ways.

```
int *ptr1 = &val;
*ptr1=4;
cout<<*ptr<<endl;
```
Conclusion: For a pointer to a constant, the value of the object cannot be modified by the pointer. And You cannot use the void `*` pointer to save the address of the const object. You must use a pointer of type const void `*` to hold the address of the const object.

(2) Constant pointer

The const pointer must be initialized and the value of the const pointer cannot be modified.

```
#include<iostream>
using namespace std;
int main(){

    int num=0;
    int * const ptr=&num; //const指针必须初始化！且const指针的值不能修改
    int * t = &num;
    *t = 1;
    cout<<*ptr<<endl;
}

```

Modify the value pointed by ptr pointer, which can be modified by non const pointer

Finally, when the address of a const constant is assigned to ptr, an error will be reported because ptr points to a variable rather than const constant.

```
#include<iostream>
using namespace std;
int main(){
    const int num=0;
    int * const ptr=&num; //error! const int* -> int*
    cout<<*ptr<<endl;
}

```
If changed to const int `*`ptr or const int `*`const ptr, no errors have occurred.


(3) Constant pointer to a constant

```
const int p = 3;
const int * const ptr = &p; 
```

PTR is a const pointer and then points to a const object of type int

## 6. Using const in function
Return value of const decorated function

The meaning of const modifying ordinary variables and pointers is basically the same

（1）const int

```
const int func1();
```
It doesn't make sense, because the parameter return itself is assigned to other variables

（2）const int*

```
const int* func2();

```
The pointer points to the same content

（3）int *const

```
int *const func2();
```
The pointer itself is immutable

Const modifier function parameters.

The passed parameters and pointers themselves are immutable and meaningless in the function.


```
void func(const int var); // parameters are immutable
void func(int *const var); // point itself is immutable

```

(2) Parameter pointer refers to a constant and is immutable


```
void StringCopy(char *dst, const char *src);

```
SRC is the input parameter and DST is the output parameter. After adding const modifier to SRC, if the statement in the function body tries to change the contents of SRC, the compiler will point out the error

This is one of the effects of const.

(3) The parameter is a reference. In order to increase efficiency and prevent modification at the same time

```
void func(const A &a)
```

For parameters of Non internal data types, Functions such as void func (A, a) are bound to be inefficient. Because a temporary object of type A will be generated in the function body to copy parameter a, and the process of constructing, copying and destructing the temporary object will take time

To improve efficiency, you can change the function declaration to void func(A &a)，Because reference passing only uses aliases for parameters, there is no need to generate temporary objects

void func(A &a) drawback:
It is possible to change the parameter a, which we do not expect.
It's easy to solve this problem by adding const modifier

Finally, it will be : void func(const A &a).


Should void func (int x) be rewritten to void func(const int &x),in order to improve efficiency? That operation was so needless.
Because the internal data type parameters do not exist in the process of construction and deconstruction, and replication is very fast.

Conclusion:For the input parameters of non internal data type, the mode of "value passing" should be changed to "const reference passing", in order to improve the efficiency. 
For example, change void func (a a a) to void func (const A & A). It can not improve the efficiency and reduce the comprehensibility of the functionVoid func (int x) should not be changed to void func (const int & x).

So Two interview problems were solved:


(1)If a function needs to pass in a pointer, do you need to add const to the pointer? What is the difference between adding const at different positions of the pointer

(2)If the function to be written needs to pass in an instance of a complex type, what is the difference between the passed in value parameter or reference parameter, and when to add const to the passed reference parameter
## 7.Using const in Class
In a class, any function that does not modify data members should be declared as const type. When writing const member functions, if you accidentally modify data members or call other non const member functions, the compiler will point out errors, which will undoubtedly improve the robustness of the program.The member function described by const is called constant member function

Only constant member functions are qualified to operate constant or constant objects. Member functions without const keyword cannot be used to operate constant objects

For const member variables in a class, they must be initialized through the initialization list:

```
class Apple
{
private:
    int people[100];
public:
    Apple(int i); 
    const int apple_number;
};

Apple::Apple(int i):apple_number(i)
{

}
```
Const objects can only access const member functions, while non const objects can access any member functions, including const member functions

Such as:
```
//apple.cpp
class Apple
{
private:
    int people[100];
public:
    Apple(int i); 
    const int apple_number;
    void take(int num) const;
    int add(int num);
    int add(int num) const;
    int getCount() const;

};
//main.cpp
#include<iostream>
#include"apple.cpp"
using namespace std;

Apple::Apple(int i):apple_number(i)
{

}
int Apple::add(int num){
    take(num);
}
int Apple::add(int num) const{
    take(num);
}
void Apple::take(int num) const
{
    cout<<"take func "<<num<<endl;
}
int Apple::getCount() const
{
    take(1);
//    add(); //error
    return apple_number;
}
int main(){
    Apple a(2);
    cout<<a.getCount()<<endl;
    a.add(10);
    const Apple b(3);
    b.add(100);
    return 0;
}
//Compile： g++ -o main main.cpp apple.cpp
//Result
take func 1
2
take func 10
take func 100
```
The getCount () method invokes a add method, and the add method is not const modifier, so run the wrong report. In other words, const objects can only access const member functions. Add method also calls const modified take method, which proves that non const objects can access any member function, including const member function.
In addition, we also see an overloaded function of add, which also outputs two results, indicating that const objects call const member functions by default
In addition to the above initialization const constant, we can also use the following methods:
(1)Combining the definition of constants with static, i.e:
```
static const int apple_number
```
(2)Initialize outside:

```
const int Apple::apple_number=10;
```
Of course, if you use C + + 11 to compile, you can directly define the initialization, you can write:
```
static const int apple_number=10;
or
const int apple_number=10;
```
Both are supported in C + + 11.
When compiling, just add '- STD = C + + 11'.
Let's talk about static briefly:
In C + +, static static member variables cannot be initialized inside a class.It is only declared inside the class, and the definition must be outside the class definition body, usually initialized in the class's implementation file

Declared in the class:

```
static int ap;
```
Use in class implementation file
```
int Apple::ap=666
```
For this implement, C + + 11 cannot be declared and initialized.




















