#pragma once

#include "graph.hpp"
#include <functional>
#include <queue>
#include <set>

namespace simple_graph {

template<typename V, typename E>
bool bfs(const Graph<V, E> &g, int start_idx, std::function<bool(V)> &pred, std::vector<int> *path)
{
    int vnum = g.vertex_num();
    if (start_idx > vnum) {
        return false;
    }

    std::vector<bool> visited(vnum, false);
    std::vector<int> prev(vnum, -1);
    std::vector<int> dist(vnum, vnum + 1);
    std::queue<int> queue;

    bool vertex_found = false;

    queue.push(start_idx);
    dist[start_idx] = 0;
    int end_idx = -1;

    while (!vertex_found && !queue.empty()) {
        int u = queue.front();
        queue.pop();
        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        std::set<int> vertices = g.adjacent_vertices(u);

        for (auto v : vertices) {
            if (!visited[v]) {
                int alt = dist[u] + 1;
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
        int v = end_idx;
        do {
            path->push_back(v);
            v = prev[v];
        } while (v != -1);
        std::reverse(path->begin(), path->end());
    }

    return vertex_found;
}

}  // namespace simple_graph
