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


DEFINE_EXE(concept_base)

DEFINE_EXE(require_member_function)

DEFINE_EXE(concept_for_detail_info)

DEFINE_EXE(why_concept)

DEFINE_EXE(concept_requires)

DEFINE_EXE(concept_requires_details)

DEFINE_EXE(concept_requires_no_temp_function)

DEFINE_EXE(concept_requires_class_temp)

DEFINE_EXE(concept_requires_var_temp)

DEFINE_EXE(match_test)

DEFINE_EXE(coro_test)
