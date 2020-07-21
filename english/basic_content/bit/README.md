## About Author：


![](../img/wechat.jpg)

## What is Bit field ？

“ Bit field is a kind of data structure.Data can be stored compactly in bits, And allows the programmer to operate on the bits of this structure. One of the advantages of this data structure is that it can save storage space in data units, which is particularly important when programs need thousands of data units. The second advantage is that bit segments can easily access part of the contents of an integer value, which can simplify the program source code. The disadvantage of this data structure is that bit segment implementation depends on specific machines and systems, and different results may be obtained in different platforms, which leads to the fact that bit segments are not portable in nature

- The layout of bit fields in memory is machine dependent
- The type of bit field must be integer or enumeration type. The behavior of bit field in signed type depends on the implementation
- The addressing operator (&) cannot be applied to a bit field, and no pointer can point to a bit field of a class

## Bit field usage

Bit fields usually use struct declarations, which set the name of each bit field member and determine its width:

```
struct bit_field_name
{
	type member_name : width;
};
```



| Elements       | Description                                                  |
| -------------- | ------------------------------------------------------------ |
| bit_field_name | Bit field structure name                                     |
| type           | must be int、signed int or unsigned int type                 |
| member_name    |                                                              |
| width          |                                                              |

For example, declare the following bit field:

```
struct _PRCODE
{
	unsigned int code1: 2;
	unsigned int cdde2: 2;
	unsigned int code3: 8;
};
struct _PRCODE prcode;
```

This definition makes' prcode 'contain two 2-bit fields and one 8-bit field. We can use the member operator of the structure to assign values to it

```
prcode.code1 = 0;
prcode.code2 = 3;
procde.code3 = 102;
```

When assigning a value, it should be noted that the size of the value should not exceed the capacity of the bit field member，For example prcode.code3 Is a bit domain member of 8 bits. Its capacity is 2^8 = 256，Assignment range should be [0,255]。

## Size and alignment of bit fields

### Size of bit field

For example, the following bit field：

```
struct box 
{
	unsigned int a: 1;
	unsigned int  : 3;
	unsigned int b: 4;
};
```
There is an unnamed bit field in the middle of the bit field structure, which occupies 3 bits and only plays a filling role and has no practical significance. The padding makes the structure use 8 bits in total. But C language uses unsigned int as the basic unit of bit field，Even if the only member of a structure is a bit field of 1 bit, the size of the structure is the same as that of an unsigned int.In some systems, the unsigned int is 16 bits, and in x86 systems it is 32 bits.For the following articles, the default value of unsigned int is 32 bits.

### Alignment of bit fields

A bit field member is not allowed to cross the boundary of two unsigned ints. If the total number of bits declared by a member exceeds the size of an unsigned int, the editor will automatically shift the bit field members to align them according to the boundary of the unsigned int

For example：

```
struct stuff 
{
	unsigned int field1: 30;
	unsigned int field2: 4;
	unsigned int field3: 3;
};
```



`field1` + `field2` = 34 Bits，beyond 32 Bits, Complier`field2` move to the next unsigned int unit. stuff.field1 and stuff.field2 will leave the 2 Bits space， stuff.field3 follow closely stuff.field2.The structure is now of size 2 * 32 = 64 Bits。

This hole can be filled with the unnamed bit field member mentioned earlier, or we can use an unnamed bit field member with a width of 0 to align the next field member with the next integer.
For example:

```
struct stuff 
{
	unsigned int field1: 30;
	unsigned int       : 2;
	unsigned int field2: 4;
	unsigned int       : 0;
	unsigned int field3: 3; 
};
```



Between stuff.field1 and stuff.field2 there is a 2 Bits space. Stuff.field3 will be stored in the next unsigned in. The size of this structure is 3 * 32 = 96 Bits。

Code：[learn.cpp](learn.cpp)

## Initialization of bit field and remapping of bit

### Initialization


The initialization method of bit field is the same as that of ordinary structure. Here, two methods are listed as follows:

```
struct stuff s1= {20,8,6};
```

Or directly assign values to the bit field members:

```
struct stuff s1;
s1.field1 = 20;
s1.field2 = 8;
s1.field3 = 4;
```

### Re-mapping of bit field

Declare a bit field with a size of 32 bits

```
struct box {
	unsigned int ready:     2;
	unsigned int error:     2;
	unsigned int command:   4;
	unsigned int sector_no: 24;
}b1;
```

#### Zeroing bit field by remapping

```
int* p = (int *) &b1;  // 将 "位域结构体的地址" 映射至 "整形（int*) 的地址" 
*p = 0;                // clear s1, set all members to zero
```

#### The 32 bits bit field is remapped to the unsigned int type by union

Let's briefly introduce the alliance

> "Union" is a special class and a data structure of construction type.Many different data types can be defined in a "union". Any kind of data defined by the "union" can be loaded into a variable described as the "union" type. These data share the same memory to save space
>
> There are some similarities between "union" and "structure". But there are essential differences between them.In a structure, each member has its own memory space. The total length of a structure variable is the sum of the length of each member (except for empty structure, and boundary adjustment is not considered).In Union, members share a certain amount of memory space, and the length of a union variable is equal to the longest length of each member.It should be noted that the so-called sharing does not mean that multiple members are loaded into a joint variable at the same time, but that the joint variable can be assigned any member value, but only one value can be assigned at a time.

We can declare the following Union:

```
union u_box {
  struct box st_box;
  unsigned int ui_box;
};
```



In x86 system, unsigned int and box are 32 Bits, Through this combination, St_ Box and UI_ Box shares a piece of memory.Which bit in the specific bit field corresponds to which bit of the unsigned int depends on the compiler and hardware.Use union to return the bit field to zero. The code is as follows:

```
union u_box u;
u.ui_box = 0;
```

> From：<http://www.yuan-ji.me/C-C-%E4%BD%8D%E5%9F%9F-Bit-fields-%E5%AD%A6%E4%B9%A0%E5%BF%83%E5%BE%97/>
