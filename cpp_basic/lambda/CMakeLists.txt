

find_package(PkgConfig REQUIRED)

# Find HIREDIS++ LIB
pkg_search_module(
    HIREDIS
    REQUIRED
    hiredis
    IMPORTED_TARGET
)

if(TARGET PkgConfig::HIREDIS)
    message(STATUS "---> FOUND PkgConfig::HIREDIS")
else()
    message(STATUS "---> NOT FOUND PkgConfig::HIREDIS")
endif()

# Find REDIS++ LIB
pkg_search_module(
    REDIS++
    REQUIRED
    redis++
    IMPORTED_TARGET
)

if(TARGET PkgConfig::REDIS++)
    message(STATUS "---> FOUND PkgConfig::Redis++")
else()
    message(STATUS "---> NOT FOUND PkgConfig::Redis++")
endif()

# Find THREAD LIB
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

message(STATUS "-----> ${CMAKE_THREAD_LIBS_INIT}")

add_executable(lambad_example lambda_example.cpp)
target_link_libraries(lambad_example PkgConfig::REDIS++ PkgConfig::HIREDIS ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(bind_example bind_example.cpp)
set_target_properties(bind_example
    PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

add_executable(lambda_example_11 lambda_example_11.cpp)
set_target_properties(lambda_example_11
    PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

add_executable(lambda_example_14 lambda_example_14.cpp)
target_link_libraries(lambda_example_14 ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(lambda_example_14
    PROPERTIES
    CXX_STANDARD 14
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

add_executable(lambda_example_17 lambda_example_17.cpp)
target_link_libraries(lambda_example_17 ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(lambda_example_17
    PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

add_executable(lambda_example_20 lambda_example_20.cpp)
target_link_libraries(lambda_example_20 ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(lambda_example_20
    PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)