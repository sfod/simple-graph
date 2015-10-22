#pragma once

#include <map>
#include <set>
#include "graphi.hpp"

class ListGraph : public GraphI {
public:
    ListGraph();
    virtual ~ListGraph() = default;

    virtual int add_edge(int node1, int node2) override;
    virtual int rm_edge(int node1, int node2) override;
    virtual bool is_edge(int node1, int node2) const override;
    virtual int vertex_num() const { return vertices_; };

private:
    int vertices_;
    std::map<int, std::set<int>> edges_;
};
