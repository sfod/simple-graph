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
        return 0;
    }

    void rm_vertex(const Vertex<T> &vertex) override {
        if (vertices_.count(vertex.idx()) > 0) {
            vertices_.erase(vertex.idx());
        }
    }

    virtual int add_edge(int idx1, int idx2) override {
        auto minmax = std::minmax(idx1, idx2);
        if (minmax.second >= vertex_num_) {
            return -1;
        }
        if (!Dir) {
            idx1 = minmax.first;
            idx2 = minmax.second;
        }
        edges_[idx1].insert(idx2);
        return 0;
    }

    virtual int rm_edge(int idx1, int idx2) override {
        auto minmax = std::minmax(idx1, idx2);
        if (minmax.second >= vertex_num_) {
            return -1;
        }
        if (!Dir) {
            idx1 = minmax.first;
            idx2 = minmax.second;
        }
        if (edges_.count(idx1) == 0) {
            return -1;
        }
        edges_[idx1].erase(idx2);
        if (edges_[idx1].size() == 0) {
            edges_.erase(idx1);
        }
        return 0;
    }

    virtual const Vertex<T> &vertex(int idx) const override {
        return vertices_.at(idx);
    }

    virtual bool is_edge(int idx1, int idx2) const override {
        auto minmax = std::minmax(idx1, idx2);
        if (!Dir) {
            idx1 = minmax.first;
            idx2 = minmax.second;
        }
        if (edges_.count(idx1) == 0) {
            return false;
        }
        return (edges_.at(idx1).count(idx2) > 0);
    }

    virtual int vertex_num() const override { return vertex_num_; };

private:
    int vertex_num_;
    std::map<int, Vertex<T>> vertices_;
    std::map<int, std::set<int>> edges_;
};
