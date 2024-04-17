#pragma once

#include <type_traits>
#include <utility>
#include <concepts>
#include <coroutine>

/**
 * @brief 使用 concept 来约束 coroutine
 */

namespace eon::coro {

template <typename A>
struct GetAwaiter : std::type_identity<A>
{
};

template <typename A>
    requires requires(A&& a) { std::forward<A>(a).operator co_await(); }
struct GetAwaiter<A>
    : std::type_identity<decltype(std::declval<A>().operator co_await())>
{
};

template <typename A>
    requires requires(A&& a) {
        operator co_await(std::forward<A>(a));
        requires !(requires { std::forward<A>(a).operator co_await(); });
    }

struct GetAwaiter<A>
    : std::type_identity<decltype(operator co_await(std::declval<A>()))>
{
};

template <typename A>
using GetAwaiter_t = typename GetAwaiter<A>::type;

/**
 * @brief awaitable 的概念
 * @tparam A
 */
template <typename A>
concept Awaitable = requires {
    typename GetAwaiter_t<A>;
    requires requires(GetAwaiter_t<A> awaiter, std::coroutine_handle<> handle) {
        { awaiter.await_ready() } -> std::convertible_to<bool>;
        awaiter.await_suspend(handle);
        awaiter.await_resume();
    };
};

/**
 * @brief 协程的 Future 概念
 * @tparam Fut
 */
template <typename Fut>
concept Future = requires(Fut fut) {
    // 不能默认初始化
    requires !std::default_initializable<Fut>;
    // 支持移动构造
    // requires std::move_constructible<Fut>;
    // 类型约束，拥有 promise_type 类型
    typename std::remove_cvref_t<Fut>::promise_type;
};

/**
 * @brief promise 的概念
 * @tparam P
 */
template <typename P>
concept Promise = requires(P p) {
    // 拥有返回 future 对象接口
    { p.get_return_object() } -> Future;
    { p.initial_suspend() } -> Awaitable;
    { p.final_suspend() } noexcept -> Awaitable;
    p.unhandled_exception();

    // TODO: 让 return_value 接受传入任意类型的参数
    requires(
        requires(int v) { p.return_value(v); } ||
        requires { p.return_void(); });
};

} // namespace eon::coro
