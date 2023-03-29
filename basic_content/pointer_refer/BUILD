# please run `bazel run basic_content/pointer_refer:copy_construct`
# please run `bazel run basic_content/pointer_refer:effec`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "copy_construct",
    srcs = ["copy_construct.cpp"],
    copts = ["-std=c++11", "-fno-elide-constructors"]
)

cc_binary(
    name = "effec",
    srcs = ["effec.cpp"],
    copts = ["-std=c++11"]
)