#pragma once

#include <memory>
#include <set>

// TODO use move semantics

namespace simple_graph {

typedef ssize_t vertex_index_t;

template<typename T>
class Vertex {
public:
    Vertex() : idx_(-1), data_() {
        ++default_creations;
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
    explicit Vertex(vertex_index_t idx) : idx_(idx), data_() {
        ++creations;
    }
    Vertex(vertex_index_t idx, const T &data) : idx_(idx), data_(data) {
        ++creations;
    }
    virtual ~Vertex() = default;

    vertex_index_t idx() const { return idx_; }
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

public:
    static int default_creations;
    static int creations;
    static int copies;
    static int moves;
    static int assigns;

    static std::string stat() {
        return "creations: " + std::to_string(default_creations) + " + " + std::to_string(creations)
                + "; copies: " + std::to_string(copies)
                + "; moves: " + std::to_string(moves)
                + "; assigns: " + std::to_string(assigns);
    }

private:
    vertex_index_t idx_;
    T data_;
};

template<typename T> int Vertex<T>::default_creations = 0;
template<typename T> int Vertex<T>::creations = 0;
template<typename T> int Vertex<T>::copies = 0;
template<typename T> int Vertex<T>::moves = 0;
template<typename T> int Vertex<T>::assigns = 0;

template<typename T>
class Edge {
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
            "Integer of floating point required.");

public:
    Edge() : idx1_(-1), idx2_(-1), weight_() {
        ++default_creations;
    }
    Edge(const Edge<T> &edge) : idx1_(edge.idx1_), idx2_(edge.idx2_), weight_(edge.weight_) {
        ++copies;
    }
    Edge(Edge<T> &&edge) noexcept(noexcept(
                std::is_nothrow_move_constructible<T>::value
                    && std::is_nothrow_move_assignable<T>::value)) : idx1_(edge.idx1_), idx2_(edge.idx2_) {
        ++moves;
        std::swap(weight_, edge.weight_);
    }

    Edge(vertex_index_t idx1, vertex_index_t idx2) : idx1_(idx1), idx2_(idx2), weight_(1) {
        ++creations;
    }
    Edge(vertex_index_t idx1, vertex_index_t idx2, const T &weight) : idx1_(idx1), idx2_(idx2), weight_(weight) {
        ++creations;
    }

    Edge &operator=(const Edge<T> &edge) {
        ++assigns;
        if (this != &edge) {
            idx1_ = edge.idx1_;
            idx2_ = edge.idx2_;
            weight_ = edge.weight_;
        }
        return *this;
    }

    Edge &operator=(Edge<T> &&edge) noexcept(noexcept(
                std::is_nothrow_move_constructible<T>::value
                    && std::is_nothrow_move_assignable<T>::value)) {
        ++moves;
        if (this != &edge) {
            idx1_ = edge.idx1_;
            idx2_ = edge.idx2_;
            std::swap(weight_, edge.weight_);
        }
        return *this;
    }

    virtual ~Edge() = default;
    vertex_index_t idx1() const { return idx1_; }
    vertex_index_t idx2() const { return idx2_; }
    const T &weight() const { return weight_; }

public:
    static int default_creations;
    static int creations;
    static int copies;
    static int moves;
    static int assigns;

    static std::string stat() {
        return "creations: " + std::to_string(default_creations) + " + " + std::to_string(creations)
                + "; copies: " + std::to_string(copies)
                + "; moves: " + std::to_string(moves)
                + "; assigns: " + std::to_string(assigns);
    }
private:
    vertex_index_t idx1_;
    vertex_index_t idx2_;
    T weight_;
};

template<typename T> int Edge<T>::default_creations = 0;
template<typename T> int Edge<T>::creations = 0;
template<typename T> int Edge<T>::copies = 0;
template<typename T> int Edge<T>::moves = 0;
template<typename T> int Edge<T>::assigns = 0;

template<typename T>
class IteratorImplBase {
public:
    virtual ~IteratorImplBase() = default;
    virtual bool operator==(const IteratorImplBase<T> &it) const = 0;
    virtual bool operator!=(const IteratorImplBase<T> &it) const = 0;
    virtual IteratorImplBase<T> &operator++() = 0;
    virtual T &operator*() = 0;
};

template<typename T>
class iterator {
public:
    iterator(std::shared_ptr<IteratorImplBase<T>> b) : base_impl(b) {}

    bool operator==(const iterator<T> &it) const {
        return base_impl->operator==(*it.base_impl);
    }

    bool operator!=(const iterator<T> &it) const {
        return !operator==(it);
    }

    iterator &operator++() {
        base_impl->operator++();
        return *this;
    }

    T &operator*() {
        return base_impl->operator*();
    }

private:
    std::shared_ptr<IteratorImplBase<T>> base_impl;
};


template<bool Dir, typename V, typename E>
class Graph {
protected:
    class EdgesWrapper {
    public:
        virtual iterator<Edge<E>> begin() = 0;
        virtual iterator<Edge<E>> end() = 0;
    };

public:
    // TODO add constructor from std::initializer_list
    virtual ~Graph() = default;

    virtual void add_vertex(Vertex<V> vertex) = 0;
    virtual void rm_vertex(vertex_index_t idx) = 0;
    // TODO measure performance
    virtual const std::set<vertex_index_t> &adjacent_vertices(vertex_index_t idx) const = 0;

    virtual const Vertex<V> &vertex(vertex_index_t idx) const = 0;
    virtual size_t vertex_num() const = 0;

    virtual void add_edge(Edge<E> edge) = 0;
    virtual const Edge<E> &edge(vertex_index_t idx1, vertex_index_t idx2) const = 0;
    virtual void rm_edge(const Edge<E> &edge) = 0;

    virtual bool filter_edge(const Edge<E> &edge) = 0;

    virtual EdgesWrapper &edges() = 0;
};

}  // namespace simple_graph
