# please run `bazel run basic_content/extern/extern_c++:main`
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "add",
    srcs = ["add.c"],
    hdrs = ["add.h"],
)

cc_binary(
    name = "main",
    srcs = ["add.cpp"],
    deps = [":add"],
    copts = ["-std=c++11"]
)