#pragma once

#include <algorithm>
#include <vector>
#include "graphi.hpp"

template <bool Dir, typename T>
class MatrixGraph : public GraphI<T> {
public:
    MatrixGraph() : vertices_(0), matrix_(), nodes_() {}
    virtual ~MatrixGraph() = default;

    virtual int add_edge(const Vertex<T> &node1, const Vertex<T> &node2) override{
        auto minmax = std::minmax(node1, node2);
        if (vertices_ < minmax.second.idx() + 1) {
            vertices_ = minmax.second.idx() + 1;
            matrix_.resize(vertices_ * vertices_, false);
        }
        int idx1 = node1.idx();
        int idx2 = node2.idx();
        if (!Dir) {
            idx1 = minmax.first.idx();
            idx2 = minmax.second.idx();
        }

        add_vertex(node1);
        add_vertex(node2);

        matrix_[idx1 * vertices_ + idx2] = true;

        return 0;
    }

    virtual int rm_edge(const Vertex<T> &node1, const Vertex<T> &node2) override {
        auto minmax = std::minmax(node1, node2);
        if (minmax.second.idx() >= vertices_) {
            return -1;
        }
        int idx1 = node1.idx();
        int idx2 = node2.idx();
        if (!Dir) {
            idx1 = minmax.first.idx();
            idx2 = minmax.second.idx();
        }
        matrix_[idx1 * vertices_ + idx2] = false;
        return 0;
    }

    virtual const Vertex<T> &vertex(int idx) const override {
        return nodes_.at(idx);
    }

    virtual bool is_edge(int idx1, int idx2) const override {
        auto minmax = std::minmax(idx1, idx2);
        if (minmax.second >= vertices_) {
            return -1;
        }
        if (!Dir) {
            idx1 = minmax.first;
            idx2 = minmax.second;
        }
        return matrix_[idx1 * vertices_ + idx2];
    }

    virtual int vertex_num() const override { return vertices_; };

private:
    void add_vertex(const Vertex<T> &node) {
        if (nodes_.count(node.idx()) == 0) {
            nodes_[node.idx()] = node;
        }
    }

    void rm_vertex(const Vertex<T> &node) {
        if (nodes_.count(node.idx()) > 0) {
            nodes_.erase(node.idx());
        }
    }

private:
    int vertices_;
    std::vector<bool> matrix_;
    std::map<int, Vertex<T>> nodes_;
};
