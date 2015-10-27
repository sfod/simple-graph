#pragma once

#include <type_traits>
#include <vector>
#include "graphi.hpp"

template<template<bool, typename> class GraphImp, bool Dir, typename T>
class Graph {
public:
    Graph();
    virtual ~Graph() = default;

    int add_vertex(const Vertex<T> &vertex);
    void rm_vertex(const Vertex<T> &vertex);

    int add_edge(int idx1, int idx2);
    virtual bool find_path(int start_idx, int end_idx, std::vector<int> *path) const;

private:
    GraphImp<Dir, T> graph_;
};

template<template<bool, typename> class GraphImp, bool Dir, typename T>
Graph<GraphImp, Dir, T>::Graph() : graph_()
{
    static_assert(std::is_base_of<GraphI<T>, GraphImp<Dir, T>>::value, "GraphImp must implement GraphI");
}

template<template<bool, typename> class GraphImp, bool Dir, typename T>
int Graph<GraphImp, Dir, T>::add_vertex(const Vertex<T> &vertex)
{
    return graph_.add_vertex(vertex);
}

template<template<bool, typename> class GraphImp, bool Dir, typename T>
void Graph<GraphImp, Dir, T>::rm_vertex(const Vertex<T> &vertex)
{
    graph_.rm_vertex(vertex);
}

template<template<bool, typename> class GraphImp, bool Dir, typename T>
int Graph<GraphImp, Dir, T>::add_edge(int idx1, int idx2)
{
    graph_.add_edge(idx1, idx2);
    return 0;
}

template<template<bool, typename> class GraphImp, bool Dir, typename T>
bool Graph<GraphImp, Dir, T>::find_path(int start_idx, int end_idx, std::vector<int> *path) const
{
    return graph_.find_path(start_idx, end_idx, path);
}
