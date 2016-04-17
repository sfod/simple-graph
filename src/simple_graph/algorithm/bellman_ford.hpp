#pragma once

#include <cassert>
#include <set>
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

    auto asc_comp = [](const Edge<E> &a, const Edge<E> &b) {
        std::pair<vertex_index_t, vertex_index_t> ap = std::minmax(a.idx1(), a.idx2());
        std::pair<vertex_index_t, vertex_index_t> bp = std::minmax(b.idx1(), b.idx2());
        if (ap.first != bp.first) {
            return ap.first < bp.first;
        }
        else {
            return ap.second < bp.second;
        }
    };
    auto desc_comp = [](const Edge<E> &a, const Edge<E> &b) {
        std::pair<vertex_index_t, vertex_index_t> ap = std::minmax(a.idx1(), a.idx2());
        std::pair<vertex_index_t, vertex_index_t> bp = std::minmax(b.idx1(), b.idx2());
        if (ap.second != bp.second) {
            return ap.second > bp.second;
        }
        else {
            return ap.first > bp.first;
        }
    };

    std::set<Edge<E>, decltype(asc_comp)> asc_edges(asc_comp);
    std::set<Edge<E>, decltype(desc_comp)> desc_edges(desc_comp);

    for (const auto &edge : fg->edges()) {
        if (Dir) {
            if (edge.idx1() < edge.idx2()) {
                asc_edges.insert(edge);
            }
            else {
                desc_edges.insert(edge);
            }
        }
        else {
            asc_edges.insert(edge);
            desc_edges.insert(edge);
        }
    }

    for (size_t i = 0; i < g.vertex_num(); ++i) {
        bool changed = false;
        auto it = asc_edges.begin();
        while (it != asc_edges.end()) {
            bool rm_it = false;
            const auto &edge = *it;
            std::pair<vertex_index_t, vertex_index_t> e = std::minmax(edge.idx1(), edge.idx2());
            if (check_distance(distance[e.first], edge.weight())) {
                if (distance[e.first] + edge.weight() < distance[e.second]) {
                    distance[e.second] = distance[e.first] + edge.weight();
                    predecessor[e.second] = e.first;
                    changed = true;
                }
                else {
                    rm_it = true;
                }
            }

            if (rm_it) {
                it = asc_edges.erase(it);
            }
            else {
                ++it;
            }
        }
        it = desc_edges.begin();
        while (it != desc_edges.end()) {
            bool rm_it = false;
            const auto &edge = *it;
            std::pair<vertex_index_t, vertex_index_t> e = std::minmax(edge.idx1(), edge.idx2());
            if (check_distance(distance[e.second], edge.weight())) {
                if (distance[e.second] + edge.weight() < distance[e.first]) {
                    distance[e.first] = distance[e.second] + edge.weight();
                    predecessor[e.first] = e.second;
                    changed = true;
                }
                else {
                    rm_it = true;
                }
            }

            if (rm_it) {
                it = desc_edges.erase(it);
            }
            else {
                ++it;
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
