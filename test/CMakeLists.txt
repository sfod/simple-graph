include_directories(../src/)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_list_graph test_list_graph.cpp)
target_link_libraries(test_list_graph ${GTEST_LIBRARIES} pthread)

add_executable(test_astar test_astar.cpp)
target_link_libraries(test_astar ${GTEST_LIBRARIES} pthread)

add_executable(test_bellman_ford test_bellman_ford.cpp)
target_link_libraries(test_bellman_ford ${GTEST_LIBRARIES} pthread)

if (ENABLE_BENCHMARK)
    include_directories(../libs/benchmark/include)
    link_directories(${CMAKE_BINARY_DIR}/libs/benchmark/src/)

    add_executable(test_performance test_performance.cpp)
    target_link_libraries(test_performance pthread benchmark)

    add_executable(test_performance2 test_performance_2.cpp)
    target_link_libraries(test_performance2)

    add_executable(bench_performance_bellman_ford bench_performance_bellman_ford.cpp)
    target_link_libraries(bench_performance_bellman_ford)
endif()
