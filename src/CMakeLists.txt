set(SOURCE_FILES
        simple_graph/graph.hpp
        simple_graph/list_graph.hpp
        simple_graph/algorithm/astar.hpp
        simple_graph/algorithm/bfs.hpp
        simple_graph/algorithm/dfs.hpp
        simple_graph/algorithm/bellman_ford.hpp
)
add_library(simple-graph SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
