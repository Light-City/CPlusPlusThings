# please run `bazel run //codingStyleIdioms/1_classInitializers:1.2_nodefault_ctor`
# please run `bazel run //codingStyleIdioms/1_classInitializers:initializer`
# please run `bazel run //codingStyleIdioms/1_classInitializers:1.1_nest`
# please run `bazel run //codingStyleIdioms/1_classInitializers:1.3_const`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "1.2_nodefault_ctor",
    srcs = ["1.2_nodefault_ctor.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "initializer",
    srcs = ["initializer.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "1.1_nest",
    srcs = ["1.1_nest.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "1.3_const",
    srcs = ["1.3_const.cpp"],
    copts = ["-std=c++11"],
)
