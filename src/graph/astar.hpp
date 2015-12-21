#pragma once

#include "graphi.hpp"
#include <functional>
#include <set>

int min_idx(const std::set<int> &opened, const std::vector<float> &f_score)
{
    int min_idx = -1;
    float min = std::numeric_limits<float>::max();
    for (auto idx : opened) {
        if ((f_score[idx] < min) || (min_idx == -1)) {
            min_idx = idx;
            min = f_score[idx];
        }
    }
    return min_idx;
}

template<typename V, typename E>
bool astar(const GraphI<V, E> &g, int start_idx, int goal_idx,
        std::function<float(int, int)> &heuristic, std::vector<int> *path)
{
    int vnum = g.vertex_num();

    std::set<int> closed;
    std::set<int> opened;
    std::vector<int> came_from(vnum, -1);
    std::vector<float> g_score(vnum, std::numeric_limits<float>::max());
    std::vector<float> f_score(vnum, std::numeric_limits<float>::max());

    g_score[start_idx] = 0;
    f_score[start_idx] = heuristic(start_idx, goal_idx);

    opened.insert(start_idx);

    bool vertex_found = false;
    while (!vertex_found && !opened.empty()) {
        int current = min_idx(opened, f_score);
        if (current == goal_idx) {
            vertex_found = true;
            break;
        }
        opened.erase(current);
        closed.insert(current);

        std::set<int> neighbours = g.adjacent_vertices(current);
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

    if (vertex_found) {
        int idx = goal_idx;
        do {
            path->push_back(idx);
            idx = came_from[idx];
        } while (idx != -1);
        std::reverse(path->begin(), path->end());
    }

    return vertex_found;
}
