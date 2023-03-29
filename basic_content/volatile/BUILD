# please run `bazel run basic_content/volatile:noopt_vola`
# please run `bazel run basic_content/volatile:volatile`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "noopt_vola",
    srcs = ["noopt_vola.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "volatile",
    srcs = ["volatile.cpp"],
    copts = ["-std=c++11"]
)