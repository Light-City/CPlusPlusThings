# you can run some main program, just replace binary name
# such as: `bazel run basic_content/assert:ignore_assert`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "ignore_assert",
    srcs = ["ignore_assert.c"],
)

cc_binary(
    name = "assert",
    srcs = ["assert.c"],
)
