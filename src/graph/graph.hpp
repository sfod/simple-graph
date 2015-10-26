#pragma once

#include <type_traits>
#include <vector>
#include "graphi.hpp"

template<template<bool> class GraphImp, bool Dir>
class Graph {
public:
    Graph();
    virtual ~Graph() = default;

    int add_edge(int node1, int node2);
    virtual bool find_path(int node1, int node2, std::vector<int> *path) const;

private:
    GraphImp<Dir> graph_;
};

template<template<bool> class GraphImp, bool Dir>
Graph<GraphImp, Dir>::Graph() : graph_()
{
    static_assert(std::is_base_of<GraphI, GraphImp<Dir>>::value, "GraphImp must implement GraphI");
}

template<template<bool> class GraphImp, bool Dir>
int Graph<GraphImp, Dir>::add_edge(int node1, int node2)
{
    graph_.add_edge(node1, node2);
    return 0;
}

template<template<bool> class GraphImp, bool Dir>
bool Graph<GraphImp, Dir>::find_path(int node1, int node2, std::vector<int> *path) const
{
    return graph_.find_path(node1, node2, path);
}
