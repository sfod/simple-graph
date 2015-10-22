#include "list_graph.hpp"

ListGraph::ListGraph() : vertices_(0), edges_()
{
}

int ListGraph::add_edge(int node1, int node2)
{
    int min_node = std::min(node1, node2);
    int max_node = std::max(node1, node2);
    if (vertices_ < max_node + 1) {
        vertices_ = max_node + 1;
    }
    edges_[min_node].insert(max_node);
    return 0;
}

int ListGraph::rm_edge(int node1, int node2)
{
    int min_node = std::min(node1, node2);
    int max_node = std::max(node1, node2);
    edges_[min_node].erase(max_node);
    return 0;
}

bool ListGraph::is_edge(int node1, int node2) const
{
    int min_node = std::min(node1, node2);
    int max_node = std::max(node1, node2);
    return (edges_.at(min_node).count(max_node) > 0);
}
