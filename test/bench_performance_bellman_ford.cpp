#include <iostream>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/bellman_ford.hpp"

using simple_graph::vertex_index_t;

int main()
{
    simple_graph::ListGraph<true, int, ssize_t> g;

    for (vertex_index_t i = 0; i < 8; ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i));
    }

    g.add_edge(simple_graph::Edge<ssize_t>(7, 4, 1));
    g.add_edge(simple_graph::Edge<ssize_t>(4, 3, 2));
    g.add_edge(simple_graph::Edge<ssize_t>(3, 2, 3));
    g.add_edge(simple_graph::Edge<ssize_t>(2, 1, 4));
    g.add_edge(simple_graph::Edge<ssize_t>(1, 0, 5));
    g.add_edge(simple_graph::Edge<ssize_t>(7, 6, 20));
    g.add_edge(simple_graph::Edge<ssize_t>(6, 5, 20));
    g.add_edge(simple_graph::Edge<ssize_t>(5, 0, 20));

    std::vector<vertex_index_t> path;
    simple_graph::bellman_ford(g, 7, 0, &path);

    std::cout << path.size() << std::endl;

    return 0;
}
