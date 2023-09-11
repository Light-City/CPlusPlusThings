#include <iostream>
#include <vector>

// 观察者接口
class Observer {
public:
    virtual void update() = 0;
};

// 具体观察者类
class ConcreteObserver : public Observer {
public:
    void update() {
        std::cout << "ConcreteObserver: Received update notification." << std::endl;
    }
};

// 主题接口
class Subject {
public:
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notify() = 0;
};

// 具体主题类
class ConcreteSubject : public Subject {
private:
    std::vector<Observer*> observers;
public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    void detach(Observer* observer) {
        for (auto it = observers.begin(); it != observers.end(); ++it) {
            if (*it == observer) {
                observers.erase(it);
                break;
            }
        }
    }

    void notify() {
        for (auto observer : observers) {
            observer->update();
        }
    }
};

int main() {
    // 创建主题和观察者对象
    ConcreteSubject subject;
    ConcreteObserver observer1;
    ConcreteObserver observer2;

    // 将观察者对象附加到主题
    subject.attach(&observer1);
    subject.attach(&observer2);

    // 通知观察者
    subject.notify();

    // 从主题中分离一个观察者
    subject.detach(&observer1);

    // 再次通知观察者
    subject.notify();

    return 0;
}