# please run `bazel run basic_content/friend:friend_class`
# please run `bazel run basic_content/friend:friend_func`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "friend_class",
    srcs = ["friend_class.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "friend_func",
    srcs = ["friend_func.cpp"],
    copts = ["-std=c++11"]
)