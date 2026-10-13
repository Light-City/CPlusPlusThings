> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/protobuf-zerocopy-cpp/) 的文章，由站点同步脚本导出。

# Protobuf ZeroCopy 序列化

## 1.ZeroCopyStream

protobuf在io接口上有一个叫做ZeroCopyStream，对于IO的接口设计，pb提供了相关序列化与反序列化接口，如：

```cpp
// Read a protocol buffer from the given zero-copy input stream.  If
// successful, the entire input will be consumed.
bool ParseFromZeroCopyStream(io::ZeroCopyInputStream* input);

// Write the message to the given zero-copy output stream.  All required
// fields must be set.
bool SerializeToZeroCopyStream(io::ZeroCopyOutputStream* output) const;
```

ZeroCopyStream设计的初衷是最小化buffer的拷贝次数，即省略掉stream内部数据拷贝到用户buffer。因此，stream可以返回一个缓冲区，该缓冲区实际上直接指向要存储字节的最终数据结构，并且调用者可以直接与该缓冲区交互，从而消除了中间复制操作。

例如：经典的IO stream：

```cpp
char buffer[[]BUFFER_SIZE];
input->Read(buffer, BUFFER_SIZE);
DoSomething(buffer, BUFFER_SIZE);
```

然后，stream基本上只是调用memcpy()将数据从pb复制到用户buffer中。如果使用ZeroCopyInputStream，我们只需要：

```cpp
const void* buffer;
int size;
input->Next(&buffer, &size);
DoSomething(buffer, size);
```

这里不执行拷贝，调用者最终直接从buffer中读取。

ZeroCopyStream提供了两个基类：ZeroCopyOutputStream/ZeroCopyInputStream。

以InputStream为例，通常我们可以通过继承的方式自定义自己的ZerCopyStream，需要实现下面四个接口。

```cpp
// implements ZeroCopyInputStream ----------------------------------
bool Next(const void** data, int* size);
void BackUp(int count);
bool Skip(int count);
int64 ByteCount() const;
```

一些rpc框架基本都自定义自己的Stream，例如：sofa-pbrpc

> https://github.com/baidu/sofa-pbrpc/blob/master/src/sofa/pbrpc/buffer.h

## 2.Demo

定义一个pb协议，例如：授权验证：

```cpp
syntax = "proto3";

message BasicAuth {
  string username = 1;
  string password = 2;
}
```

随后编写序列化与反序列化：

- 序列化

```cpp
BasicAuth auth_message;
auth_message.set_username("user123");
auth_message.set_password("password");

StringOutputStream output_stream(&buf);

auth_message.SerializeToZeroCopyStream(&output_stream);
```

- 反序列化

```cpp
ArrayInputStream input_stream(buf.data(), buf.size());
BasicAuth auth_message;
if (!auth_message.ParseFromZeroCopyStream(&input_stream)) {
    std::cerr << "Failed to parse data." << std::endl;
}

std::cout << "Username: " << auth_message.username() << std::endl;
std::cout << "Password: " << auth_message.password() << std::endl;
```

随后我们便可以调用它：

```cpp
int main() {
    std::string buf;
    buf.reserve(512);
    ser(buf);
    deser(buf);
    return 0;
}
```

输出：

```cpp
Username: user123
Password: password
```

本节完，以上完整代码见知识星球，欢迎与我共同探讨rpc相关内容。

















































```
(gdb) p	*dsr
$8 = {typeInfo = {datumlen = 4, typid = 1082, align = 105 'i', byval = true}, datumStreamVersion = DatumStreamVersion_Original, nth = -1,
  logical_row_count = 16382, datump = 0x3840470 "\277\377\377\377\231\002", physical_datum_index = -1, physical_datum_count = 0,
  datum_beginp = 0x3840470 "\277\377\377\377\231\002", datum_afterp = 0x38503dc "", has_null = true, buffer_beginp = 0x383fc60 "",
  null_bitmap = {buffer = 0x383fc70 "", byteBit = 0 '\000', bytePointer = 0x383fc70 "", bitCount = 16382, bitPosition = -1,
    readBitOnCount = 58981488}, null_bitmap_beginp = 0x0, rle_can_have_compression = false, rle_compress_beginp = 0x0, rle_repeatcountsp = 0x0,
  delta_beginp = 0x0, delta_deltasp = 0x0, delta_datum_p = 0, rle_norepeats_null_bitmap_count = 0, rle_compress_bitmap_count = 0,
  rle_repeatcounts_count = 0, rle_repeatcounts_size = 0, delta_item = false, delta_bitmap_count = 0, deltas_count = 0, deltas_size = 0,
  rle_block_was_compressed = false, rle_compress_bitmap = {buffer = 0x0, byteBit = 0 '\000', bytePointer = 0x0, bitCount = 0, bitPosition = 0,
    readBitOnCount = 0}, rle_repeatcounts_index = 0, rle_in_repeated_item = false, rle_repeated_item_count = 0,
  rle_total_repeat_items_read = 0, delta_block_was_compressed = false, delta_bitmap = {buffer = 0x0, byteBit = 0 '\000',
    bytePointer = 0xff6c00454244 <error: Cannot access memory at address 0xff6c00454244>, bitCount = 0, bitPosition = 0,
    readBitOnCount = 13651600}, eyecatcher = "\000\000\000", physical_data_size = 0, maxDataBlockSize = 13652112,
  errdetailCallback = 0x35daeb0, errdetailArg = 0x34c4e18, errcontextCallback = 0x34c4e18, errcontextArg = 0x383fc60, memctxt = 0x1077c}
```