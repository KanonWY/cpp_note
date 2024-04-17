cmake_minimum_required(VERSION 3.20)

project(CORODEMO)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


message(STATUS "${CMAKE_C_COMPILER}")

#定义创建可执行宏
macro(DEFINE_EXE name)
    add_executable(${name} src/${name}.cpp)
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
endmacro()

DEFINE_EXE(coro_easy_task)

DEFINE_EXE(coro_await_base)

DEFINE_EXE(coro_task_sub)

DEFINE_EXE(coro_yield)

DEFINE_EXE(coro_generator)

DEFINE_EXE(coro_return)

DEFINE_EXE(coro_await_back)

DEFINE_EXE(coro_prio)

DEFINE_EXE(coroFinalAwait)

DEFINE_EXE(await_transform)

DEFINE_EXE(coro_trace)

DEFINE_EXE(coro_trait)

DEFINE_EXE(coro_pool)

DEFINE_EXE(coro_pool2)

DEFINE_EXE(coro_concept)

DEFINE_EXE(call_coro)
