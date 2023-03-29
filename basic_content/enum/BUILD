# please run `bazel run basic_content/enum:classic_practice`
# please run `bazel run basic_content/enum:tradition_color`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "classic_practice",
    srcs = ["classic_practice.cpp"],
    copts = ["-std=c++11"]
)

# Don't panic if you get compilation errors, this is what this code demonstrates, as expected.
cc_binary(
    name = "tradition_color",
    srcs = ["tradition_color.cpp"],
    copts = ["-std=c++11"]
)