> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-idioms/lookup-table-idiom/) 的文章，由站点同步脚本导出。

# C++ 查表法惯用法

```cpp
#include <benchmark/benchmark.h>
#include <cmath>
#include <random>


const int HLL_COUNT = 16;


static float harmonic_mean_table(const uint64_t* data) {
    static const float harmonic_tables[16] = {
        1.0f / static_cast<float>(1L << 0),
        1.0f / static_cast<float>(1L << 1),
        1.0f / static_cast<float>(1L << 2),
        1.0f / static_cast<float>(1L << 3),
        1.0f / static_cast<float>(1L << 4),
        1.0f / static_cast<float>(1L << 5),
        1.0f / static_cast<float>(1L << 6),
        1.0f / static_cast<float>(1L << 7),
        1.0f / static_cast<float>(1L << 8),
        1.0f / static_cast<float>(1L << 9),
        1.0f / static_cast<float>(1L << 10),
        1.0f / static_cast<float>(1L << 11),
        1.0f / static_cast<float>(1L << 12),
        1.0f / static_cast<float>(1L << 13),
        1.0f / static_cast<float>(1L << 14),
        1.0f / static_cast<float>(1L << 15),
    };

    float harmonic_mean = 0.0f;
    for (int i = 0; i < HLL_COUNT; ++i) {
        harmonic_mean += harmonic_tables[data[i]];
    }

    return harmonic_mean;
}

static float harmonic_mean_calc(const uint64_t* data) {
    float harmonic_mean = 0.0f;
    for (int i = 0; i < HLL_COUNT; ++i) {
        harmonic_mean += powf(2.0f, -data[i]);
    }

    return harmonic_mean;
}


uint64_t global_data[HLL_COUNT];

static void CustomArguments(benchmark::internal::Benchmark* b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, HLL_COUNT - 1);

    for (int i = 0; i < HLL_COUNT; ++i) {
        global_data[i] = dis(gen);
    }
}

static void BM_HarmonicMeanTable(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(harmonic_mean_table(global_data));
    }
}
BENCHMARK(BM_HarmonicMeanTable)->Apply(CustomArguments);


static void BM_HarmonicMeanCalc(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(harmonic_mean_calc(global_data));
    }
}
BENCHMARK(BM_HarmonicMeanCalc)->Apply(CustomArguments);
```

https://quick-bench.com/q/KtYyI4wutSurcnCrEezhRrH3Oks

> https://github.com/StarRocks/starrocks/pull/16351

## std::inplace_merge vs std::merge

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <benchmark/benchmark.h>


std::vector<int> generateTestData(int size) {
    std::vector<int> vec(size);

    for (int i = 0; i < size / 2; ++i) {
        vec[i] = 2 * i + 1;
    }

    for (int i = size / 2; i < size; ++i) {
        vec[i] = 2 * (i - size / 2) + 2;
    }

    return vec;
}

static void BM_Merge(benchmark::State& state) {
    state.PauseTiming();
    std::vector<int> vec = generateTestData(state.range(0) * 2);
    std::vector<int> result(state.range(0) * 2);
    state.ResumeTiming();
    for (auto _ : state) {
        std::merge(vec.begin(), vec.begin() + state.range(0), vec.begin() + state.range(0), vec.end(), result.begin());
    }
}
BENCHMARK(BM_Merge)->Range(8, 8 << 10);

static void BM_InplaceMerge(benchmark::State& state) {
    state.PauseTiming();
    std::vector<int> vec = generateTestData(state.range(0) * 2);
    std::vector<int>::iterator middle = vec.begin() + state.range(0);
    state.ResumeTiming();

    for (auto _ : state) {
        std::inplace_merge(vec.begin(), middle, vec.end());
    }
}
BENCHMARK(BM_InplaceMerge)->Range(8, 8 << 10);
```

https://quick-bench.com/q/u1WQpxF-KxrvW2s4mrnPkC6ZvLw

> https://github.com/StarRocks/starrocks/pull/14609