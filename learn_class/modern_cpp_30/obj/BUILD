# please run `bazel run //learn_class/modern_cpp_30/obj:obj1`
# please run `bazel run //learn_class/modern_cpp_30/obj:obj4`
# please run `bazel run //learn_class/modern_cpp_30/obj:obj5`
# please run `bazel run //learn_class/modern_cpp_30/obj:obj3`
# please run `bazel run //learn_class/modern_cpp_30/obj:all`
# please run `bazel run //learn_class/modern_cpp_30/obj:obj2`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "obj1",
    srcs = ["obj1.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "obj4",
    srcs = ["obj4.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "obj5",
    srcs = ["obj5.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "obj3",
    srcs = ["obj3.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "all",
    srcs = ["all.cpp"],
    copts = ["-std=c++11"],
)
cc_binary(
    name = "obj2",
    srcs = ["obj2.cpp"],
    copts = ["-std=c++11"],
)
