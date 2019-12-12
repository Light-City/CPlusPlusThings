//
// Created by light on 19-12-9.
//

#include <iostream>

class Animal {
public:
    Animal(int age,std::string name):age_(age),name_(name) {
        std::cout << "Animal(int age) is called" << std::endl;
    }
private:
    int &age_;
    const std::string name_;
};

int main() {
    Animal animal(10,"hh");
    return 0;
}