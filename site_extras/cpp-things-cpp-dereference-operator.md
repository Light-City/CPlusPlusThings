> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-dereference-operator/) 的文章，由站点同步脚本导出。

# C++ 解引用运算符 *

## 0.导语

通常我们在操作指针时，会使用到`*`，例如：

```cpp
int a = 1;
int *ptr = a;
std::cout << *ptr << std::endl;
```

除此之外，还有什么用途？

答案是肯定的，例如，在C++中还有下面三类：

- `::*`
- `.*`
- `->*`

那么你知道这三个分别表示什么吗？

## 1.::*

`::*`被称之为**Pointer to member declarator**，指向成员声明符的指针。

举个例子吧：

下面这个例子你如何区w_的地址呢？

如果你写成`double * d = &w_`; 它便是无效的，因为不知道`w_`在哪里哦，所以需要指向成员声明符的指针，这样便可以进行绑定，有了这个指针之后，便可以访问成员了，继续讲后面两个。

```cpp
class Rectangle {
 public:
  double w_, h_;

  Rectangle(double w, double h) : w_(w), h_(h) {}
  double area() { return w_ * h_; }
};

int main() {
  Rectangle r{2.1, 3.2};
  double Rectangle::* w = &Rectangle::w_;
  r.*w = 2.2;
  std::cout << r.w_ << std::endl;
};
```

## 2..*

`.*`被称为pointer to member of object，这个比较好理解，就是访问**对象**成员的指针，指向成员的指针允许你引用类对象的非静态成员。不能使用指向成员的指针指向静态类成员，因为静态成员的地址与任何特定对象都没有关联。

所以看上面的例子中有了这一行代码，修改成员w的值为2.2。

```cpp
r.*w = 2.2;
```

## 3.->*

`->*`被称为pointer to member of pointer，这个就更好理解了，跟.*类似，不过操作的是**指针对象**成员的地址，

所以对应的例子变为：

```cpp
Rectangle r{2.1, 3.2}, *pr;
pr = &r;
std::cout << pr->*w << std::endl;
```

更加详细的请阅读cppreference.com。

> https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_members

你学会了吗？本节完~