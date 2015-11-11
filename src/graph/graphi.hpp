#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <queue>
#include <vector>

template<typename T>
class Vertex {
public:
    Vertex() : idx_(-1), data_() {}
    explicit Vertex(int idx) : idx_(idx), data_() {}
    Vertex(int idx, const T &data) : idx_(idx), data_(data) {}
    virtual ~Vertex() = default;

    int idx() const { return idx_; }
    T data() const { return data_; }

    bool operator<(const Vertex<T> &vertex) const {
        return idx_ < vertex.idx_;
    }

private:
    int idx_;
    T data_;
};

template<typename T>
class GraphI {
public:
    virtual ~GraphI() = default;

    virtual int add_vertex(const Vertex<T> &vertex) = 0;
    virtual int set_vertex(const Vertex<T> &vertex) = 0;
    virtual void rm_vertex(const Vertex<T> &vertex) = 0;

    virtual const Vertex<T> &vertex(int idx) const = 0;
    virtual int vertex_num() const = 0;

    virtual int add_edge(int idx1, int idx2) = 0;
    virtual int rm_edge(int idx1, int idx2) = 0;
    virtual bool is_edge(int idx1, int idx2) const = 0;

    virtual bool bfs(int start_idx, std::function<bool(T)> &pred, std::vector<int> *path) const;
};


template<typename T>
bool GraphI<T>::bfs(int start_idx, std::function<bool(T)> &pred, std::vector<int> *path) const
{
    int vnum = vertex_num();
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
        visited[u] = true;

        for (auto v = 0; v < vnum; ++v) {
            if (is_edge(u, v) && !visited[v]) {
                int alt = dist[u] + 1;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }

                queue.push(v);
                if (pred(vertex(v).data())) {
                    vertex_found = true;
                    end_idx = v;
                    break;
                }
            }
        }
    }

    if (vertex_found) {
        int v = end_idx;
        do {
            path->push_back(v);
            v = prev[v];
        } while (v != -1);
        // FIXME
        std::reverse(path->begin(), path->end());
    }

    return vertex_found;
}
