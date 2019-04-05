add_subdirectory(googletest)

add_executable(test_list_graph test_undirected_list_graph.cpp)
target_include_directories(test_list_graph
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/
)
target_include_directories(test_list_graph PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
target_link_libraries(test_list_graph gtest pthread)

add_executable(test_directed_list_graph test_directed_list_graph.cpp)
target_include_directories(test_directed_list_graph
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/
)
target_include_directories(test_directed_list_graph PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
target_link_libraries(test_directed_list_graph gtest pthread)

add_executable(test_bfs test_bfs.cpp)
target_include_directories(test_bfs
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/
)
target_include_directories(test_bfs PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
target_link_libraries(test_bfs gtest pthread)

add_executable(test_dfs test_dfs.cpp)
target_include_directories(test_dfs
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/
)
target_include_directories(test_dfs PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
target_link_libraries(test_dfs gtest pthread)

add_executable(test_astar test_astar.cpp)
target_include_directories(test_astar
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/
)
target_include_directories(test_astar PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
target_link_libraries(test_astar gtest pthread)

add_executable(bench_astar bench_astar.cpp)
target_include_directories(bench_astar
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/
)
target_include_directories(bench_astar PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
target_link_libraries(bench_astar -O2 -ggdb -pg -fno-omit-frame-pointer)

add_executable(test_bellman_ford test_bellman_ford.cpp)
target_include_directories(test_bellman_ford
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/
)
target_include_directories(test_bellman_ford PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
target_link_libraries(test_bellman_ford gtest pthread)

if (ENABLE_BENCHMARK)
    add_subdirectory(benchmark)

    add_executable(test_performance test_performance.cpp)
    target_include_directories(test_performance
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src/
    )
    target_include_directories(test_performance PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
    target_link_libraries(test_performance pthread benchmark)

    add_executable(test_performance2 test_performance_2.cpp)
    target_include_directories(test_performance2 PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
    target_include_directories(test_performance2
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src/
    )

    add_executable(bench_bellman_ford bench_bellman_ford.cpp)
    target_include_directories(bench_bellman_ford
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src/
    )
    target_include_directories(bench_bellman_ford PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gsl/include/)
    target_link_libraries(bench_bellman_ford pthread benchmark)
endif()
