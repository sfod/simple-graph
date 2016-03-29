#pragma once

#include "graph.hpp"
#include <functional>
#include <queue>
#include <set>

namespace simple_graph {

template<typename V, typename E>
bool bfs(const Graph<V, E> &g, size_t start_idx, std::function<bool(V)> &pred, std::vector<size_t> *path)
{
    size_t vnum = g.vertex_num();
    if (start_idx > vnum) {
        return false;
    }

    std::vector<bool> visited(vnum, false);
    std::vector<size_t> prev(vnum, 0);
    std::vector<size_t> dist(vnum, vnum + 1);
    std::queue<size_t> queue;

    bool vertex_found = false;

    queue.push(start_idx);
    dist[start_idx] = 0;
    size_t end_idx = 0;

    while (!vertex_found && !queue.empty()) {
        size_t u = queue.front();
        queue.pop();
        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        const std::set<size_t> &vertices = g.adjacent_vertices(u);

        for (auto v : vertices) {
            if (!visited[v]) {
                size_t alt = dist[u] + 1;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }

                queue.push(v);
                if (pred(g.vertex(v).data())) {
                    vertex_found = true;
                    end_idx = v;
                    break;
                }
            }
        }
    }

    // FIXME optimize
    if (vertex_found) {
        size_t v = end_idx;
        do {
            path->push_back(v);
            v = prev[v];
        } while (v != start_idx);
        path->push_back(v);
        std::reverse(path->begin(), path->end());
    }

    return vertex_found;
}

}  // namespace simple_graph
