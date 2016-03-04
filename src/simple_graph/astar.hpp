#pragma once

#include "graph.hpp"
#include <functional>
#include <set>

namespace simple_graph {

size_t min_idx(const std::set<size_t> &opened, const std::vector<float> &f_score)
{
    size_t min_idx = 0;

    float min = std::numeric_limits<float>::max();
    bool inited = false;
    for (auto idx : opened) {
        if ((f_score[idx] < min) || !inited) {
            min_idx = idx;
            min = f_score[idx];
            inited = true;
        }
    }

    return min_idx;
}

template<typename V, typename E>
bool astar(const Graph<V, E> &g, size_t start_idx, size_t goal_idx,
        std::function<float(size_t, size_t)> &heuristic, std::vector<size_t> *path)
{
    size_t vnum = g.vertex_num();

    std::set<size_t> closed;
    std::set<size_t> opened;
    std::vector<size_t> came_from(vnum, 0);
    std::vector<float> g_score(vnum, std::numeric_limits<float>::max());
    std::vector<float> f_score(vnum, std::numeric_limits<float>::max());

    g_score[start_idx] = 0;
    f_score[start_idx] = heuristic(start_idx, goal_idx);

    opened.insert(start_idx);

    bool vertex_found = false;
    while (!vertex_found && !opened.empty()) {
        size_t current = min_idx(opened, f_score);
        if (current == goal_idx) {
            vertex_found = true;
            break;
        }
        opened.erase(current);
        closed.insert(current);

        std::set<size_t> neighbours = g.adjacent_vertices(current);
        for (auto neighbour : neighbours) {
            if (closed.count(neighbour) > 0) {
                continue;
            }

            float tentative_score = g_score[current] + g.edge(current, neighbour).weight();
            if (opened.count(neighbour) == 0) {
                opened.insert(neighbour);
            }
            else if (tentative_score >= g_score[neighbour]) {
                continue;
            }

            came_from[neighbour] = current;
            g_score[neighbour] = tentative_score;
            f_score[neighbour] = g_score[neighbour] + heuristic(neighbour, goal_idx);
        }
    }

    // FIXME optimize
    if (vertex_found) {
        size_t idx = goal_idx;
        do {
            path->push_back(idx);
            idx = came_from[idx];
        } while (idx != start_idx);
        path->push_back(idx);
        std::reverse(path->begin(), path->end());
    }

    return vertex_found;
}

}  // namespace simple_graph
