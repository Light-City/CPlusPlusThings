# please run `bazel run basic_content/sizeof:blackclass`
# please run `bazel run basic_content/sizeof:genA`
# please run `bazel run basic_content/sizeof:geninhe`
# please run `bazel run basic_content/sizeof:moreinhe`
# please run `bazel run basic_content/sizeof:morevir`
# please run `bazel run basic_content/sizeof:static`
# please run `bazel run basic_content/sizeof:virinhe`
# please run `bazel run basic_content/sizeof:virmoreinhe`
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "blackclass",
    srcs = ["blackclass.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "genA",
    srcs = ["genA.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "geninhe",
    srcs = ["geninhe.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "moreinhe",
    srcs = ["moreinhe.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "static",
    srcs = ["static.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "virinhe",
    srcs = ["virinhe.cpp"],
    copts = ["-std=c++11"]
)

cc_binary(
    name = "virmoreinhe",
    srcs = ["virmoreinhe.cpp"],
    copts = ["-std=c++11"]
)