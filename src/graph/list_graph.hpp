#pragma once

#include <algorithm>
#include <map>
#include <set>
#include "graphi.hpp"

template <bool Dir>
class ListGraph : public GraphI {
public:
    ListGraph() = default;
    virtual ~ListGraph() = default;

    virtual int add_edge(int node1, int node2) override {
        auto minmax = std::minmax(node1, node2);
        if (vertices_ < minmax.second + 1) {
            vertices_ = minmax.second + 1;
        }
        if (!Dir) {
            node1 = minmax.first;
            node2 = minmax.second;
        }
        edges_[node1].insert(node2);
        return 0;
    }

    virtual int rm_edge(int node1, int node2) override {
        auto minmax = std::minmax(node1, node2);
        if (!Dir) {
            node1 = minmax.first;
            node2 = minmax.second;
        }
        edges_[node1].erase(node2);
        return 0;
    }

    virtual bool is_edge(int node1, int node2) const override {
        auto minmax = std::minmax(node1, node2);
        if (!Dir) {
            node1 = minmax.first;
            node2 = minmax.second;
        }
        return (edges_.at(node1).count(node2) > 0);
    }

    virtual int vertex_num() const { return vertices_; };

private:
    int vertices_;
    std::map<int, std::set<int>> edges_;
};
