## C++ 协程- 理解 co_await 操作符

本文章翻译自[AsymmetricTransfer协程系列文章](https://lewissbaker.github.io/2017/11/17/understanding-operator-co-await)

## 概述

在上一篇关于协程理论的文章中，我描述了函数和协程之间的高级差异，但没有详细介绍 C++ 协程 TS (N4680) 中描述的协程语法和语义。

Coroutines TS 添加到 C++ 语言的关键新功能是能够暂停协程，以便稍后恢复。 TS 提供的机制是通过新的 co_await 运算符来执行此操作。

了解 co_await 运算符的工作原理有助于揭开协程行为以及它们如何暂停和恢复的神秘面纱。在这篇文章中，我将解释 co_await
运算符的机制，并介绍相关的 Awaitable 和 Awaiter 类型概念。

但在深入研究 co_await 之前，我想简要概述一下 Coroutines TS，以提供一些背景信息。

## 协程 TS 给我们带来了什么？

- 三个关键字： co_await, co_yield, co_return
- 几个新的类型在 std 命名空间下（原文是 std::experimental)：
-
    - coroutine_handle<P>
-
    - coroutine_traits<Ts...>
-
    - suspend_always
-
    - suspend_never
- 库编写者可以用来与协程交互并自定义其行为的通用机制。
- 一种使编写异步代码变得更加容易的语言工具

C++ 协程 TS 在该语言中提供的功能可以被视为协程的低级汇编语言。这些工具可能很难以安全的方式直接使用，并且主要供库编写者使用来构建应用程序开发人员可以安全使用的更高级别的抽象。

该计划是将这些新的低级设施交付到即将推出的语言标准（希望是
C++20）中，以及标准库中的一些随附的高级类型，这些类型包装了这些低级构建块，并使协程在以下环境中更易于访问：为应用程序开发人员提供。

## 编译器 <-> 库交互

有趣的是，协程 TS 实际上并没有定义协程的语义。它没有定义如何生成返回给调用者的值。它没有定义如何处理传递给 co_return
语句的返回值或如何处理从协程传播的异常。它没有定义协程应该在哪个线程上恢复。

相反，它为库代码指定了一种通用机制，通过实现复合特定接口的类型来自定义协程的行为。然后，编译器生成调用库提供的类型实例上的方法的代码。这种方法类似于库编写者可以通过定义
begin()/end() 方法和迭代器类型来自定义基于范围的 for 循环的行为。

例如，您可以定义一个异步生成单个值的协程，或者一个延迟生成一系列值的协程，或者一个遇到一个optional<T> 的值为 nullopt
时提前退出来简化使用可选 <T> 值的控制流的协程。

协程 TS 定义了两种接口：Promise 接口和 Awaitable 接口。

Promise 接口指定了自定义协程本身行为的方法。库编写者能够自定义调用协程时发生的情况、协程返回时发生的情况（通过正常方式或通过未处理的异常）并自定义协程中任何
co_await 或 co_yield 表达式的行为。

Awaitable 接口指定控制 co_await 表达式语义的方法。当值为 co_awaited
时，代码会转换为对可等待对象上的方法的一系列调用，允许其指定：是否暂停当前协程，暂停后执行一些逻辑以安排协程稍后恢复，并在协程恢复后执行一些逻辑以产生
co_await 表达式的结果。

我将在以后的文章中详细介绍 Promise 接口，但现在让我们看看 Awaitable 接口。

## Awaiters 和 Awaitables：解释运算符 co_await

co_await 运算符是一个新的一元运算符，可以应用于值。例如：co_await someValue

co_await 运算符只能在协程上下文中使用。但这有点同义反复，因为根据定义，任何包含使用 co_await 运算符的函数体都将被编译为协程。

支持 co_await 运算符的类型称为 Awaitable 类型。

请注意，co_await 运算符是否可以应用于类型取决于 co_await 表达式出现的上下文。用于协程的 Promise 类型可以通过其
wait_transform 方法更改协程内 co_await 表达式的含义（稍后将详细介绍）。

更具体地说，在需要的地方，我喜欢使用术语“Normally Awaitable”来描述在协程上下文中支持 co_await 运算符的类型，而该协程上下文的
Promise 类型没有 wait_transform 成员。我喜欢使用术语 Contextually Awaitable 来描述仅在某些类型的协程上下文中支持 co_await
运算符的类型，因为协程的 Promise 类型中存在await_transform 方法。 （我愿意在这里为这些名字提供更好的建议......）

Awaiter 类型是一种实现作为 co_await 表达式的一部分调用的三个特殊方法的类型：await_ready、await_suspend 和await_resume。

请注意，我无耻地从 C# 异步关键字的机制中“借用”了术语“Awaiter”，该机制是根据 GetAwaiter() 方法实现的，该方法返回一个对象，其接口与
C++ 的 Awaiter 概念极其相似。

当编译器看到 co_await <expr> 表达式时，实际上可以将其转换为多种可能的内容，具体取决于所涉及的类型。

## Obtaining the Awaiter

编译器做的第一件事是生成代码来获取等待值的 Awaiter 对象。 N4680 第 5.3.8(3) 节中列出了获取等待者对象的多个步骤。

假设等待协程的 Promise 对象的类型为 P，并且 Promise 是对当前协程的 Promise 对象的左值引用。

句子
如果promise类型P有一个名为await_transform的成员，那么首先将<expr>传递到对promise.await_transform(<expr>)
的调用中以获取Awaitable值awaitable。否则，如果promise类型没有await_transform成员，那么我们直接使用<expr>
的计算结果作为Awaitable对象awaitable。

然后，如果 Awaitable 对象 awaitable 有一个适用的运算符 co_await() 重载，则调用它来获取 Awaiter 对象。否则，awaitable
对象将用作等待者对象。

如果我们将这些规则编码到函数 get_awaitable() 和 get_awaiter() 中，它们可能看起来像这样：

```c++

template <typename P, typename T>
decltype(auto) get_awaitable(P &promise, T &&expr) {
  if constexpr (has_any_await_transform_member_v<P>)
    return promise.await_transform(static_cast<T &&>(expr));
  else
    return static_cast<T &&>(expr);
}

template <typename Awaitable>
decltype(auto) get_awaiter(Awaitable &&awaitable) {
  if constexpr (has_member_operator_co_await_v<Awaitable>)
    return static_cast<Awaitable &&>(awaitable).operator co_await();
  else if constexpr (has_non_member_operator_co_await_v<Awaitable &&>)
    return operator co_await(static_cast<Awaitable &&>(awaitable));
  else
    return static_cast<Awaitable &&>(awaitable);
}
```

## Awaiting the Awaiter

因此，假设我们已经将 <expr> 结果转换为 Awaiter 对象的逻辑封装到上述函数中，那么 co_await <expr> 的语义可以（大致）翻译如下：

```c++
{
  auto &&value = <expr>;
  auto &&awaitable =
      get_awaitable(promise, static_cast<decltype(value)>(value));
  auto &&awaiter = get_awaiter(static_cast<decltype(awaitable)>(awaitable));
  if (!awaiter.await_ready()) {
    using handle_t = std::coroutine_handle<P>;

    using await_suspend_result_t =
        decltype(awaiter.await_suspend(handle_t::from_promise(p)));
    //    <suspend-coroutine>
    if constexpr (std::is_void_v<await_suspend_result_t>) {
      awaiter.await_suspend(handle_t::from_promise(p));
      // return to caller or resumer
    } else {
      static_assert(std::is_same_v<await_suspend_result_t, bool>,
                    "await_suspend() must return void or bool");

      if (awaiter.await_suspend(handle_t::from_promise(p))) {
        // return to caller or resumer
      }
      // resume-point
    }
  }
  return awaiter.await_resume();
}
```

（？？？ 上述似乎漏掉了一点，await_suspend(auto h) 还允许返回一个 协程句柄用于恢复协程）

当对await_suspend()的调用返回时，await_suspend()
的void返回版本无条件地将执行转移回协程的调用者/恢复者，而bool返回版本允许awaiter对象立即有条件地恢复协程，而不返回到呼叫者/恢复者。

当等待者可能启动有时可以同步完成的异步操作时，await_suspend() 的 bool 返回版本非常有用。在同步完成的情况下，await_suspend()
方法可以返回 false 来指示协程应立即恢复并继续执行。

在 <suspend-coroutine> 点，编译器生成一些代码来保存协程的当前状态并准备恢复。这包括存储 <resume-point>
的位置以及将寄存器中当前保存的任何值溢出到协程帧内存中.

一旦操作完成，await_suspend() 方法就有责任安排协程在将来的某个时刻恢复（或销毁）。请注意，从await_suspend() 返回 false
算作调度协程以在当前线程上立即恢复。

wait_ready() 方法的目的是让您在已知操作将同步完成而无需挂起的情况下避免 <suspend-coroutine> 操作的成本。

在 <return-to-caller-or-resumer> 点，执行被转移回调用者或​​恢复者，弹出本地堆栈帧，但保持协程帧处于活动状态。

当（或如果）挂起的协程最终恢复时，执行将在 <resume-point> 处恢复。 IE。紧接着调用await_resume() 方法以获取操作结果之前。

await_resume() 方法调用的返回值成为 co_await 表达式的结果。 wait_resume() 方法还可以引发异常，在这种情况下，异常会从
co_await 表达式中传播出去。

请注意，如果异常从await_suspend()
调用中传播出来，则协程将自动恢复，并且异常从co_await表达式中传播出来，而不调用await_resume()。

## 协程句柄

您可能已经注意到使用了 coroutine_handle<P> 类型，该类型传递给 co_await 表达式的await_suspend() 调用。

该类型表示协程框架的非拥有句柄，可用于恢复协程的执行或销毁协程框架。它还可用于访问协程的 Promise 对象。

coroutine_handle 类型具有以下接口：

```c++
// 原文是 std::experimental，这在 clang 中仍然适用
namespace std {
template <typename Promise> struct coroutine_handle;

template <> struct coroutine_handle<void> {
  bool done() const;
  void resume();
  void destory();

  void *address() const;
  static coroutine_handle from_address(void *address);
};

template <typename Promise> struct coroutine_handle : coroutine_handle<void> {
  Promise &promise() const;
  static coroutine_handle from_promise(Promise &promise);
  static coroutine_handle from_address(void *address);
};
} // namespace std
```

TODO...