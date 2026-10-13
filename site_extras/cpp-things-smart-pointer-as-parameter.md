> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/smart-pointer-as-parameter/) 的文章，由站点同步脚本导出。

# C++ 智能指针作函数参数

## C++那些事之智能指针何时作为函数参数

通常我们可以看到有些函数用了智能指针作为参数，有些函数又使用了裸指针，究竟用哪一个呢？使用原则是什么？

本节使用abseil的tips进行讲解。

如下所示，这段代码有什么问题呢？

```cpp
bool CanYouPetTheDog(const std::shared_ptr<Dog>& dog,
                     absl::Duration min_delay) {
  return dog->GetLastPetTime() + min_delay < absl::Now();
}
```

CanYouPetTheDog函数并不会影响其 dog 参数的所有权，但它的签名要求将其存储在 std::shared_ptr 中。这样做会在特定所有权模型上创建一个不必要的依赖，尽管函数本身并不需要这种依赖。这种依赖会阻止调用者使用其他模型，比如 std::unique_ptr 或在堆栈上构造对象。

## 使用指针/引用

当所有权没有受到影响时使用**引用或指针**。 通过使用引用/指针，我们可以消除对特定所有权模型的依赖，并允许我们的函数与任何类型为 Dog 的对象一起工作。

```cpp
bool CanYouPetTheDog(const Dog& dog, absl::Duration min_delay) {
  return dog.GetLastPetTime() + min_delay < absl::Now();
}
```

有了上述定义，无论调用者的所有权模型如何，都可以调用该函数：

```cpp
Dog stack_dog;
if (CanYouPetTheDog(stack_dog, delay)) { ... }

auto heap_dog = std::make_unique<Dog>();
if (CanYouPetTheDog(*heap_dog, delay)) { ... }

CustomPetPtr<Dog> custom_dog = CreateDog();
if (CanYouPetTheDog(*custom_dog, delay)) { ... }
```

函数修改了传递的值，则传递可变引用或原始指针，并使用与上述相同的用法。

## 使用智能指针

在函数修改所有权时使用智能指针，以下代码为不同的智能指针参数提供了几个重载。第一个重载假定接管传递对象的所有权，第二个重载为传递对象添加了一个共享引用。这两种操作取决于调用方如何处理 Dog 的所有权。无法接管位于栈的 Dog，因为无法从栈中取走所有权。

```cpp
class Human {
 public:
  ...
  void Adopt(std::unique_ptr<Dog> dog) {
    pets_.push_back(std::move(dog));
  }
  void Adopt(std::shared_ptr<Cat> cat) {
    pets_.push_back(std::move(cat));
  }

 private:
  std::vector<std::shared_ptr<Pet>> pets_;
  ...
};

```

## 总结

如果所有权没有被转移或修改，则应该避免将智能指针作为函数参数。

> https://abseil.io/tips/188