> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-benchmarks/cpp-sso-benchmark/) 的文章，由站点同步脚本导出。

# C++ string SSO 性能测试

今天想问大家两个问题：

**1.下面代码性能上有没有区别？**

```cpp
std::string s1("hello world hel");
std::string s2("hello world helo");
```

**2.sizeof(std::string)是多少？**

下面，我们来一起探索这两个问题。

由于这两个答案在g++(libstdc++)与clang(libc++)上的结果不一样，所以这里会分别进行分析。

## 1.g++(libstdc++)

g++的源码我们以g++-10为例，详细代码可以戳这个github地址，这里列出比较核心的代码。

> https://github.com/gcc-mirror/gcc/blob/devel/omp/gcc-10/libstdc%2B%2B-v3/include/bits/basic_string.h#L171

```cpp
template<typename _CharT, typename _Traits, typename _Alloc>
class basic_string {
  private:
    struct _Alloc_hider : allocator_type {
    	pointer _M_p; // The actual data.
  	};
 	  enum { __min_cap = 15 / sizeof(value_type) };
    _Alloc_hider	_M_dataplus;             //  8 bytes
    size_type __size_;                     //  8 bytes
  
  	union {
			_CharT           _M_local_buf[_S_local_capacity + 1]; // 16 bytes 
			size_type        _M_allocated_capacity;								// 8 bytes
    }; // 16 bytes
}; // 32 bytes
```

在上面代码中注释部分详细的列出了string占据的字节(注意：这里的value_type以char来计算，也就是sizeof(value_type) = 1)。

sizeof(string) = 8 + 8 + 16 = 32。

其中8字节ptr，8字节size，16字节union。

性能层面，这里实现了SSO，本地buffer会存储最多15个字符，当超过15字符便会分配在heap上，这里我写了一个benchmark来对比性能。

- 短字符串测试内容为："hello world hel"，总共15个字符。
- 长字符串测试内容为："hello world helo"，总共为16个字符。

```
std::string s1("hello world hel");
std::string s2("hello world helo");
```

可以看到性能差距非常大，仅仅一个字符会导致std::string性能差别这么大！

这里本质还是前面的SSO local buffer。

性能测试：https://quick-bench.com/q/s7pvDB_SwQyDCcZk50odAaYIzMc

![](../assets/images/2024/bench/img/gcc_short_long.png)

## 2.clang++(libc++)

同理，我们以llvm-14为例，如果想要看完整代码戳下面的github地址即可，这里摘取出核心代码。

> https://github.com/llvm/llvm-project/blob/release/14.x/libcxx/include/string#L765

```cpp
template<class _CharT, class _Traits, class _Allocator>
class basic_string {
  private:
    struct __long {
        size_type __cap_;               // 8 bytes
        size_type __size_;              // 8 bytes
        pointer   __data_;              // 8 bytes
    };

		enum {__min_cap = (sizeof(__long) - 1)/sizeof(value_type) > 2 ?
                      (sizeof(__long) - 1)/sizeof(value_type) : 2};

    struct __short {
        union {
            unsigned char __size_;      // 1 bytes
            value_type __lx;            // 1 bytes 
        };                              // 1 bytes
        value_type __data_[__min_cap];  // 23 bytes
    };                                  // 24 bytes

		union __ulx {
			__long __lx; 		// 24 bytes
			__short __lxx;	// 24 bytes
		}; // 24 bytes
}
```

在上面代码中注释部分详细的列出了string占据的字节(注意：这里的value_type以char来计算，也就是sizeof(value_type) = 1)。

sizeof(string) = union的大小，这个union是24字节，所以等于24。

而SSO，本地buffer存储的字符不超过22，当超过22个字符便会分配在heap上。

所以当我们使用与gcc同样的测试代码应用在clang上，我们便得到了不太一样的结果，即：long字符串的性能与short字符串的性能拆别不大。原因就是gcc的测试case都是在22个字符以内，都是短字符。

性能测试：https://quick-bench.com/q/nmLMqQZ31cmWBbbIFr2nHN2JL1k

![](../assets/images/2024/bench/img/llvm_short_long_no.png)

当我们测试真正的长短字符串时，性能便会发生明显变化，趋势与gcc保持一致。

- 短字符串测试内容为："hello world hello worl"，总共22个字符。
- 长字符串测试内容为："hello world hello world"，总共为23个字符。

性能测试：https://quick-bench.com/q/ujtMrtotEi6RrpfMDJ1UcSLFIJE

![](../assets/images/2024/bench/img/llvm_short_long.png)