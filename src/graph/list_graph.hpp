#pragma once

#include <algorithm>
#include <map>
#include <set>
#include "graphi.hpp"

template <bool Dir, typename T>
class ListGraph : public GraphI<T> {
public:
    ListGraph() : vertex_num_(0), vertices_(), edges_() {}
    virtual ~ListGraph() = default;

    virtual int add_vertex(const Vertex<T> &vertex) override {
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

    virtual int set_vertex(const Vertex<T> &vertex) override {
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
        }
        vertices_[vertex.idx()] = vertex;
        return 0;
    }

    void rm_vertex(const Vertex<T> &vertex) override {
        if (vertices_.count(vertex.idx()) > 0) {
            vertices_.erase(vertex.idx());
        }
        for (auto v : edges_[vertex.idx()]) {
            rm_edge(v, vertex.idx());
        }
    }

    virtual const std::set<int> &adjacent_vertices(int idx) const override {
        return edges_.at(idx);
    }

    virtual int add_edge(int idx1, int idx2) override {
        if (std::max(idx1, idx2) >= vertex_num_) {
            return -1;
        }
        edges_[idx1].insert(idx2);
        if (!Dir) {
            edges_[idx2].insert(idx1);
        }
        return 0;
    }

    virtual int rm_edge(int idx1, int idx2) override {
        if (std::max(idx1, idx2) >= vertex_num_) {
            return -1;
        }
        if (edges_.count(idx1) == 0) {
            return -1;
        }
        edges_[idx1].erase(idx2);
        if (!Dir) {
            edges_[idx2].erase(idx1);
        }
        return 0;
    }

    virtual const Vertex<T> &vertex(int idx) const override {
        return vertices_.at(idx);
    }

    virtual int vertex_num() const override { return vertex_num_; };

private:
    int vertex_num_;
    std::map<int, Vertex<T>> vertices_;
    std::map<int, std::set<int>> edges_;
};
