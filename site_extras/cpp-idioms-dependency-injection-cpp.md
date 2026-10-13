> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-idioms/dependency-injection-cpp/) 的文章，由站点同步脚本导出。

# C++ 依赖注入

最近星球里面有个小伙伴让更新一下依赖注入，于是写出了这篇文章，来从实际的例子讲解，本文会讲解一些原理与实现，完整的实现代码懒人版放在星球中，我们开始正文。

大纲：

- 直接依赖
- 接口依赖注入
- 模版依赖注入

依赖注入是一种常用的设计模式，通常会采用接口/模版的方式提高代码的可测性与维护性，下面我们来举个例子。

假设有一个用户服务类UserService，需要通过HTTP客户端与远程服务器进行通信，在这种情况下我们可能会写出如下三种实现。

## 1.直接依赖

UserService内部直接依赖HttpClient，例如：

```cpp
class HttpClient {
 public:
  std::string Get(const std::string& url);
  void Post(const std::string& url, const std::string& data);
};

class UserService {
 private:
  HttpClient m_httpClient;
};
```

UserService内部通常会有一些方法去操作m_httpClient，这种方式简单粗暴，但是UserService与具体的HttpClient相耦合，导致难以替换依赖对象，也难以进行单元测试。

于是引出了后面两个方法。

## 2.接口依赖注入

我们的目标是期望测试的时候很方便的测试，而不需要创建一个具体的类，可以由用户mock出一个类去测试，那么我们可以想到接口，将HttpClient抽出来，得到：

```cpp
struct IHttpClient {
  virtual std::string Get(const std::string& url) = 0;
  virtual void Post(const std::string& url, const std::string& data) = 0;
  virtual ~IHttpClient() = default;
};
class UserService {
 private:
  std::unique_ptr<IHttpClient> m_httpClient;
};

```

那么在测试的时候便可以mock出一个client进行测试，例如：

```cc
class MockHttpClient : public IHttpClient {
public:
  MOCK_METHOD(std::string, Get, (const std::string& url), (override));
  MOCK_METHOD(void, Post, (const std::string& url, const std::string& data), (override));
};

auto mockHttpClient = std::make_unique<MockHttpClient>();
UserService userService(std::move(mockHttpClient));
```

经过以上改造，我们可以做到：

- 在 .cc 文件中隐藏实现细节
- 针对抽象接口工作
- 在对象构造期间注入依赖项

## 3.模版依赖注入

模板依赖注入通过C++模板技术，将依赖对象作为模板参数传递给`UserService`。在实例化模板时，指定具体的HTTP客户端实现。这种方法在编译时解决依赖问题，无需虚函数调用，提高了性能。同时，也使得`UserService`类可以灵活地与不同的HTTP客户端实现进行集成，并且易于进行单元测试。

```cpp
template <typename THttpClient>
class UserService {
 private:
  THttpClient m_httpClient;
};
```

在这种情况下比较好测试，例如：

```cpp
UserService<MockHttpClient> userService;
```

这种方式有如下好处：

- 在模板实例化期间注入依赖项
- 无需虚拟调用（注意 THttpClient 不是接口，因此可以在编译时解析调用）

以上便是本节要讲的所有内容，本节完整代码放于星球，欢迎订阅下载！