#pragma once

#include <cassert>
#include <vector>
#include "simple_graph/graph.hpp"

namespace simple_graph {

template<typename E>
bool check_distance(const E &d, const E &w)
{
    if ((d > 0) && (w >0) && (std::numeric_limits<E>::max() - d < w)) {
        return false;
    }
    return true;
}

template<bool Dir, typename V, typename E>
bool bellman_ford(const Graph<Dir, V, E> &g, vertex_index_t start_idx, vertex_index_t goal_idx,
        std::vector<vertex_index_t> *path)
{
    // TODO add utility function to check passed vertex indices
    if ((start_idx < 0) || (goal_idx < 0)) {
        return false;
    }

    std::vector<E> distance(g.vertex_num(), std::numeric_limits<E>::max());
    std::vector<vertex_index_t> predecessor(g.vertex_num(), -1);

    distance[start_idx] = 0;

    Graph<Dir, V, E> *fg = const_cast<Graph<Dir, V, E>*>(&g);
    for (size_t i = 0; i < g.vertex_num(); ++i) {
        bool changed = false;
        for (const auto &edge : fg->edges()) {
            if (check_distance(distance[edge.idx1()], edge.weight())
                    && (distance[edge.idx1()] + edge.weight() < distance[edge.idx2()])) {
                distance[edge.idx2()] = distance[edge.idx1()] + edge.weight();
                predecessor[edge.idx2()] = edge.idx1();
                changed = true;
            }
        }
        if (!changed) {
            break;
        }
    }

    for (const auto &edge : fg->edges()) {
        if (check_distance(distance[edge.idx1()], edge.weight())
                && (distance[edge.idx1()] + edge.weight() < distance[edge.idx2()])) {
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
