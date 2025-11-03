#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

class Person {
public:
    std::string name;
    int age;

    Person(const std::string& name, int age) : name(name), age(age) {
        std::cout << "Person constructor called: " << name << std::endl;
    }

    ~Person() {
        std::cout << "Person destructor called: " << name << std::endl;
    }

    void greet() const {
        std::cout << "Hello, my name is " << name << " and I'm " << age << " years old." << std::endl;
    }
};

std::unique_ptr<Person> unique_ptr_example() {
    std::cout << "\n--- unique_ptr Example ---" << std::endl;
    std::unique_ptr<Person> alice = std::make_unique<Person>("Alice", 30);
    alice->greet();
    std::cout << "unique_ptr alice owns the object: " << (alice != nullptr) << std::endl;

    std::unique_ptr<Person> bob = std::move(alice);
    bob->greet();
    std::cout << "unique_ptr alice owns the object: " << (alice != nullptr) << std::endl;
    std::cout << "unique_ptr bob owns the object: " << (bob != nullptr) << std::endl;

    return bob;
}

void shared_ptr_example() {
    std::cout << "\n--- shared_ptr Example ---" << std::endl;
    std::shared_ptr<Person> charlie = std::make_shared<Person>("Charlie", 25);
    std::cout << "Reference count: " << charlie.use_count() << std::endl;

    {
        std::shared_ptr<Person> dave = charlie;
        std::cout << "Reference count: " << charlie.use_count() << std::endl;
        dave->greet();
    }

    std::cout << "Reference count after dave goes out of scope: " << charlie.use_count() << std::endl;
    charlie->greet();
}

void weak_ptr_example() {
    std::cout << "\n--- weak_ptr Example ---" << std::endl;
    std::shared_ptr<Person> eve = std::make_shared<Person>("Eve", 35);
    std::weak_ptr<Person> weak_eve = eve;
    std::cout << "Reference count: " << eve.use_count() << std::endl;
    std::cout << "weak_ptr is expired: " << weak_eve.expired() << std::endl;

    if (auto shared_eve = weak_eve.lock()) {
        shared_eve->greet();
        std::cout << "Reference count after locking weak_ptr: " << eve.use_count() << std::endl;
    }

    eve.reset();
    std::cout << "weak_ptr is expired: " << weak_eve.expired() << std::endl;

    if (auto shared_eve = weak_eve.lock()) {
        shared_eve->greet();
    } else {
        std::cout << "weak_ptr points to an expired object." << std::endl;
    }
}

int main() {
    auto bob = unique_ptr_example();
    shared_ptr_example();
    weak_ptr_example();

    std::cout << "\n--- End of Program ---" << std::endl;
    // bob will be destroyed here, calling the destructor of Person
    return 0;
}
