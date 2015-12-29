#pragma once

#include <set>

// TODO use move semantics

namespace simple_graph {

int creations = 0;
int copies = 0;
int moves = 0;
int assigns = 0;

template<typename T>
class Vertex {
public:
    Vertex() : idx_(-1), data_() {
        ++creations;
    }
    Vertex(const Vertex<T> &v) {
        ++copies;
        idx_ = v.idx_;
        data_ = v.data_;
    }
    Vertex(Vertex<T> &&v) noexcept(noexcept(
                std::is_nothrow_move_constructible<T>::value
                    && std::is_nothrow_move_assignable<T>::value)) {
        ++moves;
        idx_ = v.idx_;
        std::swap(data_, v.data_);
    }
    explicit Vertex(int idx) : idx_(idx), data_() {
        ++creations;
    }
    Vertex(int idx, const T &data) : idx_(idx), data_(data) {}
    virtual ~Vertex() = default;

    int idx() const { return idx_; }
    T data() const { return data_; }

    bool operator<(const Vertex<T> &vertex) const {
        return idx_ < vertex.idx_;
    }

    Vertex &operator=(const Vertex<T> &v) {
        ++assigns;
        if (this != &v) {
            idx_ = v.idx_;
            data_ = v.data_;
        }
        return *this;
    }

    Vertex &operator=(Vertex<T> &&v) noexcept(noexcept(
                std::is_nothrow_move_constructible<T>::value
                    && std::is_nothrow_move_assignable<T>::value)) {
        ++moves;
        if (this != &v) {
            idx_ = v.idx_;
            std::swap(data_, v.data_);
        }
        return *this;
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
    // TODO add constructor from std::initializer_list
    virtual ~Graph() = default;

    virtual int add_vertex(const Vertex<V> &vertex) = 0;
    virtual int add_vertex(Vertex<V> &&vertex) = 0;
    virtual int set_vertex(const Vertex<V> &vertex) = 0;
    virtual void rm_vertex(const Vertex<V> &vertex) = 0;
    // TODO measure performance
    virtual const std::set<int> &adjacent_vertices(int idx) const = 0;

    virtual const Vertex<V> &vertex(int idx) const = 0;
    virtual int vertex_num() const = 0;

    virtual int add_edge(const Edge<E> &edge) = 0;
    virtual const Edge<E> &edge(int idx1, int idx2) const = 0;
    virtual int rm_edge(const Edge<E> &edge) = 0;
};

}  // namespace simple_graph
