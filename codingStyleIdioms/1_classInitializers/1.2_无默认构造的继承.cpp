//
// Created by light on 19-12-9.
//
#include <iostream>
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

int main() {
    Animal animal(10);
    std::cout << std::endl;
    Dog d(100);
    std::cout << std::endl;
    return 0;
}