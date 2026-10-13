> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/cpp-grpc-demo/) 的文章，由站点同步脚本导出。

# C++ gRPC 小项目

## C++那些事之grpc小Demo

github上比较火的rpc有grpc、brpc，腾讯内部比较牛逼的trpc等等，这些rpc支持不同的语言、不同平台。今天来聊聊如何使用grpc，从一个简单的demo入手，整个项目使用CMake构建，一个非常标准的rpc项目管理，所有代码已更新至星球，获取方式见最后的二维码。

## 1.Step by Step 环境

如果你的电脑是mac，不好意思，暂时用不了最新版本的grpc，最新版本的grpc用了abseil，编译会有问题，官方暂时未解决，如果大家知道如何解决，可以留言，我这边目前未找到解决方法。

所以，建议安装低版本的grpc在你的mac上，或者在linux直接安装，linux上可以使用最新版本grpc，一次安装没有任何问题。

安装方式：

```
git clone -b v1.57.0 git@github.com:grpc/grpc.git
```

然后初始化submodule

```cpp
git submodule update --init
```

cmake编译安装：

```cpp
cd cmake
mkdir build
cd build
cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF   -DCMAKE_INSTALL_PREFIX=/usr/local/grpc ../..
  
make -j8
make -j8 install // root 权限
```

大家可以修改上面的安装目录，注意install时需要root权限。

安装完之后，将你的bin目录配置到path，例如：

```
export PATH=/usr/local/grpc/bin:$PATH
```

## 2.反转字符串Demo

环境搞定了之后，我们便可以上手搞事了。

### 2.1 协议

第一件事是确定rpc协议：

```cpp
syntax = "proto3";

package stringreverse;

service StringReverse {
    rpc sendRequest (StringRequest) returns (StringReply) {}
}

message StringRequest {
    string original = 1;
}

message StringReply {
    string reversed = 1;
}
```

非常简单的协议，请求一个字符串，返回一个字符串。

### 2.2 客户端

构造rpc请求字符串，然后调用sendRequest接口发送出去即可。

```cpp
std::string str = "hello world grpc!";
std::cout << "Original string: " << str << std::endl;
StringRequest request;
StringReply reply;
ClientContext context;
request.set_original(str);
Status status = stub->sendRequest(&context, request, &reply);
if (status.ok()) {
	std::cout << "Reversed string: " << reply.reversed() << std::endl;
}
// do something
```

### 2.3 服务端

通过继承Service实现反转字符串逻辑。

```cpp
class ReverseServiceImplementation final : public StringReverse::Service {
	Status sendRequest(ServerContext* context, const StringRequest* request,
                     StringReply* reply) override {
   	// 反转逻辑
  }             
}
```

最后注册service即可：

```cpp
builder.RegisterService(&service);
std::unique_ptr<Server> server(builder.BuildAndStart());
std::cout << "Server listening on port: " << server_address << std::endl;
```

## 3.运行

创建编译目录：

```
mkdir build
cmake ..
# 输出
-- The C compiler identification is GNU 10.2.1
-- The CXX compiler identification is GNU 10.2.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /opt/rh/devtoolset-10/root/usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /opt/rh/devtoolset-10/root/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Found Protobuf: /usr/local/grpc/bin/protoc-23.4.0 (found version "23.4.0") 
-- Using protobuf 
-- Using gRPC 1.57.0
-- Configuring done
-- Generating done
-- Build files have been written to: /code/stringreverse/build
```

make编译：

```
[ 10%] Generating stringreverse.pb.cc, stringreverse.pb.h, stringreverse.grpc.pb.cc, stringreverse.grpc.pb.h
Scanning dependencies of target server
[ 20%] Building CXX object CMakeFiles/server.dir/server.cc.o
[ 30%] Building CXX object CMakeFiles/server.dir/stringreverse.pb.cc.o
[ 40%] Building CXX object CMakeFiles/server.dir/stringreverse.grpc.pb.cc.o
[ 50%] Linking CXX executable server
[ 50%] Built target server
Scanning dependencies of target client
[ 60%] Building CXX object CMakeFiles/client.dir/client.cc.o
[ 70%] Building CXX object CMakeFiles/client.dir/stringreverse.pb.cc.o
[ 80%] Building CXX object CMakeFiles/client.dir/stringreverse.grpc.pb.cc.o
[ 90%] Linking CXX executable client
[100%] Built target client
```

运行server:

```
[light@i-i339drlk build]$ ./server 
Server listening on port: 0.0.0.0:5000
```

运行client:

```cpp
[light@i-i339drlk build]$ ./client 
Original string: hello world grpc!
Reversed string: !cprg dlrow olleh
```