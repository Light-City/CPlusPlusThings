# please run `bazel run //concurrency/concurrency_v1/chapter2:2.4_runtime`
# please run `bazel run //concurrency/concurrency_v1/chapter2:2.1_basic`
# please run `bazel run //concurrency/concurrency_v1/chapter2:2.3_ownership`
# please run `bazel run //concurrency/concurrency_v1/chapter2:2_5_id`
# please run `bazel run //concurrency/concurrency_v1/chapter2:2.2_transfer`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "2.4_runtime",
    srcs = ["2.4_runtime.cpp"],
)
cc_binary(
    name = "2.1_basic",
    srcs = ["2.1_basic.cpp"],
)
cc_binary(
    name = "2.3_ownership",
    srcs = ["2.3_ownership.cpp"],
)
cc_binary(
    name = "2_5_id",
    srcs = ["2_5_id.cpp"],
    copts = ["-std=c++11"]
)
cc_binary(
    name = "2.2_transfer",
    srcs = ["2.2_transfer.cpp"],
    copts = ["-std=c++11"]
)
