# please run `bazel run //learn_class/modern_cpp_30/constexpr:test3`
# please run `bazel run //learn_class/modern_cpp_30/constexpr:newconstexpr`
# please run `bazel run //learn_class/modern_cpp_30/constexpr:container`
# please run `bazel run //learn_class/modern_cpp_30/constexpr:sqrt`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "test3",
    srcs = ["test3.cpp"],
    copts = ["-std=c++17"],
)
cc_binary(
    name = "newconstexpr",
    srcs = ["newconstexpr.cpp"],
    copts = ["-std=c++17"],
)
cc_binary(
    name = "container",
    srcs = ["container.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "//learn_class/modern_cpp_30/container1:output_container",
    ],
)
cc_binary(
    name = "sqrt",
    srcs = ["sqrt.cpp"],
    copts = ["-std=c++17"],
)
