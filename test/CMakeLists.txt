include_directories(../src/)
add_subdirectory(googletest)
include_directories(googletest/googletest/include)

add_executable(test_list_graph test_undirected_list_graph.cpp)
target_link_libraries(test_list_graph gtest pthread)

add_executable(test_directed_list_graph test_directed_list_graph.cpp)
target_link_libraries(test_directed_list_graph gtest pthread)

add_executable(test_bfs test_bfs.cpp)
target_link_libraries(test_bfs gtest pthread)

add_executable(test_dfs test_dfs.cpp)
target_link_libraries(test_dfs gtest pthread)

add_executable(test_astar test_astar.cpp)
target_link_libraries(test_astar gtest pthread)

add_executable(test_bellman_ford test_bellman_ford.cpp)
target_link_libraries(test_bellman_ford gtest pthread)

if (ENABLE_BENCHMARK)
    include_directories(../libs/benchmark/include)
    link_directories(${CMAKE_BINARY_DIR}/libs/benchmark/src/)

    add_executable(test_performance test_performance.cpp)
    target_link_libraries(test_performance pthread benchmark)

    add_executable(test_performance2 test_performance_2.cpp)
    target_link_libraries(test_performance2)

    add_executable(bench_bellman_ford bench_bellman_ford.cpp)
    target_link_libraries(bench_bellman_ford pthread benchmark)
endif()
