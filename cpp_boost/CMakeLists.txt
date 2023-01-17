find_package(Boost REQUIRED CONFIG)

add_executable(boost_type_test boost_type_test.cpp)
target_link_libraries(boost_type_test ${Boost_LIBRARIES})
