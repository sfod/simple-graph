#pragma once

#include <algorithm>
#include <vector>
#include "graphi.hpp"

template <bool Dir, typename V, typename E>
class MatrixGraph : public GraphI<V, E> {
public:
    MatrixGraph() : vertex_num_(0), vertices_(), matrix_() {}
    virtual ~MatrixGraph() = default;

    virtual int add_vertex(const Vertex<V> &vertex) override {
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
            matrix_.resize(vertex_num_ * vertex_num_, false);
        }
        if (vertices_.count(vertex.idx()) != 0) {
            return -1;
        }
        vertices_[vertex.idx()] = vertex;
        return 0;
    }

    virtual int set_vertex(const Vertex<V> &vertex) override {
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
            matrix_.resize(vertex_num_ * vertex_num_, false);
        }
        vertices_[vertex.idx()] = vertex;
        return 0;
    }

    void rm_vertex(const Vertex<V> &vertex) override {
        if (vertices_.count(vertex.idx()) > 0) {
            vertices_.erase(vertex.idx());
        }
    }

    virtual const std::set<int> &adjacent_vertices(int idx) const override {
        return std::set<int>();
    }

    virtual const Vertex<V> &vertex(int idx) const override {
        return vertices_.at(idx);
    }

    virtual int vertex_num() const override {
        return vertex_num_;
    }

    virtual int add_edge(const Edge<E> &edge) override {
        std::pair<int, int> minmax = std::minmax(edge.idx1(), edge.idx2());
        if (minmax.second >= vertex_num_) {
            return -1;
        }
        if (!Dir) {
//            edge.idx1() = minmax.first;
//            edge.idx2() = minmax.second;
        }
        matrix_[edge.idx1() * vertex_num_ + edge.idx2()] = true;
        return 0;
    }

    virtual int rm_edge(const Edge<E> &edge) override {
        std::pair<int, int> minmax = std::minmax(edge.idx1(), edge.idx2());
        if (minmax.second >= vertex_num_) {
            return -1;
        }
        if (!Dir) {
//            edge.idx1() = minmax.first;
//            edge.idx2() = minmax.second;
        }
        matrix_[edge.idx1() * vertex_num_ + edge.idx2()] = false;
        return 0;
    }

private:
    int vertex_num_;
    std::map<int, Vertex<V>> vertices_;
    std::vector<bool> matrix_;
};
