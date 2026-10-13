> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/cpp17-sqlite-folder-size/) 的文章，由站点同步脚本导出。

# C++17 与 SQLite 统计目录大小

## 项目实战：使用 C++17 和 SQLite 实现文件夹大小计算与存储

[TOC]

本节将会围绕一个项目背景展开：假设业务中需要定时检查某些文件夹大小，并做一下持久化操作，C++如何去完成这一个项目呢？

本节带你通过一个小项目，使用 C++17 和 SQLite 实现一个文件夹大小计算与存储的系统，帮助你更好地管理你的文件系统。

### 项目目标

我们将实现以下功能：
1. **递归计算指定目录及其子目录的大小**。
2. **将计算结果保存到 SQLite 数据库**，方便后续查询和分析。
3. **可扩展性强**：你可以轻松修改项目，添加多线程支持、优化性能，或将结果存储到其他数据库中。

**注：懒人版，完整代码已更新星球。**

### 项目实现步骤

#### 1. 环境

首先，你需要确保开发环境中已经安装了以下工具：
- **支持C++17 编译器**：如 g++，通过以下命令可以检查版本：
  
  ```bash
  g++ --version
  ```
  确保 g++ 支持 C++17 标准。
  
- **SQLite3**：数据库操作我们会使用 SQLite，你可以通过包管理器来安装它：
  ```bash
  sudo apt-get install sqlite3 libsqlite3-dev
  ```
  安装完后，SQLite 数据库的开发库 `libsqlite3-dev` 可以确保我们在 C++ 中能正常使用 SQLite。

#### 2. 创建项目结构

我们将把代码分为多个模块，保持清晰的架构：
- `folder_size.h` / `folder_size.cpp`：负责递归计算目录及其子目录大小。
- `database.h` / `database.cpp`：封装 SQLite 数据库操作逻辑。
- `main.cpp`：主程序入口，负责整体流程的控制。

项目的目录结构如下：
```
project/
│── folder_size.h
│── folder_size.cpp
│── database.h
│── database.cpp
└── main.cpp
```

#### 3. 代码实现

接下来，我们将一步步实现每个模块。

##### 3.1 目录计算模块

C++17 提供了`std::filesystem` 库来递归遍历目录并计算大小。因此其核心实现如下：

```cpp
uintmax_t total_size = 0;
for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
  if (fs::is_regular_file(entry.path())) {
    total_size += fs::file_size(entry.path());
  }
}
```

如果要计算根目录 再计算一次即可。

##### 3.2 SQLite数据库操作

引入：

```
#include <sqlite3.h>
```

使用：

```cpp
sqlite3_close(db_); // 关闭

// 插入
for (const auto& [folder, size] : folder_sizes) {
  const std::string insert_sql = 
      "INSERT OR REPLACE INTO FolderSizes (Folder, Size) "
      "VALUES ('" + folder + "', " + std::to_string(size) + ");";
  if (!ExecuteSQL(insert_sql)) {
    return false;
  }
}

// 执行
sqlite3_exec
```

#### 4. 编译与运行

编译项目非常简单：

```bash
mkdir build
cmake ../
make -j8
```

然后，运行程序，计算目录大小并存储到数据库：

```bash
./folder_size_calculator
```

程序将会递归计算指定目录的大小，并将结果保存到 `folder_sizes.db` 数据库中。

#### 5. 查询数据库

最后，你可以通过

 SQLite 命令行工具查询数据库中的内容：

```bash
sqlite3 folder_sizes.db
sqlite> SELECT * FROM FolderSizes;
```

这将显示所有已保存的目录及其对应的大小。

> 本节完