#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <queue>
#include <set>
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
class Edge {
public:
    Edge(int idx1, int idx2) : idx1_(idx1), idx2_(idx2) {}
    virtual ~Edge() = default;
    int idx1() const { return idx1_; }
    int idx2() const { return idx2_; }

private:
    int idx1_;
    int idx2_;
};

template<typename V, typename E>
class GraphI {
public:
    virtual ~GraphI() = default;

    virtual int add_vertex(const Vertex<V> &vertex) = 0;
    virtual int set_vertex(const Vertex<V> &vertex) = 0;
    virtual void rm_vertex(const Vertex<V> &vertex) = 0;
    virtual const std::set<int> &adjacent_vertices(int idx) const = 0;

    virtual const Vertex<V> &vertex(int idx) const = 0;
    virtual int vertex_num() const = 0;

    virtual int add_edge(const Edge<E> &edge) = 0;
    virtual int rm_edge(const Edge<E> &edge) = 0;

    virtual bool bfs(int start_idx, std::function<bool(V)> &pred, std::vector<int> *path) const;
};


template<typename V, typename E>
bool GraphI<V, E>::bfs(int start_idx, std::function<bool(V)> &pred, std::vector<int> *path) const
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
        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        std::set<int> vertices = adjacent_vertices(u);

        for (auto v : vertices) {
            if (!visited[v]) {
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
