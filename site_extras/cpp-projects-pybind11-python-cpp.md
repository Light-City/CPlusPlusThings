> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/pybind11-python-cpp/) 的文章，由站点同步脚本导出。

# pybind11 加速 Python

通常像一些耗时的操作，我们期望在C++中去实现，然后使用Python去调用对应的接口，或者因为底层库的原因，需要支持对外的Python API，那么我们通常需要支持在Python中访问C++，如何实现呢？

方法比较多，本节以pybind11为例，引入一个完整的项目工程模版，如果你后续有这种需求，可以基于模版去修改。

注：(懒人版)本节的所有代码也都放在星球里面。

## 1.宏展开

假设：我们以比较耗时的排序为例，如果我们想在C++中写一个排序，然后在Python中去使用它，怎么去写呢？

C++程序是比较简单的，对外暴露一个接口里面做排序即可，例如：

```cpp
std::vector<int> sort_vector(const std::vector<int>& input) {
  std::vector<int> result = input;
  std::sort(result.begin(), result.end());
  return result;
}
```

那么问题来了，对于Pybind11来说，如何使用这个接口呢？

使用pybind11的宏即可定义一个模块，例如：

```cpp
PYBIND11_MODULE(sort_module, m) {
    m.doc() = "pybind11 example plugin"; // Optional module docstring

    m.def("sort_vector", &sort_vector, "A function that sorts a list of integers");
}
```

宏展开之后是：PyInit_sort_module

当我们在python当中import xxx库的时候，Python 解释器会去加载一个 C/C++ 编写的扩展模块时自动调用的入口函数，而这个入口函数就是下面的PyInit_sort_module：

```cpp
static ::pybind11::module_::module_def pybind11_module_def_sort_module [[maybe_unused]];

[[maybe_unused]] static void pybind11_init_sort_module(::pybind11::module_ &);

extern "C" [[maybe_unused]] __attribute__((visibility("default"))) PyObject *PyInit_sort_module();

extern "C" __attribute__((visibility("default"))) PyObject *PyInit_sort_module() {
    {
        const char *compiled_ver = "3" "." "11";
        const char *runtime_ver = Py_GetVersion();
        size_t len = std::strlen(compiled_ver);

        if (std::strncmp(runtime_ver, compiled_ver, len) != 0 || (runtime_ver[len] >= '0' && runtime_ver[len] <= '9')) {
            PyErr_Format(PyExc_ImportError,
                         "Python version mismatch: module was compiled for Python %s, "
                         "but the interpreter version is incompatible: %s.",
                         compiled_ver, runtime_ver);
            return nullptr;
        }
    }

    pybind11::detail::get_internals();
    auto m = ::pybind11::module_::create_extension_module("sort_module", nullptr, &pybind11_module_def_sort_module);

    try {
        pybind11_init_sort_module(m);
        return m.ptr();
    } catch (pybind11::error_already_set & e) {
        pybind11::raise_from(e, PyExc_ImportError, "initialization failed");
        return nullptr;
    } catch (const std::exception &e) {
        ::pybind11::set_error(PyExc_ImportError, e.what());
        return nullptr;
    }
}

void pybind11_init_sort_module(::pybind11::module_ & m) {
    m.doc() = "pybind11 example plugin";

    m.def("sort_vector", &sort_vector, "A function that sorts a list of integers");
}

```

所以，如果我们不去使用这个宏，在import库的时候会报错：

```python
>>> import sort_module
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: dynamic module does not define module export function (PyInit_sort_module)
```

## 2.CMake + pybind11

pybind11在C++中的使用如下：

- 头文件

```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
```

- 宏

```
PYBIND11_MODULE(sort_module, m) {
	// 你的实现
}
```

那么如何在cmake中下载并链接pybind11呢？

只需要使用FetchContent模块即可，例如：

```cpp
FetchContent_Declare(
    pybind11
    URL https://github.com/pybind/pybind11/archive/v2.12.0.tar.gz
    )
```

然后在CMakeLists中加入你的模块即可：

```cpp
pybind11_add_module(sort_module sort_module.cc)
```

cmake将会帮你下载pybingd11，同时编译好。

## 3.使用

上面会生成一个so，install到python的site-packages目录，然后import的时候便会识别，如下演示我们自定义的模块：

```python
import sort_module

def test_sort():
    input_list = [5, 2, 3, 1, 4]
    sorted_list = sort_module.sort_vector(input_list)
    print(f"Original list: {input_list}")
    print(f"Sorted list: {sorted_list}")

if __name__ == "__main__":
    test_sort()

```

运行：

```
(base) ➜  build python ../test.py
Original list: [5, 2, 3, 1, 4]
Sorted list: [1, 2, 3, 4, 5]
```