#include "matrix_graph.hpp"

MatrixGraph::MatrixGraph() : vertices_(0), matrix_()
{
}

int MatrixGraph::add_edge(int node1, int node2)
{
    int max_node = std::max(node1, node2);
    if (vertices_ < max_node + 1) {
        vertices_ = max_node + 1;
        matrix_.resize(vertices_ * vertices_, false);
    }

    matrix_[node1 * vertices_ + node2] = true;
    matrix_[node2 * vertices_ + node1] = true;

    return 0;
}

int MatrixGraph::rm_edge(int node1, int node2)
{
    if ((node1 >= vertices_) || (node2 >= vertices_)) {
        return -1;
    }
    matrix_[node1 * vertices_ + node2] = false;
    matrix_[node2 * vertices_ + node1] = false;
    return 0;
}

bool MatrixGraph::is_edge(int node1, int node2) const
{
    if ((node1 >= vertices_) || (node2 >= vertices_)) {
        return false;
    }
    return matrix_[node1 * vertices_ + node2];
}
