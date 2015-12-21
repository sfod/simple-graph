set(SOURCE_FILES
    graph/graph.hpp
    graph/graphi.hpp
    graph/list_graph.hpp
    graph/astar.hpp
    graph/bfs.hpp
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(simple_graph PROPERTIES LINKER_LANGUAGE CXX)
