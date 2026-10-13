> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/meson-build/) 的文章，由站点同步脚本导出。

# Meson 构建 C++ 项目

最近在使用一些开源项目的时候，发现都采用了meson+ninja的构建方式，摒弃了CMake，Makefile的方式，本文将会引入Meson，从入门安装到完整编写一个meson项目。

下面，我们来进入正文。

## 1.Meson安装

> https://github.com/mesonbuild/meson

Meson 可以通过 PyPi 获得，因此可以使用 pip3 install meson 进行安装。确保在不同系统中使用正确的 pip 命令，务必使用 Python 3 版本的 pip。

如果希望，也可以使用标准的 Python 命令在本地安装：

```bash
python3 -m pip install meson
```

对于使用 Ninja 进行构建的情况，可以直接从 Ninja GitHub 发布页面或通过 PyPi 下载 Ninja：

```bash
python3 -m pip install ninja
```

有关安装 Meson build 的更多信息，请阅读下方Meson官方网站。

> https://mesonbuild.com/Getting-meson.html

## 2.Meson Vs Cmake

既然大家都用CMake，为何还要用Meson呢，他们各自优劣是什么呢？

在性能方面，Meson 通常以其快速的构建速度而闻名。Meson 的设计旨在优化构建速度，并且在增量构建方面表现特别出色。Meson 使用 Ninja 构建系统作为默认后端，Ninja 本身也以快速和高效而著称。

相比之下，CMake 的构建速度可能会受到项目规模的影响，尤其是在较大的项目中。CMake 使用各种生成器（如Makefile），而不同生成器的性能也可能有所不同。

当然也可以使用CMake + Ninja去提高编译速度。

从开源项目角度来看Meson，其使用场景也是非常的多，例如：[PostgreSQL](https://www.postgresql.org/)、[QEMU](https://qemu.org/)、[GNOME Software](https://gitlab.gnome.org/GNOME/gnome-software)、[GTK+](https://gitlab.gnome.org/GNOME/gtk),等等

> https://mesonbuild.com/Users.html

### 2.1 Meson的优势与劣势

1.优势

- 基于Python，使用简单
- 支持交叉编译
- 所有对象都是强类型的：由于对象是诸如 'dependency'、'include directory' 等实体，因此很难出现字符串替换错误。
- 非常方便的引用第三发库
- **非常非常快，特别是在增量构建时。**
- 文档比cmake好n倍。

> http://mesonbuild.com

2.劣势

- 可能不如CMake成熟
- 有Python3 + Ninja的依赖
- 项目生成器，IDE支持Meson不如CMake
  - vscode支持
  - https://github.com/mesonbuild/vscode-meson
- 可用模块有限
  - 需要自己将非meson项目提交到wrapdb
  - https://mesonbuild.com/Wrapdb-projects.html

### 2.2 CMake的优势与劣势

1.优势

- IDE支持友好，团队协作比较友
- 成熟，几乎成为C++项目的标准

2.缺点

- 交叉编译困难
- 编译速度相对比较慢

## 3.Meson WrapDB

由于googletest不是meson项目，那如何使用meson去依赖它呢？

同理，grpc、abseil-cpp使用方式类似。

这里就要先引入WrapDB，Meson WrapDB 是 Meson 构建系统的一个组件，用于管理和提供第三方项目的构建描述文件，通常称为 "wrap" 文件。这些 wrap 文件包含了配置和构建第三方项目所需的信息，以便能够轻松地集成到使用 Meson 构建系统的其他项目中。

可以通过search命令或者下方网站查询第三方库。

> https://mesonbuild.com/Wrapdb-projects.html#meson-wrapdb-packages

我们以gtest使用为例：

- 搜索gtest

```
meson wrap search gtest
```

如果该命令返回包装的项目名称，那么便可以进行下一步了：

- 在自己的项目创建subprojects目录，通过install安装gtest

```
mkdir -p subprojects
meson wrap install gtest
```

此时你的项目所依赖的第三方库均已在subprojects目录安装好了。

```cpp
➜  gtest_div tree -L 1 subprojects
subprojects
├── gtest.wrap
```

此时会生成gtest.wrap，里面的内容会包含下面的内容。

注意：此文件是上方install之后产生的文件，建议不要修改。

```cpp
[wrap-file]
directory = googletest-1.14.0
source_url = https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
source_filename = gtest-1.14.0.tar.gz
source_hash = 8ad598c73ad796e0d8280b082cebd82a630d73e73cd3c70057938a6501bba5d7
patch_filename = gtest_1.14.0-1_patch.zip
patch_url = https://wrapdb.mesonbuild.com/v2/gtest_1.14.0-1/get_patch
patch_hash = 2e693c7d3f9370a7aa6dac802bada0874d3198ad4cfdf75647b818f691182b50
source_fallback_url = https://github.com/mesonbuild/wrapdb/releases/download/gtest_1.14.0-1/gtest-1.14.0.tar.gz
wrapdb_version = 1.14.0-1

[provide]
gtest = gtest_dep
gtest_main = gtest_main_dep
gmock = gmock_dep
gmock_main = gmock_main_dep

```

## 4.如何编写一个meson项目？

接下来，来到实践环节，如何编写一个meson项目呢？

下面以a/b除法测试为例，使用gtest进行单元测试的一个小项目。

### 4.1 编写代码

```
double divide(double a, double b);
```

.h与.cc放到src目录：

```cpp
src
├── division.cc
└── division.h
```

随后同级目录创建test目录。

```cpp
test
└── division_test.cc
```

### 4.2 meson项目管理

在项目根路径添加meson.build文件，内容如下：

```python
project('gtest_div', 'cpp')
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep') 
div_src = files('src/division.cc')
tests_src = files('test/division_test.cc')
e = executable(
    'division_test',
    tests_src + div_src, 
    dependencies : [ gtest_dep],
    override_options : ['cpp_std=c++14']
)
# https://mesonbuild.com/Unit-tests.html#testing-tool
test('this is a division test', e)
```

可以看到这个内容跟python的语法基本一致，熟悉python的看这个代码就非常容易了，这里着重讲一下上面比较重要的部分。

- 引入第三方库

例如：你引入的第三方库的项目叫做gtest，如下：

```
project('gtest', 'cpp',
  version : '1.14.0',
  default_options : ['cpp_std=c++14'],
  license : 'bsd')
```

那么便可以通过subproject获取到这个project，然后通过`gtest_proj.get_variable`便可以获取到gtest的依赖信息，例如：googletest里面是这样声明的：

```c
gtest_main_dep = declare_dependency(
    include_directories : gtest_incdir,
    sources : [gtest_libsources, gtest_mainsources],
    dependencies : dependency('threads')
)
```

- executable

executable跟cmake里面的add_executable非常类似，生成可执行文件division_test。

- test

单元测试语法：

```
e = executable('prog', 'testprog.c')
test('name of test', e)
```

可以使用`meson test`或者`ninja test`，运行完之后会产生测试日志，位于：

```
build/meson-logs/testlog.txt
```

### 4.3 构建与运行Meson项目

经过上面的操作，一个meson项目准备就绪。接下来，我们来构建与运行这个项目。

#### 4.3.1 配置构建目录

此时类似于cmake的`mkdir build && cmake ..`。

```cpp
meson setup build
```

运行如下：

```
➜  gtest_div meson setup build               
The Meson build system
Version: 1.2.3
Source dir: /Users/light/work/practice/gtest_div
Build dir: /Users/light/work/practice/gtest_div/build
Build type: native build
Project name: gtest_div
Project version: undefined
C++ compiler for the host machine: c++ (clang 12.0.5 "Apple clang version 12.0.5 (clang-1205.0.22.11)")
C++ linker for the host machine: c++ ld64 650.9
Host machine cpu family: x86_64
Host machine cpu: x86_64
Downloading gtest source from https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
Download size: 867764
Downloading: ..........
Downloading gtest patch from https://wrapdb.mesonbuild.com/v2/gtest_1.14.0-1/get_patch
Download size: 2440
Downloading: ..........

Executing subproject gtest 

gtest| Project name: gtest
gtest| Project version: 1.14.0
gtest| C++ compiler for the host machine: c++ (clang 12.0.5 "Apple clang version 12.0.5 (clang-1205.0.22.11)")
gtest| C++ linker for the host machine: c++ ld64 650.9
gtest| Run-time dependency threads found: YES
gtest| Dependency threads found: YES unknown (cached)
gtest| Dependency threads found: YES unknown (cached)
gtest| Dependency threads found: YES unknown (cached)
gtest| Build targets in project: 0
gtest| Subproject gtest finished.

Build targets in project: 1

gtest_div undefined

  Subprojects
    gtest: YES

Found ninja-1.11.1 at /usr/local/bin/ninja
```

此时我们可以在subprojects看到缓存的gtest项目。

```
subprojects
├── googletest-1.14.0  # here
├── gtest.wrap
└── packagecache # here
```

#### 4.3.2 编译

我们可以使用下面两个命令，第一种：

```
meson compile -C build/
```

运行日志：

```
➜  gtest_div meson compile -C build/            
INFO: autodetecting backend as ninja
INFO: calculating backend command to run: /usr/local/bin/ninja -C /Users/light/work/practice/gtest_div/build
ninja: Entering directory `/Users/light/work/practice/gtest_div/build'
[5/5] Linking target division_test
```

第二种：

```
➜  gtest_div ninja -C build
ninja: Entering directory `build'
[5/5] Linking target division_test
```

这两种命令本质是一样，都会用ninja来编译，当然也可以不用`-C`指定目录，那么此时需要`cd build && ninja`或者`cd build && meson compile`。

#### 4.3.3 运行与测试

此时会生成上division_test可执行程序，运行如下：

```
➜  gtest_div ./build/division_test
Running main() from ../subprojects/googletest-1.14.0/googletest/src/gtest_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from DivisionTest
[ RUN      ] DivisionTest.Divide
[       OK ] DivisionTest.Divide (0 ms)
[ RUN      ] DivisionTest.DivideByZero
[       OK ] DivisionTest.DivideByZero (0 ms)
[----------] 2 tests from DivisionTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
```

由于在meson.build中添加单元测试test，所以我们可以使用

```
meson test
```

或者：

```
ninja test
```

如果指定目录：

```
meson test -C build/

or 

ninja test -C build/
```

运行日志：

```
➜  gtest_div meson test -C build 
ninja: Entering directory `/Users/light/work/practice/gtest_div/build'
ninja: no work to do.
1/1 this is a division test        OK              0.01s

Ok:                 1   
Expected Fail:      0   
Fail:               0   
Unexpected Pass:    0   
Skipped:            0   
Timeout:            0   

Full log written to /Users/light/work/practice/gtest_div/build/meson-logs/testlog.txt
```