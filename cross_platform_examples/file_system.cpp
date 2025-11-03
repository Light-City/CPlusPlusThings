#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// 打印文件信息
void print_file_info(const fs::path& path) {
    if (fs::exists(path)) {
        std::cout << "Path: " << path << std::endl;
        std::cout << "Is directory: " << fs::is_directory(path) << std::endl;
        std::cout << "Is regular file: " << fs::is_regular_file(path) << std::endl;
        if (fs::is_regular_file(path)) {
            std::cout << "File size: " << fs::file_size(path) << " bytes" << std::endl;
        }
        std::cout << "Last write time: " << fs::last_write_time(path).time_since_epoch().count() << std::endl;
    } else {
        std::cout << "Path does not exist: " << path << std::endl;
    }
}

// 遍历目录
void traverse_directory(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_directory(path)) {
        std::cout << "Path is not a directory: " << path << std::endl;
        return;
    }

    std::cout << "Directory contents: " << path << std::endl;
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << "  " << entry.path().filename();
        if (fs::is_directory(entry.path())) {
            std::cout << "/";
        }
        std::cout << std::endl;
    }
}

// 创建目录
void create_directory(const fs::path& path) {
    try {
        if (fs::create_directory(path)) {
            std::cout << "Directory created: " << path << std::endl;
        } else {
            std::cout << "Directory already exists: " << path << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error creating directory: " << e.what() << std::endl;
    }
}

// 创建文件
void create_file(const fs::path& path, const std::string& content) {
    try {
        std::ofstream file(path);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "File created: " << path << std::endl;
        } else {
            std::cout << "Error creating file: " << path << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error creating file: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "--- Cross-platform File System Example ---" << std::endl;

    // 平台特定路径
    fs::path current_path = fs::current_path();
    std::cout << "Current path: " << current_path << std::endl;

    // 测试文件路径操作
    fs::path test_path = current_path / "test_directory" / "test_file.txt";
    std::cout << "Test path: " << test_path << std::endl;
    std::cout << "Parent path: " << test_path.parent_path() << std::endl;
    std::cout << "Filename: " << test_path.filename() << std::endl;
    std::cout << "Extension: " << test_path.extension() << std::endl;

    // 创建目录
    create_directory(test_path.parent_path());

    // 创建文件
    create_file(test_path, "Hello, cross-platform file system!");

    // 打印文件信息
    print_file_info(test_path);

    // 遍历目录
    traverse_directory(test_path.parent_path());

    // 删除文件和目录
    try {
        fs::remove(test_path);
        std::cout << "File deleted: " << test_path << std::endl;
        fs::remove(test_path.parent_path());
        std::cout << "Directory deleted: " << test_path.parent_path() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error deleting file or directory: " << e.what() << std::endl;
    }

    return 0;
}
