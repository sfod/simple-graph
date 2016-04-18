set(SOURCE_FILES
        simple_graph/graph.hpp
        simple_graph/list_graph.hpp
        simple_graph/algorithm/astar.hpp
        simple_graph/algorithm/bfs.hpp
        simple_graph/algorithm/dfs.hpp
        simple_graph/algorithm/bellman_ford.hpp
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(simple_graph PROPERTIES LINKER_LANGUAGE CXX)
