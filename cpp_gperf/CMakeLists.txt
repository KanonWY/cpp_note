cmake_minimum_required(VERSION 3.1)


project(testperf)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")


set(CMAKE_CXX_CIMPLIER "clang++")
set(CMAKE_C_COMPILER "clang")

# message(STATUS "-----> CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
find_package(Gperftools REQUIRED)

if(Gperftools_FOUND)
    message(STATUS "-----> Gperftools_FOUND [YES]")
    message(STATUS "----->GPERFTOOLS_INCLUDE_DIR =  " ${GPERFTOOLS_INCLUDE_DIR})
    message(STATUS "----->GPERFTOOLS_LIBRARIES = " ${GPERFTOOLS_LIBRARIES})
else()
    message(STATUS "-----> Gperftools_FOUND [NO]")
endif()

include_directories(${GPERFTOOLS_INCLUDE_DIR})

# find unwind
find_package(PkgConfig REQUIRED)
if(PkgConfig_FOUND)
    message(STATUS "--------------------------------------------------------------------------------->")
    message(STATUS "======> USE [[ pkg_check_modules ]] <======")
    pkg_check_modules(UNWIND REQUIRED IMPORTED_TARGET libunwind)
    if(UNWIND_FOUND)
        message(STATUS "======> FOUND UNWIND")
        message(STATUS "======> UNWIND VERSION = ${UNWIND_VERSION}")
        message(STATUS "======> UNWIND INCLUDE_DIRS = ${UNWIND_INCLUDE_DIRS}")
        message(STATUS "======> UNWIND LIBRARIES = ${UNWIND_LIBRARIES}")
        message(STATUS "======> UNWIND LIBRARY_DIRS = ${UNWIND_LIBRARY_DIRS}")
    endif()
    message(STATUS "--------------------------------------------------------------------------------->")
    message(STATUS "======> USE [[ pkg_search_module ]] <======")
    pkg_search_module(ZLIB REQUIRED zlib IMPORTED_TARGET)
    if(ZLIB_FOUND)
        message(STATUS "=====> FOUND ZLIB ${ZLIB_VERSION}")
        message(STATUS "=====> ZLIB_INCLUDE_DIRS = ${ZLIB_INCLUDE_DIRS}")
        message(STATUS "=====> ZLIB_LIBRARIES = ${ZLIB_LIBRARIES}")
        message(STATUS "=====> ZLIB_LIBRARY_DIRS = ${ZLIB_LIBRARY_DIRS}")
    endif()
endif()

add_executable(testCpu testCpu.cpp gperftoolsWrap.cpp)
target_link_libraries(testCpu ${GPERFTOOLS_LIBRARIES} ${UNWIND_LIBRARIES})


add_executable(testheap testHeap.cpp gperftoolsWrap.cpp)
target_link_libraries(testheap ${GPERFTOOLS_LIBRARIES} ${UNWIND_LIBRARIES})

add_executable(main main.cpp gperftoolsWrap.cpp)
target_link_libraries(main ${GPERFTOOLS_LIBRARIES} ${UNWIND_LIBRARIES})
