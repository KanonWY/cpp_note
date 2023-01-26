add_executable(future_test future_test.cpp)
target_link_libraries(future_test pthread)

add_executable(test_async async_test.cpp)
target_link_libraries(test_async pthread)