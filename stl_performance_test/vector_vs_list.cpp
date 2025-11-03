#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>

// 测试插入性能
template <typename Container>
double test_insert(Container& container, int num_elements) {
    auto start = std::chrono::high_resolution_clock::now();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_elements - 1);

    for (int i = 0; i < num_elements; ++i) {
        int value = dis(gen);
        container.push_back(value);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}

// 测试删除性能
template <typename Container>
double test_delete(Container& container, int num_elements) {
    auto start = std::chrono::high_resolution_clock::now();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, container.size() - 1);

    for (int i = 0; i < num_elements; ++i) {
        auto it = container.begin();
        std::advance(it, dis(gen));
        container.erase(it);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}

// 测试访问性能
template <typename Container>
double test_access(Container& container, int num_elements) {
    auto start = std::chrono::high_resolution_clock::now();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, container.size() - 1);

    for (int i = 0; i < num_elements; ++i) {
        int index = dis(gen);
        auto it = container.begin();
        std::advance(it, index);
        int value = *it;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}

int main() {
    const int num_elements = 10000;
    const int num_tests = 10;

    // 测试vector性能
    std::vector<int> vec;
    double vec_insert_time = test_insert(vec, num_elements);
    double vec_delete_time = test_delete(vec, num_elements / 10);
    double vec_access_time = test_access(vec, num_elements);

    // 测试list性能
    std::list<int> lst;
    double lst_insert_time = test_insert(lst, num_elements);
    double lst_delete_time = test_delete(lst, num_elements / 10);
    double lst_access_time = test_access(lst, num_elements);

    // 输出结果
    std::cout << "Performance comparison between vector and list:" << std::endl;
    std::cout << "Number of elements: " << num_elements << std::endl;
    std::cout << "Number of tests: " << num_tests << std::endl;
    std::cout << std::endl;

    std::cout << "Insertion time: " << std::endl;
    std::cout << "vector: " << vec_insert_time << " seconds" << std::endl;
    std::cout << "list: " << lst_insert_time << " seconds" << std::endl;
    std::cout << "vector is " << lst_insert_time / vec_insert_time << "x faster" << std::endl;
    std::cout << std::endl;

    std::cout << "Deletion time: " << std::endl;
    std::cout << "vector: " << vec_delete_time << " seconds" << std::endl;
    std::cout << "list: " << lst_delete_time << " seconds" << std::endl;
    std::cout << "list is " << vec_delete_time / lst_delete_time << "x faster" << std::endl;
    std::cout << std::endl;

    std::cout << "Access time: " << std::endl;
    std::cout << "vector: " << vec_access_time << " seconds" << std::endl;
    std::cout << "list: " << lst_access_time << " seconds" << std::endl;
    std::cout << "vector is " << lst_access_time / vec_access_time << "x faster" << std::endl;

    return 0;
}
