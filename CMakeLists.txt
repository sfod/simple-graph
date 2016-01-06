cmake_minimum_required(VERSION 2.8)
project(simple_graph CXX)

include(CMakeDependentOption)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# TODO use cmake_dependent_option
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ENABLE_BENCHMARK TRUE CACHE BOOL "Enable benchmarking" FORCE)
endif()

if (ENABLE_BENCHMARK)
    add_subdirectory(libs/benchmark)
endif()

add_subdirectory(src)
add_subdirectory(test)
