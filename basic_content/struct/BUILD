# please run `bazel run basic_content/struct:ext_struct_func`
# please run `bazel run basic_content/struct:struct_func_func`
# please run `bazel run basic_content/struct:struct_func`
# please run `bazel run basic_content/struct:struct_func_cpp`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "ext_struct_func",
    srcs = ["ext_struct_func.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "struct_func_func",
    srcs = ["struct_func_func.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "struct_func",
    srcs = ["struct_func.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "struct_func_cpp",
    srcs = ["struct_func.cpp"],
    copts = ["-std=c++11"]
)