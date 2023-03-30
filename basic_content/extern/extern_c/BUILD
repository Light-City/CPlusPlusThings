# please run `bazel run basic_content/extern/extern_c:main`
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "add",
    srcs = ["add.cpp"],
    hdrs = ["add.h"],
)

cc_binary(
    name = "main",
    srcs = ["add.c"],
    deps = [":add"],
)