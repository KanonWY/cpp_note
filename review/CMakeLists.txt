cmake_minimum_required(VERSION 3.12)

project(LeetCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

find_package(spdlog REQUIRED)

add_subdirectory(tree)

add_subdirectory(bit)
add_subdirectory(memoryBasic)

add_subdirectory(typeTraits)

# add_subdirectory(linklist)
# add_subdirectory(DP)
add_subdirectory(STL)

add_subdirectory(time)