> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/bazel-build-guide/) 的文章，由站点同步脚本导出。

# Bazel 构建 C++ 项目

```
gpadmin@gpadmin:~$ ./bazel-6.1.1-installer-linux-x86_64.sh --user
Bazel installer
---------------

Bazel is bundled with software licensed under the GPLv2 with Classpath exception.
You can find the sources next to the installer on our release page:
   https://github.com/bazelbuild/bazel/releases

# 

## Build information
   - [Commit](https://github.com/bazelbuild/bazel/commit/1a90fe5)
Uncompressing......WARNING: Invoking Bazel in batch mode since it is not invoked from within a workspace (below a directory having a WORKSPACE file).
Extracting Bazel installation...
.

Bazel is now installed!

Make sure you have "/home/gpadmin/bin" in your path.

For bash completion, add the following line to your :
  source /home/gpadmin/.bazel/bin/bazel-complete.bash

For fish shell completion, link this file into your
/home/gpadmin/.config/fish/completions/ directory:
  ln -s /home/gpadmin/.bazel/bin/bazel.fish /home/gpadmin/.config/fish/completions/bazel.fish

See http://bazel.build/docs/getting-started.html to start a new project!
```



默认安装到home/bin



```
gpadmin@gpadmin:~$ ls ~/bin/
bazel
```





1. 在安装过程中，Bazel 可能会提示您添加环境变量。如果是这样，请根据提示编辑您的 `~/.bashrc` 文件并将以下行添加到文件末尾：

```
复制代码export PATH="$PATH:$HOME/bin"
```

1. 重新加载 `.bashrc` 文件以使更改生效：

```
复制代码source ~/.bashrc
```