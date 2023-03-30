# please run `bazel run //learn_class/modern_cpp_30/RAII:stack`
# please run `bazel run //learn_class/modern_cpp_30/RAII:heap`
# please run `bazel run //learn_class/modern_cpp_30/RAII:RAII`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "stack",
    srcs = ["stack.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "heap",
    srcs = ["heap.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "RAII",
    srcs = ["RAII.cpp"],
    copts = ["-std=c++11"],
)
