# please run `bazel run basic_content/macro:do_while`
# please run `bazel run basic_content/sig_examp:sig_examp`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "do_while",
    srcs = ["do_while.cpp"],
    copts = ["-std=c++11"],
)

cc_binary(
    name = "sig_examp",
    srcs = ["sig_examp.cpp"],
    copts = ["-std=c++11"],
)
