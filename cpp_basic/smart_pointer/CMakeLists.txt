
find_package(Boost REQUIRED CONFIG)

if(Boost_FOUND)
    message(STATUS "-----> Boost FOUND")
endif()

add_executable(shared_pointer_use share_pointer_use.cpp)

add_executable(unique_pointer_use unique_pointer_use.cpp)

add_executable(weak_pointer_use weak_pointer_use.cpp)
target_link_libraries(weak_pointer_use ${Boost_LIBRARIES})