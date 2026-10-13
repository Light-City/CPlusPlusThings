> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp11/std-bind-vs-lambda/) 的文章，由站点同步脚本导出。

# C++ std::bind 与 lambda 对比

https://quick-bench.com/q/CEnR_XUuIwnZxaP4KJNSlpYG38c

```
#include <functional>
#include <numeric>
#include <vector>



struct Calculator {
    int count{1};
    int total{0};

    auto calculate() -> void {
        using std::iota, std::vector;

        // Random math
        vector<int> u(8), v(Calculator::count * u.size());

        iota(v.begin(), v.end(), *u.begin());

        int sum{*v.begin() + *u.begin()};

        for (size_t pos{0}; pos < v.size(); ++pos) {
            sum += u[pos % u.size()] * v[pos];
        }

        Calculator::total += sum;
        ++Calculator::count;
    }
};

// auto f(std::function<void(int)> g) -> void { g(rand()); }

template<typename FN> auto f(FN g) -> void { g(rand()); }


static auto BM_bind(benchmark::State& state) -> void {
    Calculator calculator;
    for (auto _ : state) {
        f(std::bind(&Calculator::calculate, &calculator));
    }
}

static auto BM_lambda(benchmark::State& state) -> void {
    Calculator calculator;
    for (auto _ : state) {
        f([&calculator]([[maybe_unused]] int x) { calculator.calculate(); });
    }
}

BENCHMARK(BM_bind);
BENCHMARK(BM_lambda);
```