#pragma once

#include <vector>

template<typename Imp, typename Dir>
class Graph {
public:
    Graph();
    virtual ~Graph();

    int add_edge(int node1, int node2);
    virtual bool find_path(int node1, int node2, std::vector<int> *path) const;

private:
    Imp graph_;
};

template<typename Imp, typename D>
Graph<Imp, D>::Graph() : graph_()
{
}

template<typename Imp, typename D>
Graph<Imp, D>::~Graph()
{

}

template<typename Imp, typename D>
int Graph<Imp, D>::add_edge(int node1, int node2)
{
    graph_.add_edge(node1, node2);
    return 0;
}

template<typename Imp, typename D>
bool Graph<Imp, D>::find_path(int node1, int node2, std::vector<int> *path) const
{
    return graph_.find_path(node1, node2, path);
}
