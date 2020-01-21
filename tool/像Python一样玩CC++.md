# 像Python一样玩C/C++

在Python中我们可以使用`Jupyter Notebook`直接看到结果，例如：

```c
l = [1,2]
l
```

直接输出：

```
[1,2]
```

那当使用C++的时候，例如：

```cpp
map<string, int> mp{
    {"one",   1},
    {"two",   2},
    {"three", 3},
    {"four",  4}
};
```

如果要输出，就得循环遍历，可否直接输出结果呢？

so easy!!! `Jupyter Notebook`可以解决一切问题，哈哈～

## 如何在Jupyter中玩C++？

在github上有一个仓库，如下所示：

> https://github.com/QuantStack/xeus-cling

`xeus-cling` 是一个用于C++的Jupyter内核，基于C++解释器和Jupyter协议xeus的原生实现。

目前，支持Mac与Linux，但不支持Windows。

安装也是非常简单，首先安装好Anaconda，在里面创建一个虚拟环境：

```
conda create -n cling
```

切换进去：

```
conda activate cling
```

给新环境安装`jupyter`和`notebook`

```
conda install jupyter notebook
```

使用`conda-forge`安装`xeus-cling`

```
conda install xeus-cling -c conda-forge
```

为了加速安装，请记得给Anaconda配置源！

检查是否安装好了内核(kernel)：

```
jupyter kernelspec list
```

输出：

```cpp
python3    /home/xxx/anaconda3/envs/cling/share/jupyter/kernels/python3
xcpp11     /home/xxx/anaconda3/envs/cling/share/jupyter/kernels/xcpp11
xcpp14     /home/xxx/anaconda3/envs/cling/share/jupyter/kernels/xcpp14
xcpp17     /home/xxx/anaconda3/envs/cling/share/jupyter/kernels/xcpp17
```

打开`Jupyter Notebook`，就可以看到看到kernel了。

启动`Jupyter Notebook`：

```
jupyter-notebook
```

## 如何在Jupyter中玩C？

只需要安装c kernel即可！

可以直接在当前环境中创建c kernel，也可以新开一个环境安装，下面是在当前环境中直接安装。

```
pip install jupyter-c-kernel
install_c_kernel
jupyter kernelspec list
```

此时，就输出：

```cpp
c          /home/light/anaconda3/envs/cling/share/jupyter/kernels/c
python3    /home/light/anaconda3/envs/cling/share/jupyter/kernels/python3
xcpp11     /home/light/anaconda3/envs/cling/share/jupyter/kernels/xcpp11
xcpp14     /home/light/anaconda3/envs/cling/share/jupyter/kernels/xcpp14
xcpp17     /home/light/anaconda3/envs/cling/share/jupyter/kernels/xcpp17
```

启动`Jupyter Notebook`：

```
jupyter-notebook
```

