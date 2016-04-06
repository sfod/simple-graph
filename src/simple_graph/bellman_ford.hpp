#pragma once

#include <cassert>
#include <vector>
#include "graph.hpp"

namespace simple_graph {

template<typename E>
bool check_distance(const E &d, const E &w)
{
    if ((d > 0) && (w >0) && (std::numeric_limits<E>::max() - d < w)) {
        return false;
    }
    return true;
}

template<typename V, typename E>
bool bellman_ford(const Graph<V, E> &g, vertex_index_t start_idx, vertex_index_t goal_idx,
        std::vector<vertex_index_t> *path)
{
    // TODO add utility function to check passed vertex indices
    if ((start_idx < 0) || (goal_idx < 0)) {
        return false;
    }

    std::vector<E> distance(g.vertex_num(), std::numeric_limits<E>::max());
    std::vector<vertex_index_t> predecessor(g.vertex_num(), -1);

    distance[start_idx] = 0;

    Graph<V, E> *fg = const_cast<Graph<V, E>*>(&g);
    for (size_t i = 0; i < g.vertex_num(); ++i) {
        bool changed = false;
        for (auto e : fg->edges()) {
            if (check_distance(distance[e.idx1()], e.weight())
                    && (distance[e.idx1()] + e.weight() < distance[e.idx2()])) {
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
        if (check_distance(distance[e.idx1()], e.weight())
                && (distance[e.idx1()] + e.weight() < distance[e.idx2()])) {
            return false;
        }
    }

    if (distance[goal_idx] == std::numeric_limits<E>::max()) {
        return false;
    }

    vertex_index_t idx = goal_idx;
    while (idx != start_idx) {
        path->push_back(idx);
        idx = predecessor[idx];
        assert(idx != -1);
    }
    path->push_back(idx);
    std::reverse(path->begin(), path->end());

    return true;
}

}  // simple_graph
