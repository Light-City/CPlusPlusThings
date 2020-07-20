# Reference and Pointer

## About Author：


![](../img/wechat.jpg)

## 1.References and pointers

Conclusion：

| Reference         | Pointer              |
| ------------ | ------------              |
| Must be initialized   | No initialization is allowed    |
| cannot be empty     | could be empty     |
| Target cannot be changed | could change target   |

> References must be initialized, and pointers can be uninitialized

When we define a reference, we must specify an initial value for it, but the pointer does not

```c++
int &r;    //Illegal, no initialization reference
int *p;    //It is legal, but p is a wild pointer. You should be careful when using it
```

> Reference cannot be null and pointer can be null

Since the reference cannot be null, we do not need to test its validity when using a reference. When using a pointer, we need to first judge whether the pointer is a null pointer, otherwise it may cause the program to crash.
```c++
void test_p(int* p)
{
  	if(p != null_ptr)    //
    	*p = 3;
    return;
}
void test_r(int& r)
{
    r = 3;    //
    return;
}
```

> References cannot change targets



The pointer can be changed at any time, but the reference can only point to the object pointed to during initialization, and cannot be changed.


```
int a = 1;
int b = 2;

int &r = a;    //
int *p = &a;   //

p = &b;        //
r = b;         //引
```

## 2.Reference

#### lvalue reference

General reference, which generally represents the identity of an object.


#### rvalue reference



An R-value reference is a reference that must be bound to an R-value (a temporary object, an object to be destroyed), and generally represents the value of an object.


An R-value reference is a reference that must be bound to an R-value (a temporary object, an object to be destroyed), and generally represents the value of an object.

- It eliminates unnecessary copy of objects when two objects interact, saves operation storage resources and improves efficiency
- It can define generic functions more concisely and clearly

#### Reference collapse

- `X& &`、`X& &&`、`X&& &` can be folded into `X&`
- `X&& &&` can be floded into `X&&`

The reference of C ++ **At the same time, it improves the security of memory operation and the beauty of semantics**。For example, the mandatory requirement of reference must be initialized, so that we don't have to judge whether the reference is empty when using the reference, which makes the code more concise and elegant, and avoids the situation of pointer flying all over the sky. In addition to this scenario, references are used for the following two scenarios：

> Reference type parameter

In general, we use const reference parameter as a read-only formal parameter. In this case, we can not only avoid parameter copy, but also get the same call method as value passing parameter.

```c++
void test(const vector<int> &data)
{
    //...
}
int main()
{
  	vector<int> data{1,2,3,4,5,6,7,8};
    test(data);
}
```

> Reference type return value

C++ Provides the ability to overload operators.The syntax of the overloaded operator is the same as that of the original operator.An example is the operator [] operator, which generally needs to return a reference object in order to be modified correctly.

```c++
vector<int> v(10);
v[5] = 10;    //[]Operator returns the reference, and then the corresponding element of vector can be modified
              //If[] operator do not return a reference but a pointer, the assignment statement needs to be written like this
*v[5] = 10;   // This way of writing is totally inconsistent with our understanding of the call of [], which is easy to be misunderstood
```

## 3.Performance gap between pointer and reference

Is there a performance gap between pointers and references？This kind of problem needs to enter the assembly level to have a look. Let's first write a test1 function, which uses pointers for parameter passing：

```c++
void test1(int* p)
{
    *p = 3;    //此处应该首先判断p是否为空，为了测试的需要，此处我们没加。
    return;
}
```

The assembly code corresponding to this code segment is as follows：

```c++
(gdb) disassemble 
Dump of assembler code for function test1(int*):
   0x0000000000400886 <+0>:  push   %rbp
   0x0000000000400887 <+1>:  mov    %rsp,%rbp
   0x000000000040088a <+4>:  mov    %rdi,-0x8(%rbp)
=> 0x000000000040088e <+8>:  mov    -0x8(%rbp),%rax
   0x0000000000400892 <+12>: movl   $0x3,(%rax)
   0x0000000000400898 <+18>: nop
   0x0000000000400899 <+19>: pop    %rbp
   0x000000000040089a <+20>: retq   
End of assembler dump.

```

The above code lines 1 and 2 are the field operation of parameter calling and saving；The third line is parameter passing. The first parameter of the function call is usually placed in the RDI register. This line of code writes the RDI register value (the value of pointer P) to the stack；Line 4 writes the value of P in the stack to the rax register；Line 5 is to write the immediate number 3 to the**Memory pointed to by the value of the rax register**.

Let's write out the reference C + + code segment test2 for parameter passing：

```c++
void test2(int& r)
{
    r = 3;    // do not need to judge whether reference is null.
    return;
}
```

This code corresponds to the following assembly code：

```c++
(gdb) disassemble 
Dump of assembler code for function test2(int&):
   0x000000000040089b <+0>:  push   %rbp
   0x000000000040089c <+1>:  mov    %rsp,%rbp
   0x000000000040089f <+4>:  mov    %rdi,-0x8(%rbp)
=> 0x00000000004008a3 <+8>:  mov    -0x8(%rbp),%rax
   0x00000000004008a7 <+12>: movl   $0x3,(%rax)
   0x00000000004008ad <+18>: nop
   0x00000000004008ae <+19>: pop    %rbp
   0x00000000004008af <+20>: retq   
End of assembler dump.

```

We find that the assembly code corresponding to test2 is exactly the same as that of test1.This shows that the C + + compiler compiles the pointer and reference into exactly the same machine code when compiling the program. Therefore, the reference in C + + is just a "syntax sugar" of pointer operation in C ++，In the underlying implementation, the C + + compiler implements these two operations in the same way.

## 3. Conclusion

The introduction of reference operation in C + + ensures the security and convenience of reference use and maintains the elegance of code under the condition of adding more restrictions on the use of reference. The use of reference can avoid the situation of "pointer flying all over the sky" to a certain extent, and it has a positive significance to improve the stability of the program. Finally, the underlying implementations of pointers and references are the same, and there is no need to worry about the performance gap between them.

From：<http://irootlee.com/juicer_pointer_reference/#>
