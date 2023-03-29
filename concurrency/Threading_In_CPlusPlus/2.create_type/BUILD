# please run `bazel run //concurrency/Threading_In_CPlusPlus/2.create_type:4.no_static_member_function`
# please run `bazel run //concurrency/Threading_In_CPlusPlus/2.create_type:2.lambda_function`
# please run `bazel run //concurrency/Threading_In_CPlusPlus/2.create_type:1.function_pointer`
# please run `bazel run //concurrency/Threading_In_CPlusPlus/2.create_type:3.functor`
# please run `bazel run //concurrency/Threading_In_CPlusPlus/2.create_type:5.static_member_function`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "4.no_static_member_function",
    srcs = ["4.no_static_member_function.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "2.lambda_function",
    srcs = ["2.lambda_function.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "1.function_pointer",
    srcs = ["1.function_pointer.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "3.functor",
    srcs = ["3.functor.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "5.static_member_function",
    srcs = ["5.static_member_function.cpp"],
    copts = ["-std=c++11"],
)
