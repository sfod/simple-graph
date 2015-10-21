#pragma once
#include <vector>
class GraphI {
public:
    virtual ~GraphI() = default;
    virtual int add_edge(int node1, int node2) = 0;
    virtual int rm_edge(int node1, int node2) = 0;
    virtual bool is_edge(int node1, int node2) const = 0;
    virtual bool find_path(int node1, int node2, std::vector<int> *path) const = 0;
};
