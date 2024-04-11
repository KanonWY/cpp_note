#pragma once

#include <future>
#include <optional>
#include <coroutine>
#include <coroTask.h>

// 默认情况下，编译器会查找 Future 的类型成员 promise_type 作为 Promise。
// 若 promise_type 不存在则会返回编译报错

// 如果有一个类无法向其中添加新的类型成员，如何使得这个类成为 Future ?
//
// coroutine_traits


// struct AsCoroutine {};

// template<>
// struct std::coroutine_traits<void, int, eon::CoroTask&> {
//     using promise_type = eon::CoroTask::promise_type;
// };


// inline void coro(int max, eon::CoroTask&) {
//     std::cout << "CORO: " << max << std::endl;

//     for (int val = 1; val <= max; ++val) {
//         std::cout << "CORO: in { " << val << " }" << std::endl;
//         co_await std::suspend_always{};
//     }

//     std::cout << "CORO: END" << std::endl;
// }


// template<typename T>
// struct maybe: public std::optional<T> {
//     using Base = std::optional<T>;
//     using Base::Base;
// };

// template<typename T, typename... Args>
// struct std::coroutine_traits<maybe<T>, Args...> {
//     struct promise_type {
//         auto get_return_object(){

//         };

//     private:
//         maybe<T>* res_;
//     };
// };
