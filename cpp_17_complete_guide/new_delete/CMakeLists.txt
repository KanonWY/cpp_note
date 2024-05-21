cmake_minimum_required(VERSION 3.20)

project(DEMO LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)




add_executable(traceTest  tracenew.cpp)
target_include_directories(traceTest PRIVATE tracenew.h)




