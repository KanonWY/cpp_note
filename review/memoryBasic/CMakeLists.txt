add_executable(
    class_memory
    class_memory.cpp
)

add_executable(testSp
    main.cpp
)

add_executable(test_myAlloc testMyAlloc.cpp)

add_executable(test_stl_sp test_std_sp.cpp)
target_link_libraries(test_stl_sp spdlog::spdlog)