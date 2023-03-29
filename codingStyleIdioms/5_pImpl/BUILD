# please run `bazel run //codingStyleIdioms/5_pImpl:pimpl`
# please run `bazel run //codingStyleIdioms/5_pImpl:pimplTime`
# please run `bazel run //codingStyleIdioms/5_pImpl:noPimpl`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "pimpl",
    srcs = ["pimpl.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "pimplTime",
    srcs = ["pimplTime.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "noPimpl",
    srcs = ["noPimpl.cpp"],
    copts = ["-std=c++11"],
)
