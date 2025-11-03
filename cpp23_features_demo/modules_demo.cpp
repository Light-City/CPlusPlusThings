// 模块接口单元：module_demo
// 模块声明
module module_demo;

// 导出公共接口
#include <iostream>
#include <string>

export namespace module_demo {
    // 公共函数声明
    void hello();
    std::string get_message();
}

// 模块实现单元
module module_demo;

// 导入必要的头文件
#include <iostream>
#include <string>

// 实现公共函数
namespace module_demo {
    void hello() {
        std::cout << "Hello from module_demo!" << std::endl;
    }

    std::string get_message() {
        return "This is a message from module_demo.";
    }
}

// 主程序
import module_demo;

int main() {
    module_demo::hello();
    std::cout << module_demo::get_message() << std::endl;
    return 0;
}
