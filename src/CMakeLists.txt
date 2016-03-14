set(SOURCE_FILES
    simple_graph/graph.hpp
    simple_graph/list_graph.hpp
    simple_graph/astar.hpp
    simple_graph/bfs.hpp
    simple_graph/bellman_ford.hpp
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(simple_graph PROPERTIES LINKER_LANGUAGE CXX)
