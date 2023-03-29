# please run `bazel run basic_content/const/class_const/overload_example:main`
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "apple",
    srcs = ["apple.cpp"],
    hdrs = ["apple.h"],
)

# Don't panic if you get compilation errors, this is what this code demonstrates, as expected.
cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        ":apple",
    ],
    copts = ["-std=c++11"]
)