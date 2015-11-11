include_directories(../src/)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(test_simple_graph ${SOURCE_FILES})
target_link_libraries(test_simple_graph simple_graph)

add_executable(test_list_graph test_list_graph.cpp)
target_link_libraries(test_list_graph ${GTEST_LIBRARIES} pthread)
