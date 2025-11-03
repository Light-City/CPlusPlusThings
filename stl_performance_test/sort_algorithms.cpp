#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>

// 测试排序算法性能
template <typename SortFunc>
double test_sort(SortFunc sort_func, const std::vector<int>& data) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> copy = data;
    sort_func(copy);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {
    const int num_elements = 100000;
    const int num_tests = 5;

    // 生成随机测试数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);
    std::vector<int> data(num_elements);
    for (int i = 0; i < num_elements; ++i) {
        data[i] = dis(gen);
    }

    // 测试std::sort
    double sort_time = 0.0;
    for (int i = 0; i < num_tests; ++i) {
        sort_time += test_sort([](std::vector<int>& v) {
            std::sort(v.begin(), v.end());
        }, data);
    }
    sort_time /= num_tests;

    // 测试std::stable_sort
    double stable_sort_time = 0.0;
    for (int i = 0; i < num_tests; ++i) {
        stable_sort_time += test_sort([](std::vector<int>& v) {
            std::stable_sort(v.begin(), v.end());
        }, data);
    }
    stable_sort_time /= num_tests;

    // 测试std::partial_sort (排序前半部分)
    double partial_sort_time = 0.0;
    for (int i = 0; i < num_tests; ++i) {
        partial_sort_time += test_sort([](std::vector<int>& v) {
            std::partial_sort(v.begin(), v.begin() + v.size() / 2, v.end());
        }, data);
    }
    partial_sort_time /= num_tests;

    // 测试std::nth_element (找到中位数)
    double nth_element_time = 0.0;
    for (int i = 0; i < num_tests; ++i) {
        nth_element_time += test_sort([](std::vector<int>& v) {
            std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
        }, data);
    }
    nth_element_time /= num_tests;

    // 测试std::sort_heap
    double sort_heap_time = 0.0;
    for (int i = 0; i < num_tests; ++i) {
        sort_heap_time += test_sort([](std::vector<int>& v) {
            std::make_heap(v.begin(), v.end());
            std::sort_heap(v.begin(), v.end());
        }, data);
    }
    sort_heap_time /= num_tests;

    // 输出结果
    std::cout << "排序算法性能对比:" << std::endl;
    std::cout << "元素数量: " << num_elements << std::endl;
    std::cout << "测试次数: " << num_tests << std::endl;
    std::cout << std::endl;

    std::cout << "std::sort: " << sort_time << " 秒" << std::endl;
    std::cout << "std::stable_sort: " << stable_sort_time << " 秒" << std::endl;
    std::cout << "std::partial_sort (前半部分): " << partial_sort_time << " 秒" << std::endl;
    std::cout << "std::nth_element (中位数): " << nth_element_time << " 秒" << std::endl;
    std::cout << "std::sort_heap: " << sort_heap_time << " 秒" << std::endl;
    std::cout << std::endl;

    std::cout << "相对性能 (std::sort = 1.0):" << std::endl;
    std::cout << "std::sort: 1.0" << std::endl;
    std::cout << "std::stable_sort: " << stable_sort_time / sort_time << "x 慢" << std::endl;
    std::cout << "std::partial_sort: " << partial_sort_time / sort_time << "x 慢" << std::endl;
    std::cout << "std::nth_element: " << nth_element_time / sort_time << "x 慢" << std::endl;
    std::cout << "std::sort_heap: " << sort_heap_time / sort_time << "x 慢" << std::endl;

    return 0;
}
