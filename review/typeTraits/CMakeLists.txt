find_package(Boost)

if(Boost_FOUND)
    message(STATUS "-------> find boost---->")
    MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")

    # include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(test_enable_if test_enable_if.cpp)

add_executable(test_condtional test_condition.cpp ${Boost_LIBRARIES})

add_executable(test_change_var test_changevar.cpp)

add_executable(test_special test_special.cpp)

add_executable(test_void_t test_void_t.cpp)

add_executable(test_declval test_declval.cpp)
target_link_libraries(test_declval ${Boost_LIBRARIES})

add_executable(test_function test_function.cpp)

add_executable(ka_function ka_function.cpp)

# target_link_directories(test_declval ${Boost_LIBRARIES})