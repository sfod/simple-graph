cmake_minimum_required(VERSION 2.8)
project(simple_graph CXX)

option(SIMPLE_GRAPH_ENABLE_BENCHMARK "Enable benchmarking" OFF)

include(CMakeDependentOption)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type is not specified, set to Release")
    set(CMAKE_BUILD_TYPE "Release")
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

if (ENABLE_BENCHMARK)
    add_subdirectory(libs/benchmark)
endif()

add_subdirectory(src)
add_subdirectory(test)
