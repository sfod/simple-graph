#pragma once

#include <functional>
#include <stack>
#include <set>
#include "simple_graph/graph.hpp"

namespace simple_graph {

template<bool Dir, typename V, typename E>
bool dfs(const Graph<Dir, V, E> &g, vertex_index_t start_idx, std::function<bool(V)> &pred,
        std::vector<vertex_index_t> *path)
{
    size_t vnum = g.vertex_num();
    if (start_idx > vnum) {
        return false;
    }

    std::vector<bool> visited(vnum, false);
    std::vector<vertex_index_t> prev(vnum, 0);
    std::vector<size_t> dist(vnum, vnum + 1);
    std::stack<vertex_index_t> stack;

    bool vertex_found = false;

    stack.push(start_idx);
    dist[start_idx] = 0;
    vertex_index_t end_idx = 0;

    while (!vertex_found && !stack.empty()) {
        vertex_index_t u = stack.top();
        stack.pop();
        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        const std::set<vertex_index_t> &vertices = g.adjacent_vertices(u);

        for (auto v : vertices) {
            if (!visited[v]) {
                size_t alt = dist[u] + 1;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }

                stack.push(v);
                if (pred(g.vertex(v).data())) {
                    vertex_found = true;
                    end_idx = v;
                    break;
                }
            }
        }
    }

    if (vertex_found) {
        vertex_index_t v = end_idx;
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
