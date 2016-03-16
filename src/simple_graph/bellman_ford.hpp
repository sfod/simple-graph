#pragma once

#include <vector>
#include "graph.hpp"

namespace simple_graph {

template<typename V, typename E>
bool bellman_ford(const Graph<V, E> &g, size_t start_idx, size_t goal_idx, std::vector<size_t> *path)
{
    std::vector<E> distance(g.vertex_num(), std::numeric_limits<E>::max());
    std::vector<size_t> predecessor(g.vertex_num(), 0);

    distance[start_idx] = 0;

    Graph<V, E> *fg = const_cast<Graph<V, E>*>(&g);
    for (size_t i = 0; i < g.vertex_num(); ++i) {
        bool changed = false;
        for (auto e : fg->edges()) {
            if (distance[e.idx1()] + e.weight() < distance[e.idx2()]) {
                distance[e.idx2()] = distance[e.idx1()] + e.weight();
                predecessor[e.idx2()] = e.idx1();
                changed = true;
            }
        }
        if (!changed) {
            break;
        }
    }

    for (auto e : fg->edges()) {
        if (distance[e.idx1()] + e.weight() < distance[e.idx2()]) {
            return false;
        }
    }

    if (distance[goal_idx] == 0) {
        return false;
    }

    size_t idx = goal_idx;
    while (idx != start_idx) {
        path->push_back(idx);
        idx = predecessor[idx];
    }
    path->push_back(idx);
    std::reverse(path->begin(), path->end());

    return true;
}

}  // simple_graph
