# please run `bazel run basic_content/const:const_function`
# please run `bazel run basic_content/const:const_num`
load("@rules_cc//cc:defs.bzl", "cc_binary")

# Don't panic if you get compilation errors, this is what this code demonstrates, as expected.
cc_binary(
    name = "const_function",
    srcs = ["const_function.cpp"],
    copts = ["-std=c++11"]
)

# Don't panic if you get compilation errors, this is what this code demonstrates, as expected.
cc_binary(
    name = "const_num",
    srcs = ["const_num.cpp"],
    copts = ["-std=c++11"]
)