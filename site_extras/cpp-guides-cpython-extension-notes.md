> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-guides/cpython-extension-notes/) 的文章，由站点同步脚本导出。

# CPython 扩展开发笔记

本篇文章将会围绕最近给Apache提的一个feature为背景，展开讲讲CPython遇到的问题，以及尝试自己从0写一个库出来，代码也已经放星球了，感兴趣的同学可以去下载学习。

## 0.背景

最近在给apache arrow提的一个feature因为C++接口的变动引发其他语言的接口变动，一些测试也跟着需要修复。

像PyArrow熟悉的人应该一点也不陌生，这次接口变动也需要修改这个库，因为是在一个仓库里的，不然ci过不了。而PyArrow的实现是通过CPython实现的，之前也没特别学习CPython，改出了一堆问题，其中遇到两个问题比较重要，这里记录一下。

**问题1：初始化函数里面不支持其他类的默认构造。**

示例：

```
 def __init__(self, mode="only_valid", filter=Expression._scalar(True)):
 		pass
```

报错：

```
TypeError: descriptor '_scalar' for 'pyarrow._compute.Expression' objects doesn't apply to a 'bool' object
```

可以看到没识别出来，实际情况是Expression._scalar(True)合法的，我们看里面的实现：

```python
@staticmethod
def _scalar(value):
    cdef:
        Scalar scalar

    if isinstance(value, Scalar):
        scalar = value
    else:
        scalar = lib.scalar(value)

    return Expression.wrap(CMakeScalarExpression(scalar.unwrap()))
```

可以看到，里面支持正常的bool类型，我怀疑这是cpython的限制，于是改为下面这种方式就可以了：

```python
def __init__(self, mode="only_valid", filter=None):
    if filter is None:
        filter = Expression._scalar(True)
```

**问题2：定义顺序**

当我使用后面创建的`_true`，每次传递进去的默认值是空，这个比较好理解，因为最后编译好了会翻译为一个`xxx.cpp`文件，根据C++规则前面读到的自然就是空了。

```python
def __init__(self, mode="only_valid", filter=_true):
	 	pass

  
cdef CExpression _true = CMakeScalarExpression(
    <shared_ptr[CScalar]> make_shared[CBooleanScalar](True)
)
```

好了，基于以上背景，我自己也想写一个例子出来，例如：使用C++写一个类，封装sort和sum，然后使用Python调用。

## 1.CPython完整例子

1. 创建一个.h文件

```cpp
void sort(std::vector<int>& nums) {
    std::sort(nums.begin(), nums.end());
}
int sum(std::vector<int>& nums) {
    int sum = 0;
    for (int num : nums) {
        sum += num;
    }
    return sum;
}
```

2) 创建foo.pyx

重要点：上面vector需要：

```
from libcpp.vector cimport vector
```

然后去定义一个class，调用C++的接口。

```
cdef class PyFoo:
    cdef Foo* f

    def __cinit__(self):
        self.f = new Foo()

    def __dealloc__(self):
        del self.f

    def sort(self, nums):
        cdef vector[int] c_nums = nums
        self.f.sort(c_nums)

    def sum(self, nums):
        cdef vector[int] c_nums = nums
        return self.f.sum(c_nums)
```

3. 创建setup.py文件

```python
ext = Extension('Foo', sources=["foo.pyx"], language="c++", include_dirs=[numpy.get_include()])

setup(name="Foo", ext_modules = cythonize([ext]))
```

4. 运行

```
python3 setup.py build_ext --inplace
```

最后，可以写一个测试脚本去使用自己写的python接口。

```python
import Foo

f = Foo.PyFoo()
nums = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5]
f.sort(nums)
print("Sorted nums:", nums)
print("Sum of nums:", f.sum(nums))
```

CPython在一些项目中使用挺多的，学习起来吧~

运行：

```
➜  cpython_examples python3 test.py 
Sorted nums: [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5]
Sum of nums: 44
```