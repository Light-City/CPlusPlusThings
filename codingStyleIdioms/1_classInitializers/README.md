# 初始化列表与赋值

- const成员的初始化只能在构造函数初始化列表中进行
- 引用成员的初始化也只能在构造函数初始化列表中进行
- 对象成员（对象成员所对应的类没有默认构造函数）的初始化，也只能在构造函数初始化列表中进行

## 类之间嵌套

**第一种： 使用初始化列表。**


```cpp
class Animal {
public:
    Animal() {
        std::cout << "Animal() is called" << std::endl;
    }

    Animal(const Animal &) {
        std::cout << "Animal (const Animal &) is called" << std::endl;
    }

    Animal &operator=(const Animal &) {
        std::cout << "Animal & operator=(const Animal &) is called" << std::endl;
        return *this;
    }

    ~Animal() {
        std::cout << "~Animal() is called" << std::endl;
    }
};

class Dog {
public:
    Dog(const Animal &animal) : __animal(animal) {
        std::cout << "Dog(const Animal &animal) is called" << std::endl;
    }

    ~Dog() {
        std::cout << "~Dog() is called" << std::endl;
    }

private:
    Animal __animal;
};

int main() {
    Animal animal;
    std::cout << std::endl;
    Dog d(animal);
    std::cout << std::endl;
    return 0;
}
```

运行结果：
```cpp
Animal() is called

Animal (const Animal &) is called
Dog(const Animal &animal) is called

~Dog() is called
~Animal() is called
~Animal() is called
```

依次分析从上到下：

main函数中`Animal animal;`调用默认构造。

`Dog d(animal);`等价于：

```
Animal __animal = animal;
```
实际上就是调用了拷贝构造，因此输出了：
```
Animal (const Animal &) is called
```
再然后打印Dog的构造函数里面的输出。

最后调用析构，程序结束。

**第二种：构造函数赋值来初始化对象。**

构造函数修改如下：
```cpp
Dog(const Animal &animal) {
    __animal = animal;
    std::cout << "Dog(const Animal &animal) is called" << std::endl;
}
```
此时输出结果：
```
Animal() is called

Animal() is called
Animal & operator=(const Animal &) is called
Dog(const Animal &animal) is called

~Dog() is called
~Animal() is called
~Animal() is called
```

于是得出：

当调用`Dog d(animal);`时，等价于：

先定义对象，再进行赋值，因此先调用了默认构造，再调用=操作符重载函数。

```cpp
// 假设之前已经有了animal对象
Animal __animal;
__animal = animal;
```

> 小结

通过上述我们得出如下结论：

- **类中包含其他自定义的class或者struct，采用初始化列表，实际上就是创建对象同时并初始化**
- **而采用类中赋值方式，等价于先定义对象，再进行赋值，一般会先调用默认构造，在调用=操作符重载函数。**

## 无默认构造函数的继承关系中

现考虑把上述的关系改为继承，并修改Animal与Dog的构造函数，如下代码：

```cpp
class Animal {
public:
    Animal(int age) {
        std::cout << "Animal(int age) is called" << std::endl;
    }

    Animal(const Animal & animal) {
        std::cout << "Animal (const Animal &) is called" << std::endl;
    }

    Animal &operator=(const Animal & amimal) {
        std::cout << "Animal & operator=(const Animal &) is called" << std::endl;
        return *this;
    }

    ~Animal() {
        std::cout << "~Animal() is called" << std::endl;
    }
};

class Dog : Animal {
public:
    Dog(int age) : Animal(age) {
        std::cout << "Dog(int age) is called" << std::endl;
    }

    ~Dog() {
        std::cout << "~Dog() is called" << std::endl;
    }

};
```

上述是通过初始化列表给基类带参构造传递参数，如果不通过初始化列表传递，会发生什么影响？

去掉初始化列表
```
Dog(int age)  {
    std::cout << "Dog(int age) is called" << std::endl;
}
```
运行程序：

```
error: no matching function for call to ‘Animal::Animal()’
```

由于在Animal中没有默认构造函数，所以报错，遇到这种问题属于灾难性的，我们应该尽量避免，可以通过初始化列表给基类的构造初始化。


## 类中const数据成员、引用数据成员

特别是引用数据成员，必须用初始化列表初始化，而不能通过赋值初始化！

例如：在上述的Animal中添加私有成员，并修改构造函数：
```cpp
class Animal {
public:
    Animal(int age,std::string name) {
        std::cout << "Animal(int age) is called" << std::endl;
    }
private:
    int &age_;
    const std::string name_;
};
```

报下面错误：

```cpp
error: uninitialized reference member in ‘int&’
```

应该改为下面：

```cpp
Animal(int age, std::string name) : age_(age), name_(name) {
    std::cout << "Animal(int age) is called" << std::endl;
}
```

