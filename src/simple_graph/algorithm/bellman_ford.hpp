#pragma once

#include <algorithm>
#include <cassert>
#include <set>
#include <vector>
#include "simple_graph/graph.hpp"

namespace simple_graph {

template<typename W>
bool check_distance(const W &d, const W &w)
{
    return !((d > 0) && (w > 0) && (std::numeric_limits<W>::max() - d < w));
}

template<bool Dir, typename V, typename E, typename W>
bool bellman_ford(const Graph<Dir, V, E, W> &g, vertex_index_t start_idx, vertex_index_t goal_idx,
        std::vector<vertex_index_t> *path)
{
    // TODO add utility function to check passed vertex indices
    if ((start_idx < 0) || (goal_idx < 0)) {
        return false;
    }

    std::vector<W> distance(g.vertex_num(), std::numeric_limits<W>::max());
    std::vector<vertex_index_t> predecessor(g.vertex_num(), -1);

    distance[start_idx] = 0;

    auto fg = const_cast<Graph<Dir, V, E, W>*>(&g);

    std::vector<Edge<E, W>> asc_edges;
    std::vector<Edge<E, W>> desc_edges;
    for (const auto &edge : fg->edges()) {
        if (Dir) {
            if (edge.idx1() < edge.idx2()) {
                asc_edges.push_back(edge);
            }
            else {
                desc_edges.push_back(edge);
            }
        }
        else {
            asc_edges.push_back(edge);
            desc_edges.push_back(edge);
        }
    }
    std::sort(asc_edges.begin(), asc_edges.end(), [](const Edge<E, W> &a, const Edge<E, W> &b) {
        vertex_index_t a_idx = std::min(a.idx1(), a.idx2());
        vertex_index_t b_idx = std::min(b.idx1(), b.idx2());
        return a_idx < b_idx;
    });
    std::sort(desc_edges.begin(), desc_edges.end(), [](const Edge<E, W> &a, const Edge<E, W> &b) {
        vertex_index_t a_idx = std::max(a.idx1(), a.idx2());
        vertex_index_t b_idx = std::max(b.idx1(), b.idx2());
        return b_idx < a_idx;
    });

    for (size_t i = 0; i < g.vertex_num(); ++i) {
        bool changed = false;
        for (const auto &edge : asc_edges) {
            std::pair<vertex_index_t, vertex_index_t> e = std::minmax(edge.idx1(), edge.idx2());
            if (check_distance(distance[e.first], edge.weight())
                    && (distance[e.first] + edge.weight() < distance[e.second])) {
                distance[e.second] = distance[e.first] + edge.weight();
                predecessor[e.second] = e.first;
                changed = true;
            }
        }
        for (const auto &edge : desc_edges) {
            std::pair<vertex_index_t, vertex_index_t> e = std::minmax(edge.idx1(), edge.idx2());
            if (check_distance(distance[e.second], edge.weight())
                    && (distance[e.second] + edge.weight() < distance[e.first])) {
                distance[e.first] = distance[e.second] + edge.weight();
                predecessor[e.first] = e.second;
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

    if (distance[goal_idx] == std::numeric_limits<W>::max()) {
        return false;
    }

    vertex_index_t idx = goal_idx;
    while (idx != start_idx) {
        path->push_back(idx);
        idx = predecessor[idx];
        assert(idx != static_cast<vertex_index_t>(-1));
    }
    path->push_back(idx);
    std::reverse(path->begin(), path->end());

    return true;
}

}  // simple_graph
