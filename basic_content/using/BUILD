# please run `bazel run basic_content/using:derived_base`
# please run `bazel run basic_content/using:using_derived`
# please run `bazel run basic_content/using:using_global`
# please run `bazel run basic_content/using:using_typedef`
load("@rules_cc//cc:defs.bzl", "cc_binary")

# Don't panic if you get compilation errors, this is what this code demonstrates, as expected.
cc_binary(
    name = "derived_base",
    srcs = ["derived_base.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "using_derived",
    srcs = ["using_derived.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "using_global",
    srcs = ["using_global.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "using_typedef",
    srcs = ["using_typedef.cpp"],
    copts = ["-std=c++11"]
)