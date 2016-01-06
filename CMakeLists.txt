cmake_minimum_required(VERSION 2.8)
project(simple_graph CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# TODO make it optional
add_subdirectory(libs/benchmark)

add_subdirectory(src)
add_subdirectory(test)
