# please run `bazel run //practical_exercises/10_day_practice/day5/inherit_access:public`
# please run `bazel run //practical_exercises/10_day_practice/day5/inherit_access:protected_inherit`
# please run `bazel run //practical_exercises/10_day_practice/day5/inherit_access:protected`
# please run `bazel run //practical_exercises/10_day_practice/day5/inherit_access:private`

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "public",
    srcs = ["public.cpp"],
)
cc_binary(
    name = "protected_inherit",
    srcs = ["protected_inherit.cpp"],
)
cc_binary(
    name = "protected",
    srcs = ["protected.cpp"],
)
cc_binary(
    name = "private",
    srcs = ["private.cpp"],
)
