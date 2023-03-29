# please run `bazel run basic_content/inline:inline_virtual`
# please run `bazel run basic_content/inline:main`
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "inline",
    hdrs = ["inline.h"],
)

cc_binary(
    name = "inline_virtual",
    srcs = ["inline_virtual.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "main",
    srcs = ["inline.cpp"],
    deps = [
        ":inline",
    ],
    copts = ["-std=c++11"]
)