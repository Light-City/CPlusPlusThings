# please run `bazel run //learn_class/modern_cpp_30/literalAssert:literal`
# please run `bazel run //learn_class/modern_cpp_30/literalAssert:assert`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "literal",
    srcs = ["literal.cpp"],
    copts = ["-std=c++17"],
)
cc_binary(
    name = "assert",
    srcs = ["assert.cpp"],
    copts = ["-std=c++11"],
)