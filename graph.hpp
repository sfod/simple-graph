#pragma once

#include <vector>

class graph {
public:
    explicit graph(int vertices);
    ~graph();

    int add_edge(int node1, int node2);
    int rm_edge(int node1, int node2);
    bool is_edge(int node1, int node2) const;
    bool find_path(int node1, int node2, std::vector<int> *path) const;

private:
    int vertices_;
    bool *matrix_;
};

