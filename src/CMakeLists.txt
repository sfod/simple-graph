set(SOURCE_FILES
    graph/graph.hpp
    graph/graphi.hpp
    graph/matrix_graph.hpp
    graph/list_graph.hpp
)
add_library(simple_graph SHARED ${SOURCE_FILES})
set_target_properties(simple_graph PROPERTIES LINKER_LANGUAGE CXX)
