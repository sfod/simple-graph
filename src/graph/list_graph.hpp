#pragma once

#include <algorithm>
#include <map>
#include <set>
#include "graphi.hpp"

template <bool Dir, typename V, typename E>
class ListGraph : public GraphI<V, E> {
public:
    ListGraph() : vertex_num_(0), vertices_(), edges_() {}
    virtual ~ListGraph() = default;

    virtual int add_vertex(const Vertex<V> &vertex) override {
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
        }
        if (vertices_.count(vertex.idx()) != 0) {
            return -1;
        }
        vertices_[vertex.idx()] = vertex;
        edges_[vertex.idx()] = std::set<int>();
        return 0;
    }

    virtual int set_vertex(const Vertex<V> &vertex) override {
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
        }
        vertices_[vertex.idx()] = vertex;
        return 0;
    }

    void rm_vertex(const Vertex<V> &vertex) override {
        if (vertices_.count(vertex.idx()) > 0) {
            vertices_.erase(vertex.idx());
        }
        for (auto v : edges_[vertex.idx()]) {
            rm_edge(Edge<E>(v, vertex.idx()));
        }
    }

    virtual const std::set<int> &adjacent_vertices(int idx) const override {
        return edges_.at(idx);
    }

    virtual int add_edge(const Edge<E> &edge) override {
        if (std::max(edge.idx1(), edge.idx2()) >= vertex_num_) {
            return -1;
        }
        edges_[edge.idx1()].insert(edge.idx2());
        if (!Dir) {
            edges_[edge.idx2()].insert(edge.idx1());
        }
        return 0;
    }

    virtual int rm_edge(const Edge<E> &edge) override {
        if (std::max(edge.idx1(), edge.idx2()) >= vertex_num_) {
            return -1;
        }
        if (edges_.count(edge.idx1()) == 0) {
            return -1;
        }
        edges_[edge.idx1()].erase(edge.idx2());
        if (!Dir) {
            edges_[edge.idx2()].erase(edge.idx1());
        }
        return 0;
    }

    virtual const Vertex<V> &vertex(int idx) const override {
        return vertices_.at(idx);
    }

    virtual int vertex_num() const override { return vertex_num_; };

private:
    int vertex_num_;
    std::map<int, Vertex<V>> vertices_;
    std::map<int, std::set<int>> edges_;
};
