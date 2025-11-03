#include <iostream>
#include <vector>
#include <mdspan>

// 打印多维数组
void print_mdspan(std::mdspan<int, std::dynamic_extent, std::dynamic_extent> m) {
    for (std::size_t i = 0; i < m.extent(0); ++i) {
        for (std::size_t j = 0; j < m.extent(1); ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// 矩阵乘法
std::vector<int> matrix_multiply(
    std::mdspan<int, std::dynamic_extent, std::dynamic_extent> a,
    std::mdspan<int, std::dynamic_extent, std::dynamic_extent> b) {
    if (a.extent(1) != b.extent(0)) {
        throw std::invalid_argument("Matrix dimensions mismatch");
    }

    std::size_t rows = a.extent(0);
    std::size_t cols = b.extent(1);
    std::size_t common = a.extent(1);
    std::vector<int> result(rows * cols);
    std::mdspan<int, std::dynamic_extent, std::dynamic_extent> res(result.data(), rows, cols);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            int sum = 0;
            for (std::size_t k = 0; k < common; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }

    return result;
}

int main() {
    // 创建一个3x3的二维数组
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::mdspan<int, std::dynamic_extent, std::dynamic_extent> m(data.data(), 3, 3);

    // 访问和修改元素
    std::cout << "Original matrix:" << std::endl;
    print_mdspan(m);

    m[1][1] = 10; // 修改中间元素
    std::cout << "Matrix after modification:" << std::endl;
    print_mdspan(m);

    // 创建一个3x2的二维数组
    std::vector<int> data2 = {1, 2, 3, 4, 5, 6};
    std::mdspan<int, std::dynamic_extent, std::dynamic_extent> m2(data2.data(), 3, 2);

    // 矩阵乘法
    std::cout << "Matrix multiplication:" << std::endl;
    std::cout << "Matrix A:" << std::endl;
    print_mdspan(m);
    std::cout << "Matrix B:" << std::endl;
    print_mdspan(m2);

    try {
        std::vector<int> result = matrix_multiply(m, m2);
        std::mdspan<int, std::dynamic_extent, std::dynamic_extent> res(result.data(), 3, 2);
        std::cout << "Result:" << std::endl;
        print_mdspan(res);
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
