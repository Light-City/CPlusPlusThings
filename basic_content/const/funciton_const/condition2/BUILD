# please run `bazel run basic_content/const/funciton_const/condition2:condition1`
# please run `bazel run basic_content/const/funciton_const/condition2:condition2`
# please run `bazel run basic_content/const/funciton_const/condition2:condition3`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "condition1",
    srcs = ["condition1.cpp"],
    copts = ["-std=c++11"]
)

# Don't panic if you get compilation errors, this is what this code demonstrates, as expected.
cc_binary(
    name = "condition2",
    srcs = ["condition2.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "condition3",
    srcs = ["condition3.cpp"],
    copts = ["-std=c++11"]
)