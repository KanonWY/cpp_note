enable_testing()
add_executable(
    travel_test
    testTreetra.cpp travel.cpp
)

target_link_libraries(
    travel_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(travel_test)