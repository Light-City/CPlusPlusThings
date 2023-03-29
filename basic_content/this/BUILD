# please run `bazel run basic_content/this:person`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "person",
    srcs = ["person.cpp"],
    copts = ["-std=c++11"]
)