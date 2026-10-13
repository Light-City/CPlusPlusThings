> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/stl-debug-mode/) 的文章，由站点同步脚本导出。

# C++ STL Debug 模式

## 问题

Project阶段的schema里面的fields发生了变化，fields是一个vector节点，每个成员是`std::shared_ptr<arrow::Field>`，对比前后的vector指针发现`_M_impl`的`_M_start`位置发生了变化，因此需要gdb watch其变化，查看何处改了它。

## Vector结构

vecotr代码：

> https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_vector.h

```cpp
typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
       	pointer;
struct _Vector_impl_data
{
	pointer _M_start;
	pointer _M_finish;
	pointer _M_end_of_storage;
}
```

需要知道几个重要信息：

- `_M_start`与`_M_finish`等都是pointer类型
- pointer类型是原始指针类型

有了这个信息，我们watch便可以非常方便的观察这个`_M_start`发生了什么变化，注意：是这个指针`_M_start`发生了变化，而不是其值发生了变化。

## Watch

首先打印其地址:

```cpp
(gdb) p &((arrow::Schema::Impl *)this->output_schema_->impl_)->fields_._M_impl._M_start
$10 = (std::_Vector_base<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field> > >::pointer *) 0xb11dd0
```

我们得到了`pointer* `的地址，然后watch它。

```cpp
(gdb) watch *((std::_Vector_base<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field> > >::pointer *) 0xb11dd0)
Hardware watchpoint 2: *((std::_Vector_base<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field> > >::pointer *) 0xb11dd0)
```

随后我们continue，变得了变更点：

```cpp
(gdb) c
Continuing.


Thread 1 "postgres" hit Hardware watchpoint 2: *((std::_Vector_base<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field> > >::pointer *) 0xb11dd0)

Old value = (std::_Vector_base<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field> > >::pointer) 0x979470
New value = (std::_Vector_base<std::shared_ptr<arrow::Field>, std::allocator<std::shared_ptr<arrow::Field> > >::pointer) 0x9fe1e0
VecSeqNext (node=0x9fdc40) at src/backend/vecexecutor/nodeSeqscan.c:210
210		if (table_scan_getnextslot(scandesc, direction, slot))
(gdb) bt
#0  VecSeqNext (node=0x9fdc40) at src/backend/vecexecutor/nodeSeqscan.c:210
#1  0x00007f45994e5c70 in ExecVecScanFetchInternal
    (node=0x9fdc40, accessMtd=0x7f45994e6335 <VecSeqNext>, recheckMtd=0x7f45994e6430 <VecSeqRecheck>) at src/backend/vecexecutor/execScan.c:146
#2  0x00007f45994e58da in ExecVecScanFetch (node=0x9fdc40, accessMtd=0x7f45994e6335 <VecSeqNext>, recheckMtd=0x7f45994e6430 <VecSeqRecheck>)
    at src/backend/vecexecutor/execScan.c:40
#3  0x00007f45994e5d0b in ExecVecScan
    (node=0x9fdc40, vestate=0x9fdd48, accessMtd=0x7f45994e6335 <VecSeqNext>, recheckMtd=0x7f45994e6430 <VecSeqRecheck>)
    at src/backend/vecexecutor/execScan.c:200
#4  0x00007f45994e6487 in ExecVecSeqScan (pstate=0x9fdc40) at src/backend/vecexecutor/nodeSeqscan.c:246
#5  0x00007f45994c40ce in VecExecProcNodeGPDB (node=0x9fdc40) at src/backend/hook/executor.c:102
#6  0x00007f45994c41c2 in VecExecProcNodeFirst (node=0x9fdc40) at src/backend/hook/executor.c:153
#7  0x00007f45994dc610 in ExecProcNode (node=0x9fdc40) at /home/gpadmin/install/cbdb/include/postgresql/server/executor/executor.h:281
#8  0x00007f45994e2065 in get_current_next_batch (node=0x9fdc40) at src/backend/vecexecutor/execMain.c:1851
#9  0x00007f4599375c76 in get_arrow_record_batch(GetNextCallback, void*) (fn=0x7f45994e204d <get_current_next_batch>, arg=0x9fdc40)
    at c_glib/arrow-glib/reader.cpp:216


```