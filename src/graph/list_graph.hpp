#pragma once

#include <algorithm>
#include <map>
#include <set>
#include "graphi.hpp"

template <bool Dir, typename T>
class ListGraph : public GraphI<T> {
public:
    ListGraph() : vertices_(0), edges_() {}
    virtual ~ListGraph() = default;

    virtual int add_edge(const Node<T> &node1, const Node<T> &node2) override {
        auto minmax = std::minmax(node1, node2);
        if (vertices_ < minmax.second.idx() + 1) {
            vertices_ = minmax.second.idx() + 1;
        }
        int idx1 = node1.idx();
        int idx2 = node2.idx();
        if (!Dir) {
            idx1 = minmax.first.idx();
            idx2 = minmax.second.idx();
        }
        edges_[idx1].insert(idx2);
        return 0;
    }

    virtual int rm_edge(const Node<T> &node1, const Node<T> &node2) override {
        auto minmax = std::minmax(node1, node2);
        int idx1 = node1.idx();
        int idx2 = node2.idx();
        if (!Dir) {
            idx1 = minmax.first.idx();
            idx2 = minmax.second.idx();
        }
        edges_[idx1].erase(idx2);
        return 0;
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

    virtual int vertex_num() const override { return vertices_; };

private:
    int vertices_;
    std::map<int, std::set<int>> edges_;
};
