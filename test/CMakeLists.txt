configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL
)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(test_list_graph test_undirected_list_graph.cpp)
target_include_directories(test_list_graph
    PRIVATE ${PROJECT_SOURCE_DIR}/src/
    PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
)
target_link_libraries(test_list_graph gtest pthread)
add_test(NAME test_list_graph COMMAND test_list_graph)

add_executable(test_directed_list_graph test_directed_list_graph.cpp)
target_include_directories(test_directed_list_graph
    PRIVATE ${PROJECT_SOURCE_DIR}/src/
    PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
)
target_link_libraries(test_directed_list_graph gtest pthread)
add_test(NAME test_directed_list_graph COMMAND test_directed_list_graph)

add_executable(test_bfs test_bfs.cpp)
target_include_directories(test_bfs
    PRIVATE ${PROJECT_SOURCE_DIR}/src/
    PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
)
target_link_libraries(test_bfs gtest pthread)
add_test(NAME test_bfs COMMAND test_bfs)

add_executable(test_dfs test_dfs.cpp)
target_include_directories(test_dfs
    PRIVATE ${PROJECT_SOURCE_DIR}/src/
    PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
)
target_link_libraries(test_dfs gtest pthread)
add_test(NAME test_dfs COMMAND test_dfs)

add_executable(test_astar test_astar.cpp)
target_include_directories(test_astar
    PRIVATE ${PROJECT_SOURCE_DIR}/src/
    PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
)
target_link_libraries(test_astar gtest pthread)
add_test(NAME test_astar COMMAND test_astar)

add_executable(bench_astar bench_astar.cpp)
target_include_directories(bench_astar
    PRIVATE ${PROJECT_SOURCE_DIR}/src/
    PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
)
target_link_libraries(bench_astar -O2 -ggdb -pg -fno-omit-frame-pointer)

add_executable(test_bellman_ford test_bellman_ford.cpp)
target_include_directories(test_bellman_ford
    PRIVATE ${PROJECT_SOURCE_DIR}/src/
    PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
)
target_link_libraries(test_bellman_ford gtest pthread)
add_test(NAME test_bellman_ford COMMAND test_bellman_ford)

option(ENABLE_BENCHMARK "Enable simple-graph benchmarking" ON)

if (ENABLE_BENCHMARK)
    # Something is wrong with benchmark library tests, disable them.
    option(BENCHMARK_ENABLE_TESTING OFF)

    add_subdirectory(benchmark)

    add_executable(test_performance test_performance.cpp)
    target_include_directories(test_performance
        PRIVATE ${PROJECT_SOURCE_DIR}/src/
        PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
    )
    target_link_libraries(test_performance pthread benchmark)

    add_executable(test_performance2 test_performance_2.cpp)
    target_include_directories(test_performance2
        PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
        PRIVATE ${PROJECT_SOURCE_DIR}/src/
    )

    add_executable(bench_bellman_ford bench_bellman_ford.cpp)
    target_include_directories(bench_bellman_ford
        PRIVATE ${PROJECT_SOURCE_DIR}/src/
        PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/
    )
    target_link_libraries(bench_bellman_ford pthread benchmark)
endif()
