cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0048 NEW)

project(simple-graph VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
option(SIMPLE_GRAPH_ENABLE_BENCHMARK "Enable benchmarking" OFF)

include(CMakeDependentOption)

ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic)
ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:-O3>")
ADD_COMPILE_OPTIONS("$<$<CONFIG:DEBUG>:-O0;-ggdb>")
ADD_COMPILE_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:-O3;-ggdb;-fno-omit-frame-pointer>")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type is not specified, set to Release")
    set(CMAKE_BUILD_TYPE "Release")
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

set(SOURCE_FILES
        src/simple_graph/graph.hpp
        src/simple_graph/list_graph.hpp
        src/simple_graph/algorithm/astar.hpp
        src/simple_graph/algorithm/bfs.hpp
        src/simple_graph/algorithm/dfs.hpp
        src/simple_graph/algorithm/bellman_ford.hpp
)
add_library(simple-graph SHARED ${SOURCE_FILES})
target_compile_options(simple-graph PRIVATE -Werror)
target_compile_features(simple-graph PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
