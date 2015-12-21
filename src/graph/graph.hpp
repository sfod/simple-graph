#pragma once

#include <type_traits>
#include <vector>
#include "graphi.hpp"

template<template<bool, typename, typename> class GraphImp, bool Dir, typename V, typename E>
class Graph {
public:
    Graph() : graph_() {
        static_assert(std::is_base_of<GraphI<V, E>, GraphImp<Dir, V, E>>::value, "GraphImp must implement GraphI");
    }

    virtual ~Graph() = default;

    int add_vertex(const Vertex<V> &vertex) {
        return graph_.add_vertex(vertex);
    }

    void rm_vertex(const Vertex<V> &vertex) {
        graph_.rm_vertex(vertex);
    }

    const Vertex<V> &vertex(int idx) const {
        return graph_.vertex(idx);
    }

    int vertex_num() const {
        return graph_.vertex_num();
    }

    int add_edge(const Edge<E> &edge) {
        return graph_.add_edge(edge);
    }

private:
    GraphImp<Dir, V, E> graph_;
};
