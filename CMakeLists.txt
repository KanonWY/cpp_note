cmake_minimum_required(VERSION 3.12)

project(LeetCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE EQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# add_subdirectory(cpp_alg)
add_subdirectory(cpp_basic)
add_subdirectory(cpp_boost)

# add_subdirectory(cpp_design)
# add_subdirectory(cpp_move_semantics)

# add_subdirectory(LeetCode)