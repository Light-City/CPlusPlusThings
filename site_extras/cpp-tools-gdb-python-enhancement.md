> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/gdb-python-enhancement/) 的文章，由站点同步脚本导出。

# GDB Python 扩展调试

## 0.导语

调试是软件开发过程中不可或缺的一环，而GDB（GNU调试器）作为一款功能强大的调试工具，在开发者中得到广泛应用。除了传统的命令行调试功能外，GDB还提供了Python的GDB库，允许开发者使用Python语言扩展和增强调试的能力。本文将介绍如何在GDB中使用Python的GDB库，展示其强大的调试功能和用例。

开始之前，确保已经正确安装Python和GDB，并且Python版本与GDB兼容。然后，按照以下步骤安装Python的GDB库： 

a. 在命令行中执行`gdb`打开GDB。 

b. 输入`python`命令，进入GDB的Python解释器。 

c. 执行`import gdb`，如果没有报错，说明Python的GDB库已经正确安装。

```gdb
(gdb) python
>import gdb
>end
```

gdb调试过程中我们通常想查看一些复杂结构变量，例如PostgreSQL中的List结构，啥都能往里面放，里面具体是啥真不好直观打印出来，此时在我们日常开发中就有了python脚本，下面示例:

```gdb
(gdb) tl
TargetEntry [resno=1 resname="sum"]
	Aggref [aggfnoid=2107 aggtype=1700 aggtranstype=2281 aggstar=false aggvariadic=false aggkind=110 'n' agglevelsup=0 aggsplit=AGGSPLIT_FINAL_DESERIAL aggno=0 aggtransno=0 agg_expr_id=0]
```

## 1.Hello python gdb

我们可以在gdb中执行一些python操作，例如：print，如下所示：

```
scssCopy code
(gdb) python
> print(1+2)
> end
3
(gdb) end
This command cannot be used at the top level.
```

在这个示例中，我们进入GDB的Python解释器，并执行了一个简单的Python语句`print(1+2)`，结果为3。

另一个例子是我们可以直接输入python xxxx来运行。

```
scssCopy code
(gdb) python f = 2
(gdb) python print(f)
2
```

在这个示例中，我们在GDB中使用Python语句定义了一个变量`f`，并打印了它的值，结果为2。

## 2.实战：如何编写？

核心点在于我们如何编写这些脚本？通过这个脚本来方便打印出自己代码，下面是实战环节，关于本节的相关代码与资料将会更新在星球中，欢迎关注：



```cpp
struct Point {
    int x;
    int y;
};
```

现在有一个Point结构体，我想通过脚本打印出来，如何实现呢？

逻辑非常简单，读取结构体，然后print即可。

```python
import gdb

def print_point(point):
    print(f"Point: x={point['x']}, y={point['y']}")

def print_person(person):
    age = person['age']
    print(f"Person: age={age}")

class PrintCppStruct(gdb.Command):
    def __init__(self):
        super(PrintCppStruct, self).__init__("print_cpp_struct", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        try:
            cl = gdb.parse_and_eval(arg)
            if cl.type.code == gdb.TYPE_CODE_STRUCT and str(cl.type) == 'Point':
                print_point(cl)
            elif cl.type.code == gdb.TYPE_CODE_STRUCT and str(cl.type) == 'Person':
                print_person(cl)
            else:
                print("Unknown C++ structure type.")
        except gdb.error as e:
            print(f"Error: {e}")

PrintCppStruct()

```

如何使用？

只需要source即可：

```
(gdb) source print.py
```

在这个示例中，我们通过`source`命令在GDB中加载了一个名为`print.py`的Python脚本。

```cpp
(gdb) print_cpp_struct myPoint
Point: x=10, y=20
```

在这个示例中，我们在GDB中使用了一个自定义的命令`print_cpp_struct`，它通过Python的GDB库打印了一个名为`myPoint`的C++结构体的值。

通过在GDB中使用Python的GDB库，开发者可以利用Python的强大功能扩展和增强调试的能力。它提供了丰富的API和灵活的脚本编写能力，使得调试过程更加高效和便捷。无论是自动化调试任务、数据分析和可视化，还是动态修改程序行为，Python的GDB库都为开发者提供了丰富的选择和灵活性。希望本文能够帮助您更好地理解和应用Python的GDB库，提升调试技能和开发效率。