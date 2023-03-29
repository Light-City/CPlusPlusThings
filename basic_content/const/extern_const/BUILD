# please run `bazel run basic_content/const/extern_const:const_file1` and `# please run `bazel run basic_content/const/extern_const:file`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "const_file1",
    srcs = ["const_file1.cpp", "const_file2.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "file",
    srcs = ["file1.cpp", "file2.cpp"],
    copts = ["-std=c++11"]
)