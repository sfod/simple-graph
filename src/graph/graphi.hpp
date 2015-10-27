#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>
#include <queue>

template<typename T>
class Vertex {
public:
    Vertex() : idx_(-1), data_() {}
    explicit Vertex(int idx) : idx_(idx), data_() {}
    Vertex(int idx, const T &data) : idx_(idx), data_(data) {}
    virtual ~Vertex() = default;

    int idx() const { return idx_; }

    bool operator<(const Vertex<T> &node) const {
        return idx_ < node.idx_;
    }

private:
    int idx_;
    T data_;
};

template<typename T>
class GraphI {
public:
    virtual ~GraphI() = default;
    virtual int add_edge(const Vertex<T> &node1, const Vertex<T> &node2) = 0;
    virtual int rm_edge(const Vertex<T> &node1, const Vertex<T> &node2) = 0;

    virtual int vertex_num() const = 0;
    virtual const Vertex<T> &vertex(int idx) const = 0;

    virtual bool find_path(const Vertex<T> &node1, const Vertex<T> &node2, std::vector<int> *path) const;

    virtual bool is_edge(int idx1, int idx2) const = 0;
};


template<typename T>
bool GraphI<T>::find_path(const Vertex<T> &node1, const Vertex<T> &node2, std::vector<int> *path) const
{
    int vnum = vertex_num();
    if (std::max(node1.idx(), node2.idx()) > vnum) {
        return false;
    }

    std::vector<bool> visited(vnum, false);
    std::vector<int> prev(vnum, -1);
    std::vector<int> dist(vnum, vnum + 1);
    std::queue<int> queue;

    bool path_found = false;

    queue.push(node1.idx());
    dist[node1.idx()] = 0;

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
                if (v == node2.idx()) {
                    path_found = true;
                    break;
                }
            }
        }
    }

    if (path_found) {
        int v = node2.idx();
        do {
            path->push_back(v);
            v = prev[v];
        } while (v != -1);
        // FIXME
        std::reverse(path->begin(), path->end());
    }

    return path_found;
}
