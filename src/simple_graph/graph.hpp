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
    Vertex() : idx_(0), data_() {
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
    explicit Vertex(size_t idx) : idx_(idx), data_() {
        ++creations;
    }
    Vertex(size_t idx, const T &data) : idx_(idx), data_(data) {}
    virtual ~Vertex() = default;

    size_t idx() const { return idx_; }
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
    size_t idx_;
    T data_;
};

// TODO limit possible types of T
template<typename T>
class Edge {
public:
    Edge() : idx1_(0), idx2_(0), weight_() {}
    Edge(size_t idx1, size_t idx2) : idx1_(idx1), idx2_(idx2), weight_(1) {}
    Edge(size_t idx1, size_t idx2, const T &weight) : idx1_(idx1), idx2_(idx2), weight_(weight) {}

    virtual ~Edge() = default;
    size_t idx1() const { return idx1_; }
    size_t idx2() const { return idx2_; }
    const T &weight() const { return weight_; }

private:
    size_t idx1_;
    size_t idx2_;
    T weight_;
};


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
    iterator(IteratorImplBase<T> *b) : base_impl(b) {}

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
    IteratorImplBase<T> *base_impl;
};


template<typename V, typename E>
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

    virtual int add_vertex(const Vertex<V> &vertex) = 0;
    virtual int add_vertex(Vertex<V> &&vertex) = 0;
    virtual int set_vertex(const Vertex<V> &vertex) = 0;
    virtual void rm_vertex(const Vertex<V> &vertex) = 0;
    // TODO measure performance
    virtual const std::set<size_t> &adjacent_vertices(size_t idx) const = 0;

    virtual const Vertex<V> &vertex(size_t idx) const = 0;
    virtual size_t vertex_num() const = 0;

    virtual int add_edge(const Edge<E> &edge) = 0;
    virtual const Edge<E> &edge(size_t idx1, size_t idx2) const = 0;
    virtual int rm_edge(const Edge<E> &edge) = 0;

    virtual EdgesWrapper &edges() = 0;
};

}  // namespace simple_graph
