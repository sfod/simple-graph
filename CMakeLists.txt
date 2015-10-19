cmake_minimum_required(VERSION 3.3)
project(simple_graph)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -O0 -ggdb")

set(SOURCE_FILES main.cpp graph.hpp graph.cpp)
add_executable(simple_graph ${SOURCE_FILES})
