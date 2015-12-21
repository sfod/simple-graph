include_directories(../src/)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_list_graph test_list_graph.cpp)
target_link_libraries(test_list_graph ${GTEST_LIBRARIES} pthread)

add_executable(test_astar test_astar.cpp)
target_link_libraries(test_astar ${GTEST_LIBRARIES} pthread)
