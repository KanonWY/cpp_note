find_package(Backward REQUIRED)

add_executable(test_backward main.cpp)
target_link_libraries(test_backward PRIVATE Backward::Backward
  dw
  )
