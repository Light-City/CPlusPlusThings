# please run `bazel run //concurrency/Threading_In_CPlusPlus/1.thread:intro`
# please run `bazel run //concurrency/Threading_In_CPlusPlus/1.thread:thread`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "intro",
    srcs = ["intro.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "thread",
    srcs = ["thread.cpp"],
    copts = ["-std=c++11"],
)
