# please run `bazel run basic_content/explicit:explicit`
load("@rules_cc//cc:defs.bzl", "cc_binary")

# Don't panic if you get compilation errors, this is what this code demonstrates, as expected.
cc_binary(
    name = "explicit",
    srcs = ["explicit.cpp"],
    copts = ["-std=c++11"]
)