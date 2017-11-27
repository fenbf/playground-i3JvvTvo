# Enforcing code contracts with [[nodiscard]]

[![nodiscard attribute C++17](https://dl.dropbox.com/s/b2o6zijwz6tunt3/nodiscard.png?dl=0)](http://www.bfilipek.com/2017/11/nodiscard.html)

For my article series about C++17 features, I've made a separate entry about new attributes. At first sight, I thought that `[[nodiscard]]` is just another simple and a rarely used thing. But later I thought... hmmm... maybe it might be valuable?

One reason is that `[[nodiscard]]` might be handy when enforcing code contracts. That way you won't miss an important return value.

Let's look at a few examples.

## Intro

`[[nodiscard]]`, as mentioned in my article: [C++17 in detail: Attributes](http://www.bfilipek.com/2017/07/cpp17-in-details-attributes.html), is used to mark the return value of functions:

```cpp
[[nodiscard]] int Compute();
```

When you call such function and don't assign the result:

```cpp
void Foo() {
    Compute();
}
```

You should get the following (or a similar) warning:

```
warning: ignoring return value of 'int Compute()', 
declared with attribute nodiscard
```

We can go further and not just mark the return value, but a whole type:

```cpp
[[nodiscard]] struct SuperImportantType { }
SuperImportantType CalcSuperImportant();
SuperImportantType OtherFoo();
SuperImportantType Calculate();
```

and you'll get a warning whenever you call any function that returns `SuperImportantType `.

In other words, you can enforce the code contract for a function, so that the caller won't skip the returned value. Sometimes such omission might cause you a bug, so using `[[nodiscard]]` will improve code safety.

You can play with some code below:

@[Simple Example]({"stubs": ["cpp_nodiscard_simple.cpp"],"command": "sh ./run.sh cpp_nodiscard_simple.cpp"})

**One note:** You'll get an warning, but usually it's a good practice to enable "treat warnings as errors" when building the code. `/WX` in MSVC or `-Werror` in GCC.

I hope you get the basic idea. But the above stuff is just some generic `FooBar` code. Are there any useful cases?

## Where can it be used?

Attributes are a standardized way of annotating the code. They are optional, but might the compiler to optimize code, detect possible errors or just be more specific about the intentions.

Here are a few places where `[[nodiscard]]` can be potentially handy:

### Errors

One, crucial use case are error codes.

How many times have you forgotten to check a returned error code from a function? (Crucial if you don't rely on exceptions).

Here's some code:

```cpp
enum class [[nodiscard]] ErrorCode {
    OK,
    Fatal,
    System,
    FileIssue
};
```

And if we have several functions:

```cpp
ErrorCode OpenFile(std::string_view fileName);
ErrorCode SendEmail(std::string_view sendto,
                    std::string_view text);
ErrorCode SystemCall(std::string_view text);
```

Now every time you'd like to call such functions you're "forced" to check the return value. I often see code where a developer checks only some return error codes. That creates inconsistencies and can lead to some severe runtime errors. 

You think your method is doing fine (because N (of M) called functions returned `OK`), but something still is failing. You check it with the debugger, and you notice that Y function returns `FAIL` and you haven't checked it.

Should you mark the error type or maybe some essential functions only? 

For error codes that are visible through the whole application that might be the right thing to go.  Of course when your function returns just `bool` then you can only mark the function, and not the type (or you can create a `typedef`/alias and then mark it with `[[nodiscard]]`).

here's the code sample:

@[Errors]({"stubs": ["cpp_nodiscard_errors.cpp"],"command": "sh ./run.sh cpp_nodiscard_errors.cpp"})


### Factories / Handles

Another important type of functions where `[[nodiscard]]` adds a value are "factories".

Every time you call "make/create/build" you don't want to skip the returned value. Maybe that's a very obvious thing, but there's a possibility (especially when doing some refactoring), to forget, or comment out.

```cpp
[[nodiscard]] Foo MakeFoo();
```

So what about `new` from the Standard Library? It's also a 'factory' ... isn't it? Read further, please :)

### When returning non-trivial types? 

What about such code:

```cpp
std::vector<std::string> GenerateNames();
```

The returned type seems to be `heavy`, so usually, it means that you have to use it later. On the other hand, even `int` might be heavy regarding semantics of the given problem.

### No side effects

The code in the previous section is also an example of a function with no side effects. In that case, we need to do something with the returned value. Otherwise, the function call can be removed/optimized from the code.

Hmmm... so maybe we should use `[[nodiscard]]`...

### Everywhere?!

Ok, let's not be that crazy... using returned value from a function is usually useful and developers do assign to a variable, so we cannot assume that all the cases should be marked `[[nodiscard]]`.

There's a paper that might be a "guide" [P0600R0 - [[nodiscard]] in the Library](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0600r0.pdf)

I think it didn't make into C++17, but it suggests the following:
(update: this paper was recently voted into C++20, so not in C++17)

> We suggest a conservative approach:  
> It should be added where: 
> 
> * For existing API’s   
>   *  not using the return value always is a “huge mistake” (e.g. always resulting in resource leak)    
>   *   not using the
> return value is a source of trouble and easily can happen (not obvious
> that something is wrong)   
> * For new API’s (not been in the C++ standard yet)
>     * not using the return value is usually an error.

Here are a few examples where the new attribute should be added:

* `malloc()`/`new`/`allocate` - expensive call, usually not using the return value is a resource leak 
* `std::async()` - not using the return value makes the call synchronous, which might be hard to detect.  

On the other hand such function as `top()` is questionable, as "not very useful, but no danger and such code might exist"

So for now, I wouldn't add `[[nodiscard]]` in all places of your code but focus on the critical places. Possibly, as mentioned before, error codes and factories are a good place to start.

Moreover, it's useful when you want to discourage using some functions: :) have a look below:

### How to discourage using `printf` :)

As `printf` is not the safest method of printing text, and there's a idea to move to a safer approach like `iostreams`. 

So maybe we can discourage printf?

I got that idea from [Björn Fahller](https://twitter.com/bjorn_fahller) @Twitter:

<blockquote class="twitter-tweet" data-lang="en"><p lang="en" dir="ltr">[[nodiscard]] on return from printf would annoy many... But it may make people stop using it, which would be good. :-)</p>&mdash; Björn Fahller (@bjorn_fahller) <a href="https://twitter.com/bjorn_fahller/status/926861878822948865?ref_src=twsrc%5Etfw">November 4, 2017</a></blockquote>
<script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

So we could mark it and then every time you'd like to use this code you'd have to write:

```cpp
if (printf(...) < 0)
    error();

// or at least:
[[maybe_unused]]auto ret = printf(...);
```

Not nice... right? :)

Ok, but let's not be that evil!

We all know printf is the core tool for debugging! :)

## How to ignore [[nodiscard]]

With `[[nodiscard]]` you should assign the return value to something, and you should also use this variable later. If you forget, you'll get another warning "unused value". 

To suppress the warning you can use another attribute from C++17: `[[maybe_unused]]`:

```cpp
[[nodiscard]] int Compute();
[[maybe_unused]] auto t = Compute();
```

Still, this is a hack to me, and usually in that case it's better to rethink the design a bit :)

## Before C++17

As I see, most of the attributes that went into the standardized `[[attrib]]` comes from compiler extensions.

For example, in GCC/Clang, you can use

```
__attribute__((warn_unused_result))
```

in MSVC:

`_Check_return_` - see more in [MSDN: Annotating Function Behavior](https://msdn.microsoft.com/en-us/library/jj159529.aspx).

## Summary

To be honest, I haven't used attributes much. Also, I haven't seen a lot of code that has attributes applied. It's probably natural because previously they were available only as an extension so used rarely. Now, since C++11 they went into the standard form, there's a chance to see them more often. We have to learn how to use them efficiently.

To sum up: `[[nodiscard]]` is an excellent addition to all the important code: public APIs, safety-critical systems, etc. Adding this attribute will at least enforce the code contract, and a compiler will help you detect bugs - at compile time, rather than finding in in the runtime.

For now, Core Guidelines also doesn't mention anything about `[[attribs]]`, but I hope to see some essential use cases described someday.

* Have you used nodiscard (in C++17 way or as an extension) before?
* What are your suggestions here?
* Would you mark all possible functions? :)

References

* [Proposal of unused, nodiscard and fallthrough attributes.](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0068r0.pdf)
* [C++ Weekly - Ep 30 - C++17's nodiscard Attribute](https://www.youtube.com/watch?v=l_5PF3GQLKc)
* [C++17 in detail: Attributes](http://www.bfilipek.com/2017/07/cpp17-in-details-attributes.html)

### Call to action:

This playground is adapted from my blog: [Bartek's coding blog: Enforcing code contracts with nodiscard](http://www.bfilipek.com/2017/11/nodiscard.html)

Visit the blog if you're looking for more good stuff about C++ :)

