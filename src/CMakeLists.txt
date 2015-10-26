set(SOURCE_FILES
    graph/graph.hpp
    graph/graphi.hpp
    graph/graphi.cpp
    graph/matrix_graph.hpp
    graph/list_graph.hpp
)
add_library(simple_graph SHARED ${SOURCE_FILES})
