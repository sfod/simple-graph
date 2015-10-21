include_directories(../src/)

set(SOURCE_FILES main.cpp)
add_executable(test_simple_graph ${SOURCE_FILES})
target_link_libraries(test_simple_graph simple_graph)
