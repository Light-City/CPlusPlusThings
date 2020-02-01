//
// Created by light on 20-1-31.
//

#include <iostream>
#include <chrono>

using namespace std::chrono;
using namespace std;

using ull = unsigned long long;
ull OddSum = 0;
ull EvenSum = 0;

void findEven(ull start, ull end) {
    for (ull i = start; i <= end; ++i)
        if ((i & 1) == 0)
            EvenSum += i;
}

void findOdd(ull start, ull end) {
    for (ull i = start; i <= end; ++i)
        if ((i & 1) == 1)
            OddSum += i;
}

int main() {

    ull start = 0, end = 1900000000;

    auto startTime = high_resolution_clock::now();
    findOdd(start, end);
    findEven(start, end);
    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);

    cout << "OddSum : " << OddSum << endl;
    cout << "EvenSum: " << EvenSum << endl;
    cout << "Sec: " << duration.count() / 1000000 << endl;
    return 0;
}
