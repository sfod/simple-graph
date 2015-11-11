#pragma once

#include <algorithm>
#include <vector>
#include "graphi.hpp"

template <bool Dir, typename T>
class MatrixGraph : public GraphI<T> {
public:
    MatrixGraph() : vertex_num_(0), vertices_(), matrix_() {}
    virtual ~MatrixGraph() = default;

    virtual int add_vertex(const Vertex<T> &vertex) override {
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

    virtual int set_vertex(const Vertex<T> &vertex) override {
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
            matrix_.resize(vertex_num_ * vertex_num_, false);
        }
        vertices_[vertex.idx()] = vertex;
        return 0;
    }

    void rm_vertex(const Vertex<T> &vertex) override {
        if (vertices_.count(vertex.idx()) > 0) {
            vertices_.erase(vertex.idx());
        }
    }

    virtual const Vertex<T> &vertex(int idx) const override {
        return vertices_.at(idx);
    }

    virtual int vertex_num() const override {
        return vertex_num_;
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
        matrix_[idx1 * vertex_num_ + idx2] = true;
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
        matrix_[idx1 * vertex_num_ + idx2] = false;
        return 0;
    }


    virtual bool is_edge(int idx1, int idx2) const override {
        auto minmax = std::minmax(idx1, idx2);
        if (minmax.second >= vertex_num_) {
            return -1;
        }
        if (!Dir) {
            idx1 = minmax.first;
            idx2 = minmax.second;
        }
        return matrix_[idx1 * vertex_num_ + idx2];
    }

private:
    int vertex_num_;
    std::map<int, Vertex<T>> vertices_;
    std::vector<bool> matrix_;
};
