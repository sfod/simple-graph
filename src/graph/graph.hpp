#pragma once

#include <type_traits>
#include <vector>
#include "graphi.hpp"

template<template<bool, typename> class GraphImp, bool Dir, typename T>
class Graph {
public:
    Graph() : graph_() {
        static_assert(std::is_base_of<GraphI<T>, GraphImp<Dir, T>>::value, "GraphImp must implement GraphI");
    }

    virtual ~Graph() = default;

    int add_vertex(const Vertex<T> &vertex) {
        return graph_.add_vertex(vertex);
    }

    void rm_vertex(const Vertex<T> &vertex) {
        graph_.rm_vertex(vertex);
    }

    const Vertex<T> &vertex(int idx) const {
        return graph_.vertex(idx);
    }

    int vertex_num() const {
        return graph_.vertex_num();
    }

    int add_edge(int idx1, int idx2) {
        return graph_.add_edge(idx1, idx2);
    }

    virtual bool find_path(int start_idx, int end_idx, std::vector<int> *path) const {
        return graph_.bfs(start_idx, end_idx, path);
    }

private:
    GraphImp<Dir, T> graph_;
};
