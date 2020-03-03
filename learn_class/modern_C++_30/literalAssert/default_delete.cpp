//
// Created by light on 19-12-25.
//

class myFun {
public:
    myFun() = default;

    myFun(const myFun &) = default;

    myFun &operator=(const myFun &) = default;

    myFun(myFun &&) = delete;

    myFun &operator=(myFun &&) = delete;

    ~myFun() = default;
};