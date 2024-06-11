cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(lock_free_cpp 
  LANGUAGES CXX
  VERSION 0.0.1
)

add_executable(lock_free_queue src/lock_free_queue.cpp)

add_executable(lock_free_stack src/lock_free_stack_use_count.cpp)

add_executable(atomic_share_ptr src/atomic_share_ptr.cpp)

