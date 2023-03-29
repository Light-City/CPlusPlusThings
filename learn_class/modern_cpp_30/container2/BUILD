# please run `bazel run //learn_class/modern_cpp_30/container2:relacontainer`
# please run `bazel run //learn_class/modern_cpp_30/container2:priority_queue`
# please run `bazel run //learn_class/modern_cpp_30/container2:hash`
# please run `bazel run //learn_class/modern_cpp_30/container2:array`
# please run `bazel run //learn_class/modern_cpp_30/container2:unorder`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "relacontainer",
    srcs = ["relacontainer.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "//learn_class/modern_cpp_30/container1:output_container",
    ],
)
cc_binary(
    name = "priority_queue",
    srcs = ["priority_queue.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "//learn_class/modern_cpp_30/container1:output_container",
    ],
)
cc_binary(
    name = "hash",
    srcs = ["hash.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "//learn_class/modern_cpp_30/container1:output_container",
    ],
)
cc_binary(
    name = "array",
    srcs = ["array.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "//learn_class/modern_cpp_30/container1:output_container",
    ],
)
cc_binary(
    name = "unorder",
    srcs = ["unorder.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "//learn_class/modern_cpp_30/container1:output_container",
    ],
)
