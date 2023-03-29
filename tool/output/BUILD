# please run `bazel run //tool/output:container`

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "output_container",
    hdrs = ["output_container.h"],
    copts = ["-std=c++17"],
)

cc_binary(
    name = "container",
    srcs = ["container.cpp"],
    deps = [
        ":output_container",
    ],
    copts = ["-std=c++17"],
)