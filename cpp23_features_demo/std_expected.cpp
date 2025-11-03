#include <iostream>
#include <expected>
#include <string>
#include <sstream>

// 尝试将字符串转换为整数，返回std::expected<int, std::string>
std::expected<int, std::string> string_to_int(const std::string& str) {
    std::istringstream iss(str);
    int value;
    if (iss >> value) {
        return value; // 成功，返回值
    } else {
        return std::unexpected("Failed to convert string to int: " + str); // 失败，返回错误
    }
}

int main() {
    // 测试成功的情况
    std::string valid_str = "123";
    auto result1 = string_to_int(valid_str);
    if (result1) {
        std::cout << "Successfully converted \"" << valid_str << "\" to " << *result1 << std::endl;
    } else {
        std::cout << "Error: " << result1.error() << std::endl;
    }

    // 测试失败的情况
    std::string invalid_str = "abc";
    auto result2 = string_to_int(invalid_str);
    if (result2) {
        std::cout << "Successfully converted \"" << invalid_str << "\" to " << *result2 << std::endl;
    } else {
        std::cout << "Error: " << result2.error() << std::endl;
    }

    // 使用value()方法获取值，如果失败则抛出异常
    try {
        int value = result1.value();
        std::cout << "Value from value(): " << value << std::endl;
    } catch (const std::bad_expected_access& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    // 使用or_else()方法处理错误
    result2.or_else([](std::string error) {
        std::cout << "or_else() error: " << error << std::endl;
        return std::expected<int, std::string>(std::unexpected(error));
    });

    // 使用transform()方法转换值
    auto result3 = result1.transform([](int value) {
        return value * 2;
    });
    if (result3) {
        std::cout << "Transformed value: " << *result3 << std::endl;
    }

    return 0;
}
