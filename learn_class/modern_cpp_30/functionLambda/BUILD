# please run `bazel run //learn_class/modern_cpp_30/functionLambda:adder`
# please run `bazel run //learn_class/modern_cpp_30/functionLambda:function`
# please run `bazel run //learn_class/modern_cpp_30/functionLambda:autoLambda`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "adder",
    srcs = ["adder.cpp"],
    copts = ["-std=c++17"],
)
cc_binary(
    name = "function",
    srcs = ["function.cpp"],
    copts = ["-std=c++17"],
)
cc_binary(
    name = "autoLambda",
    srcs = ["autoLambda.cpp"],
    copts = ["-std=c++17"],
)
