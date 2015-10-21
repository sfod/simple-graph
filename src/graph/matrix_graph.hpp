#pragma once

#include <vector>
#include "graphi.hpp"

class MatrixGraph : public GraphI {
public:
    explicit MatrixGraph(int vertices);
    ~MatrixGraph();

    virtual int add_edge(int node1, int node2) override;
    virtual int rm_edge(int node1, int node2) override;
    virtual bool is_edge(int node1, int node2) const override;
    virtual bool find_path(int node1, int node2, std::vector<int> *path) const override;

private:
    int vertices_;
    bool *matrix_;
};
