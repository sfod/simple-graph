#include <algorithm>
#include <queue>
#include "graph.hpp"

Graph::Graph(int vertices) : vertices_(vertices)
{
    matrix_ = new bool[vertices * vertices];
    for (auto i = 0; i < vertices * vertices; ++i) {
        matrix_[i] = false;
    }
}

Graph::~Graph()
{
    delete[] matrix_;
}

int Graph::add_edge(int node1, int node2)
{
    if ((node1 >= vertices_) || (node2 >= vertices_)) {
        return -1;
    }

    matrix_[node1 * vertices_ + node2] = true;
    matrix_[node2 * vertices_ + node1] = true;
    return 0;
}

int Graph::rm_edge(int node1, int node2)
{
    if ((node1 >= vertices_) || (node2 >= vertices_)) {
        return -1;
    }
    matrix_[node1 * vertices_ + node2] = false;
    matrix_[node2 * vertices_ + node1] = false;
    return 0;
}

bool Graph::is_edge(int node1, int node2) const
{
    if ((node1 >= vertices_) || (node2 >= vertices_)) {
        return false;
    }
    return matrix_[node1 * vertices_ + node2];
}

bool Graph::find_path(int node1, int node2, std::vector<int> *path) const
{
    bool visited[vertices_];
    int prev[vertices_];
    int dist[vertices_];
    for (auto i = 0; i < vertices_; ++i) {
        visited[i] = false;
        prev[i] = -1;
        dist[i] = vertices_ + 1;
    }

    std::queue<int> queue;

    bool path_found = false;

    queue.push(node1);
    dist[node1] = 0;

    while (!path_found && !queue.empty()) {
        int u = queue.front();
        queue.pop();
        visited[u] = true;

        for (auto v = 0; v < vertices_; ++v) {
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
        std::reverse(path->begin(), path->end());
    }

    return path_found;
}
