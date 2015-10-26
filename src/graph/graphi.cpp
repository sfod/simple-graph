#include "graphi.hpp"
#include <algorithm>
#include <queue>

bool GraphI::find_path(int node1, int node2, std::vector<int> *path) const
{
    int vnum = vertex_num();
    if (std::max(node1, node2) > vnum) {
        return false;
    }

    std::vector<bool> visited(vnum, false);
    std::vector<int> prev(vnum, -1);
    std::vector<int> dist(vnum, vnum + 1);
    for (auto i = 0; i < vnum; ++i) {
        visited[i] = false;
        prev[i] = -1;
        dist[i] = vnum + 1;
    }

    std::queue<int> queue;

    bool path_found = false;

    queue.push(node1);
    dist[node1] = 0;

    while (!path_found && !queue.empty()) {
        int u = queue.front();
        queue.pop();
        visited[u] = true;

        for (auto v = 0; v < vnum; ++v) {
            if (is_edge(u, v) && !visited[v]) {
                int alt = dist[u] + 1;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }

                queue.push(v);
                if (v == node2) {
                    path_found = true;
                    break;
                }
            }
        }
    }

    if (path_found) {
        int v = node2;
        do {
            path->push_back(v);
            v = prev[v];
        } while (v != -1);
        // FIXME
        std::reverse(path->begin(), path->end());
    }

    return path_found;
}
