# please run `bazel run //learn_class/modern_cpp_30/container1:container`
# please run `bazel run //learn_class/modern_cpp_30/container1:vector_l`

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "output_container",
    hdrs = ["output_container.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "container",
    srcs = ["container.cpp"],
    copts = ["-std=c++17"],
    deps = [
        ":output_container",
    ],
)
cc_binary(
    name = "vector_l",
    srcs = ["vector_l.cpp"],
    copts = ["-std=c++17"],
    deps = [
        ":output_container",
    ],
)
