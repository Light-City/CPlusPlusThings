> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-restrict-optimization/) 的文章，由站点同步脚本导出。

# C++ restrict 与编译优化

## restrict性能优化

```cpp
#include <vector>
#include <memory>
#include <benchmark/benchmark.h>

#define LEN 1<<20
using AggregateDataPtr = char *;

struct AvgState {
  uint64_t numerator{0};
  uint64_t denominator{0};

  double divide() { return numerator / denominator; }
};

class Column {
public:
  std::vector<uint64_t> & data() {return data_; }

private:
  std::vector<uint64_t> data_;
};

template <class Derived>
class IAggregate {
public:
  void addBatch(size_t batch_size, AggregateDataPtr state, Column *args) __attribute__((noinline)) {
      for (size_t i = 0; i < batch_size; ++i) {
          static_cast<const Derived *>(this)->add(state, args, i);
      }
  }

  void addBatchWithoutOpt(size_t batch_size, AggregateDataPtr state, Column *args) __attribute__((noinline)) {
      for (size_t i = 0; i < batch_size; ++i) {
          static_cast<const Derived *>(this)->addWithoutOpt(state, args, i);
      }
  }
};

class AvgAggregator final : public IAggregate<AvgAggregator> {
public:
  static AvgState& data(AggregateDataPtr __restrict place) { return *reinterpret_cast<AvgState *>(place); }
  static AvgState& data_without_opt(AggregateDataPtr place) { return *reinterpret_cast<AvgState *>(place); }

  void add(AggregateDataPtr __restrict state, Column * args, uint64_t row_num) const {
    data(state).numerator += args[0].data()[row_num];
    ++(data(state).denominator);
  }

  void addWithoutOpt(AggregateDataPtr state, Column * args, uint64_t row_num) const {
    data_without_opt(state).numerator += args[0].data()[row_num];
    ++(data_without_opt(state).denominator);
  }
};

static void BM_loop_with_optimize(benchmark::State& state) {
    AvgAggregator avgAggregator;
    Column column;
    for (size_t i = 0 ; i < LEN; i++) {
        column.data().push_back(i);
    }
    Column arg0[] = {column};
    // Code inside this loop is measured repeatedly
    for(auto _ : state) {
        auto avgState = std::make_unique<AvgState>();
        avgAggregator.addBatch(LEN, reinterpret_cast<AggregateDataPtr>(avgState.get()), arg0);
    }
}

static void BM_loop_without_optimize(benchmark::State& state) {
    AvgAggregator avgAggregator;
    Column column;
    for (size_t i = 0 ; i < LEN; i++) {
        column.data().push_back(i);
    }
    Column arg0[] = {column};
    // Code inside this loop is measured repeatedly
    for(auto _ : state) {
        auto avgState = std::make_unique<AvgState>();
        avgAggregator.addBatchWithoutOpt(LEN, reinterpret_cast<AggregateDataPtr>(avgState.get()), arg0);
    }
}

BENCHMARK(BM_loop_without_optimize);
BENCHMARK(BM_loop_with_optimize);
```

> https://quick-bench.com/q/ccjuWr_JcLozAfy3Gxf3uVu76Yk

https://github.com/ClickHouse/ClickHouse/pull/19946