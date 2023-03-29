# please run `bazel run basic_content/static:nostatic_class`
# please run `bazel run basic_content/static:static_class`
# please run `bazel run basic_content/static:static_demo`
# please run `bazel run basic_content/static:static_error_variable`
# please run `bazel run basic_content/static:static_funciton`
# please run `bazel run basic_content/static:static_variable`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "nostatic_class",
    srcs = ["nostatic_class.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "static_class",
    srcs = ["static_class.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "static_demo",
    srcs = ["static_demo.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "sstatic_error_variable",
    srcs = ["static_error_variable.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "static_funciton",
    srcs = ["static_funciton.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "static_variable",
    srcs = ["static_variable.cpp"],
    copts = ["-std=c++11"]
)