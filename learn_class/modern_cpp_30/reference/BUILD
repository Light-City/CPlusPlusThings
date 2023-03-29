# please run `bazel run //learn_class/modern_cpp_30/reference:reference`
# please run `bazel run //learn_class/modern_cpp_30/reference:forward`
# please run `bazel run //learn_class/modern_cpp_30/reference:ref`
# please run `bazel run //learn_class/modern_cpp_30/reference:collapses`
# please run `bazel run //learn_class/modern_cpp_30/reference:lifetime`

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "shape",
    hdrs = ["shape.h"],
)

cc_binary(
    name = "reference",
    srcs = ["reference.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "forward",
    srcs = ["forward.cpp"],
    copts = ["-std=c++11"],
    deps = [
        ":shape",
    ],
)
cc_binary(
    name = "ref",
    srcs = ["ref.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "collapses",
    srcs = ["collapses.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "lifetime",
    srcs = ["lifetime.cpp"],
    copts = ["-std=c++11"],
    deps = [
        ":shape",
    ],
)
