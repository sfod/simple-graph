cmake_minimum_required(VERSION 2.8)
project(simple_graph CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -O0 -ggdb")

add_subdirectory(src)
add_subdirectory(test)
