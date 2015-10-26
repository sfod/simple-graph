#pragma once

#include <algorithm>
#include <vector>
#include "graphi.hpp"

template <bool Dir>
class MatrixGraph : public GraphI {
public:
    explicit MatrixGraph() = default;
    ~MatrixGraph() = default;

    virtual int add_edge(int node1, int node2) override{
        auto minmax = std::minmax(node1, node2);
        if (vertices_ < minmax.second + 1) {
            vertices_ = minmax.second + 1;
            matrix_.resize(vertices_ * vertices_, false);
        }
        if (!Dir) {
            node1 = minmax.first;
            node2 = minmax.second;
        }
        matrix_[node1 * vertices_ + node2] = true;
        return 0;
    }

    virtual int rm_edge(int node1, int node2) override {
        auto minmax = std::minmax(node1, node2);
        if (minmax.second >= vertices_) {
            return -1;
        }
        if (!Dir) {
            node1 = minmax.first;
            node2 = minmax.second;
        }
        matrix_[node1 * vertices_ + node2] = false;
        return 0;
    }

    virtual bool is_edge(int node1, int node2) const override {
        auto minmax = std::minmax(node1, node2);
        if (minmax.second >= vertices_) {
            return -1;
        }
        if (!Dir) {
            node1 = minmax.first;
            node2 = minmax.second;
        }
        return matrix_[node1 * vertices_ + node2];
    }

    virtual int vertex_num() const { return vertices_; };

private:
    int vertices_;
    std::vector<bool> matrix_;
};
