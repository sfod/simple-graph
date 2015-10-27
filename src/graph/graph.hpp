#pragma once

#include <type_traits>
#include <vector>
#include "graphi.hpp"

template<template<bool, typename> class GraphImp, bool Dir, typename T>
class Graph {
public:
    Graph();
    virtual ~Graph() = default;

    int add_edge(const Node<T> &node1, const Node<T> &node2);
    virtual bool find_path(const Node<T> &node1, const Node<T> &node2, std::vector<int> *path) const;

private:
    GraphImp<Dir, T> graph_;
};

template<template<bool, typename> class GraphImp, bool Dir, typename T>
Graph<GraphImp, Dir, T>::Graph() : graph_()
{
    static_assert(std::is_base_of<GraphI<T>, GraphImp<Dir, T>>::value, "GraphImp must implement GraphI");
}

template<template<bool, typename> class GraphImp, bool Dir, typename T>
int Graph<GraphImp, Dir, T>::add_edge(const Node<T> &node1, const Node<T> &node2)
{
    graph_.add_edge(node1, node2);
    return 0;
}

template<template<bool, typename> class GraphImp, bool Dir, typename T>
bool Graph<GraphImp, Dir, T>::find_path(const Node<T> &node1, const Node<T> &node2, std::vector<int> *path) const
{
    return graph_.find_path(node1, node2, path);
}
