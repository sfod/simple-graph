#pragma once

#include <type_traits>
#include <vector>
#include "graphi.hpp"

template<typename GraphImp, typename Dir>
class Graph {
public:
    Graph();
    virtual ~Graph();

    int add_edge(int node1, int node2);
    virtual bool find_path(int node1, int node2, std::vector<int> *path) const;

private:
    GraphImp graph_;
};

template<typename GraphImp, typename Dir>
Graph<GraphImp, Dir>::Graph() : graph_()
{
}

template<typename GraphImp, typename Dir>
Graph<GraphImp, Dir>::~Graph()
{

}

template<typename GraphImp, typename Dir>
int Graph<GraphImp, Dir>::add_edge(int node1, int node2)
{
    graph_.add_edge(node1, node2);
    return 0;
}

template<typename GraphImp, typename Dir>
bool Graph<GraphImp, Dir>::find_path(int node1, int node2, std::vector<int> *path) const
{
    return graph_.find_path(node1, node2, path);
}
