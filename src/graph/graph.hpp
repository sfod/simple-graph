#pragma once

#include <set>

namespace simple_graph {

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
    Edge() : idx1_(-1), idx2_(-1), weight_() {}
    Edge(int idx1, int idx2) : idx1_(idx1), idx2_(idx2), weight_(1) {}
    Edge(int idx1, int idx2, const T &weight) : idx1_(idx1), idx2_(idx2), weight_(weight) {}

    virtual ~Edge() = default;
    int idx1() const { return idx1_; }
    int idx2() const { return idx2_; }
    const T &weight() const { return weight_; }

private:
    int idx1_;
    int idx2_;
    T weight_;
};

template<typename V, typename E>
class Graph {
public:
    virtual ~Graph() = default;

    virtual int add_vertex(const Vertex<V> &vertex) = 0;
    virtual int set_vertex(const Vertex<V> &vertex) = 0;
    virtual void rm_vertex(const Vertex<V> &vertex) = 0;
    virtual const std::set<int> &adjacent_vertices(int idx) const = 0;

    virtual const Vertex<V> &vertex(int idx) const = 0;
    virtual int vertex_num() const = 0;

    virtual int add_edge(const Edge<E> &edge) = 0;
    virtual const Edge<E> &edge(int idx1, int idx2) const = 0;
    virtual int rm_edge(const Edge<E> &edge) = 0;
};

}  // namespace simple_graph
