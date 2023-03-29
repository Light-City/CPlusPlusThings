# please run `bazel run //practical_exercises/10_day_practice/day4/copy_ctor:clock`

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "clock",
    hdrs = ["clock.h"],
)

cc_binary(
    name = "main",
    srcs = ["clock.cpp"],
    deps = [
        ":clock",
    ],
)
