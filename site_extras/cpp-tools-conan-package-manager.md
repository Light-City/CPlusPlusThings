> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/conan-package-manager/) 的文章，由站点同步脚本导出。

# Conan C++ 包管理入门

## 1.Conan安装

```
pip3 install conan
```

## 2.安装第三方库

安装第三方库时，我们通常需要知道安装的库版本，那么conan提供了相应的搜索命令，列出相应库的版本。

```
conan search gtest --remote=conancenter
```

如果直接搜会报错，conancenter找不到，所以第一步设置remote。

```
conan remote add conancenter https://center.conan.io
```

随后，我们再继续上面的搜索命令，发现报错：

```cpp
ERROR: HTTPSConnectionPool(host='center.conan.io', port=443): Max retries exceeded with url: /v1/ping (Caused by SSLError(SSLError(1, '[SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed (_ssl.c:877)'),))

Unable to connect to conancenter=https://center.conan.io
1. Make sure the remote is reachable or,
2. Disable it by using conan remote disable,
Then try again.
```

此时，需要配置证书

```
conan config install https://github.com/conan-io/conanclientcert.git
```

输出：

```cpp
Trying to clone repo: https://github.com/conan-io/conanclientcert.git
Repo cloned!
Copying file LICENSE to /home/light/.conan/.
Copying file cacert.pem to /home/light/.conan/.
```

随后，继续使用`conan search`，我们找到：

```cpp
[light@i-fu3d01tc build]$ conan search gtest --remote=conancenter
Existing package recipes:

gtest/cci.20210126
gtest/1.8.1
gtest/1.10.0
gtest/1.11.0
gtest/1.12.1
gtest/1.13.0
gtest/1.14.0
```

## 3.编写一个项目

接下来，使用conan编写一个项目。

项目背景：在开发阶段，我们需要进行单元测试，例如：除法需要测试除以0与非零等边界的断言，我们需要引入第三方库，这里用gtest。

1.编写代码

```cpp
#include <gtest/gtest.h>
double divide(double a, double b) { return b == 0 ? 0 : a / b; }

TEST(Div, NonZero) {
  double expected = 1.5;
  double actual = divide(3, 2);
  ASSERT_EQ(expected, actual);
}

TEST(Div, Zero) {
  double expected = 0;
  double actual = divide(3, 0);
  ASSERT_EQ(expected, actual);
}

```

2.编写conanfile.txt，指定一个gtest版本。

```
[requires]
gtest/1.13.0

[generators]
cmake
```

3.添加CMakeLists.txt，内容很简单：其中感叹号部分需要注意。

```cpp
cmake_minimum_required(VERSION 3.16)
project(MultiplyTest LANGUAGES CXX)

enable_testing()

// !!!
find_package(GTest REQUIRED)

add_executable(div_test div_test.cpp)

// !!!
target_link_libraries(div_test
 PRIVATE
  GTest::GTest)

add_test(div_gtest div_test)

```

4.开始安装与编译

- 安装第三方库

```
[gpadmin@i-fu3d01tc build]$ conan install ../
Configuration:
[settings]
arch=x86_64
arch_build=x86_64
build_type=Release
compiler=gcc
compiler.libcxx=libstdc++11
compiler.version=10
os=Linux
os_build=Linux
[options]
[build_requires]
[env]

gtest/1.13.0: Not found in local cache, looking in remotes...
gtest/1.13.0: Trying with 'conan-public'...
gtest/1.13.0: Trying with 'conancenter'...
Downloading conanmanifest.txt completed [0.10k]                                          
Downloading conanfile.py completed [8.27k]                                               
Downloading conan_export.tgz completed [0.32k]                                           
Decompressing conan_export.tgz completed [0.00k]                                         
gtest/1.13.0: Downloaded recipe revision 8a0bc5b3e159ed45de97260c2bff65b5
conanfile.txt: Installing package
Requirements
    gtest/1.13.0 from 'conancenter' - Downloaded
Packages
    gtest/1.13.0:e019a06362b932ca5d1b082b6c112aa150c88de4 - Download

Installing (downloading, building) binaries...
gtest/1.13.0: Retrieving package e019a06362b932ca5d1b082b6c112aa150c88de4 from remote 'conancenter' 
Downloading conanmanifest.txt completed [3.07k]                                          
Downloading conaninfo.txt completed [0.60k]                                              
Downloading conan_package.tgz completed [487.88k]                                        
Decompressing conan_package.tgz completed [0.00k]                                        
gtest/1.13.0: Package installed e019a06362b932ca5d1b082b6c112aa150c88de4
gtest/1.13.0: Downloaded package revision 81bd558bf2388a3aee4ee13b8a902a45
conanfile.txt: Generator txt created conanbuildinfo.txt
conanfile.txt: Generator cmake created conanbuildinfo.cmake
conanfile.txt: Aggregating env generators
conanfile.txt: Generated conaninfo.txt
conanfile.txt: Generated graphinfo
```

- 编译

```cpp
[light@i-fu3d01tc build]$ cmake ..
-- The CXX compiler identification is GNU 10.2.1
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /opt/rh/devtoolset-10/root/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found GTest: /usr/lib64/libgtest.so  
-- Configuring done
-- Generating done
-- Build files have been written to: /home/light/cmake-google-tests/conan/build
[light@i-fu3d01tc build]$ make
Scanning dependencies of target div_test
[ 50%] Building CXX object CMakeFiles/div_test.dir/div_test.cpp.o
[100%] Linking CXX executable div_test
[100%] Built target div_test
```

- 运行

```
[light@i-fu3d01tc build]$ ./div_test 
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from Div
[ RUN      ] Div.NonZero
[       OK ] Div.NonZero (0 ms)
[ RUN      ] Div.Zero
[       OK ] Div.Zero (0 ms)
[----------] 2 tests from Div (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (0 ms total)
[  PASSED  ] 2 tests.
```