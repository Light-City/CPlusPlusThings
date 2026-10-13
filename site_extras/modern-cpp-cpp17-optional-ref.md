> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/optional-ref/) 的文章，由站点同步脚本导出。

# C++17 optional_ref

This should have been part 2 of my [comparison series](https://www.foonathan.net/blog/2018/06/20/equivalence-relations.html), and I have almost finished it, but due to university stuff I just haven’t found the time to polish it.

But the optional discussion started again, so I just wanted to *really* quickly share my raw thoughts on the topic. In case you are lucky and don’t know what I mean: `std::optional<T&>` doesn’t compile right now, because the behavior of assignment wasn’t clear (even though it actually is). There are basically four questions in the discussion I want to answer:

1. Is `std::optional<T&>` the same as a pointer?
2. Do we need `std::optional<T&>`?
3. Should the assignment operator rebind or assign through?
4. Should it even have an assignment operator?

tl;dr: no, I don’t, rebind, no.



## 1. Is `std::optional<T&>` the same as a pointer?

What does it even mean to have an “optional `T&`”? Well, it is a `T&` that can also be `nullptr`.

So a pointer, a `T*`?

No, not really.

There is a more important difference between `T&` and `T*` besides the nullability: A `T&` has implicit creation and access, a `T*` explicit creation and access.

If you have an object, you can just silently bind a reference to it. And if you have a reference, you can just treat it *as if* it was the object. Whereas for pointers, you need to explicitly use `&obj` and `*ptr`.

And this difference is huge: It means `const T&` can be used for function parameters without any additional syntax issues:

```cpp
void print(const T& obj);
…
T obj = …;
print(obj);
```

You wouldn’t want to use a `const T*` as now the call side has to do extra work, it has to use the unnecessary `&obj`. This is just awkward.

> It is a different situation if the function might modify the object through the pointer/reference, or store a pointer/reference in a persistent location.

So naturally, if you want to have an optional argument, you wouldn’t want to use a pointer for the same reason: Why now introduce unnecessary syntactic overhead? It shouldn’t matter to the caller.

So `std::optional<T&>` is not the same as `T*`: It would have implicit creation syntax, not explicit.

What `std::optional<T&>` cannot have, however, is implicit access. Not only is it not implementable currently, it is also fundamentally impossible: For `std::optional<T&>` to have implicit access syntax, every operation on it would delegate to the referring object. This includes checking whether it refers to an object! Any `.has_value()` or `!opt` would forward to the referring object. This means that an explicit syntax is required, otherwise you’re just checking whether the optional has an object that is null.

A more thorough analysis can be found in the first twenty minutes of my [Rethinking Pointers](https://jonathanmueller.dev/talk/cppnow2018) talk at C++Now earlier this year.

## 2. Do we need `std::optional<T&>`?

As `std::optional<T&>` isn’t the same as `T*`, we need to look at the situations where we use `T&` and think about whether we need an optional version there.

Luckily, I did exactly that in my [Rethinking Pointers](https://jonathanmueller.dev/talk/cppnow2018) talk.

### Function Parameters

```cpp
void print(const T& obj);
void sort(Container& cont);
```

Here we want to either avoid a copy, or modify an argument in-place. If we want to have optional arguments, a `std::optional<T&>` is a solution. However, simply overloading the function works as well.

### Getter Functions

```cpp
const std::string& person::name() const;
```

Again, we want to avoid a copy.

If the returned value might not be available, we could just use non-reference `std::optional`, but have to pay for an additional copy. Or we could narrow the contact and add a precondition requiring the object to be there, but this is less type safe.

### LValue Functions

```cpp
T& std::vector::operator[](std::size_t index);
T& std::optional<T>::value();
```

Here we *absolutely* need an lvalue as a return type. This is the motivation behind references, so we use them. However, optional references wouldn’t work – we’d loose implicit access, which is incompatible with the conventional usage of operators.

### Range-based `for` loops

```cpp
for (auto& cur : container)
  …
```

Here optional references are not required.

### Lifetime extension when calling a function (experts only):

```cpp
const std::string& name = p.name();
// use `name` multiple times
```

Lifetime extension only works with normal references.

------

That’s it, that are all the situations where you should use a `T&`. The only situations where it might be feasible to have a `std::optional<T&>` are function parameters and getters where we want to avoid a copy. This is not such a compelling use-case.

## 3. Should the assignment operator rebind or assign through?

Assignment *fundamentally* is an optimization of copy. It should just do the same thing as “destroy the current object” and “copy a new one over”.

So when we write `opt_a = opt_b`, it will modify `opt_a` so it is a copy of `opt_b`. This is true for all `T`, including `T&`: If `opt_b` is a reference to `my_obj`, then `opt_a` will also be a reference to `my_obj`, even it was a reference to `other_obj` before. So the copy assignment operator does a rebind operation.

Now `std::optional` also has an assignment operator taking a `T`: This assignment operator is an optimization of the constructor taking a `T`.

> Read more about assignment as an optimization in [this blog post](https://www.foonathan.net/blog/2017/10/11/explicit-assignment.html) I’ve written some time ago.

As such, it will destroy the current object, if there is any, and then create the new object inside it. However, as it is an optimization, it will use `T::operator=` if the optional has a value already. The assignment operator of `T` might be more efficient than “destroy” followed by “construct”.

But note that it only does that, because it assumes that the assignment operator of `T` is an optimization of copy! If you provide a `T` where `rocket = launch` means “launch the rocket” this will fail. But this isn’t optional’s fault, your type is just stupid!

And one such stupid type is `T&`: The assignment operator of `T&` is not an optimization of “destroy” followed by “copy”. This is because references *have no assignment operator*: Every operation you do on a reference is actually done on the object it refers to. This includes assignment, so the assignment operator will assign the value, it assigns through.

Now some people think that having that behavior in the `operator=` of `optional<T&>` itself is even a possibility they need to consider.

It isn’t.

It absolutely isn’t.

Ignoring any other counter argument, those semantics would lead to confusion as `operator=` would do completely different things depending on the state of the `optional`!

```cpp
std::optional<T&> opt = …;

T obj;
opt = obj;
// if opt was empty before, it will now refer to obj
// if opt wasn't empty before, it will now refer to an object with the same value as obj

return opt; // so this is legal only if the optional wasn't empty before
```

There is no precedent for an assignment operator that behaves like this, because an assignment operator shouldn’t behave like this.

## 4. Should it even have an assignment operator?

Whenever we use a `T&` we don’t need to modify the reference itself – after all, we can’t. So when we replace the `T&` with a `std::optional<T&>` there is no need to mutate the `std::optional<T&>` either.

Now the “assign through” people of `std::optional<T&>` argue that this behavior is consistent with `T&`.

It isn’t, as references aren’t assignable.

Sure, writing `ref = obj` *compiles*, but it is not an assignment. It only works because *every* operation done on a reference is done on the object it refers to.

Now as I said before, when we have a nullable reference we can’t do that, because then we would have no syntax to check for nullability. So the only way to be truly consistent with `T&` would be if `std::optional<T&>` would have no modifying operators. It shouldn’t have an `operator=`, an `emplace()` function, etc. After all, `T&` is immutable, so `std::optional<T&>` should be as well.

If you are in a situation where you need to mutate a `std::optional<T&>`, you didn’t *want* an `std::optional<T&>`, you wanted a pointer. Because then you store the optional in a persistent location and should have used an explicit creation syntax to make it obvious. More on that in [my talk](https://jonathanmueller.dev/talk/cppnow2018).

Note that, if you have a `std::optional<T&>` without modifiers, it behaves nothing like an `std::optional<T>` – because a `T&` behaves nothing like a `T`. Just like generic code can’t handle `T&`, it also wouldn’t handle `std::optional<T&>`.

So we shouldn’t spell “optional `T&`” as `std::optional<T&>`, it should be spelt differently. I’d argue it should be called `std::optional_arg<T>`, because that reflects the actual use case it’s going to get.

## Conclusion

In my opinion we don’t need `std::optional<T&>`. It is a weird type with only very few use cases.

If the committee decides that adding `std::optional<T&>` is worth the effort, it should be an immutable `std::optional`, just like references are. For the actual uses cases of `std::optional<T&>`, just like the use cases of `T&`, it doesn’t actually matter.

Note that a type that behaves like a `T*`, but isn’t, is useful: A `T*` can do a lot of different things, so it might be a good idea to add a distinct type that explicitly models just one of the things it does. In my [type_safe](https://github.com/foonathan/type_safe) library, for example, I have an `ts::optional_ref<T>`, which is like a `T*` and not like a nullable `T&`. However, it definitely shouldn’t be spelled `std::optional<T&>`, because it is not a `T&`.

More details, again, in my [Rethinking Pointers](https://jonathanmueller.dev/talk/cppnow2018) talk.