# please run `bazel run //concurrency/Threading_In_CPlusPlus/3.join_detach:join`
# please run `bazel run //concurrency/Threading_In_CPlusPlus/3.join_detach:detach`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "join",
    srcs = ["join.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "detach",
    srcs = ["detach.cpp"],
    copts = ["-std=c++11"],
)
