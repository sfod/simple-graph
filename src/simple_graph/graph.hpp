#pragma once

#include <memory>
#include <set>
#include <vector>

// TODO use move semantics

namespace simple_graph {

typedef size_t vertex_index_t;

template<typename T>
class Vertex {
public:
    Vertex() : idx_(static_cast<vertex_index_t >(-1)), data_() {
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

/// counters to track down objects manipulations
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


/**
 * Class representing edge in a graph.
 *
 * @tparam P Typename for edge additional parameters.
 * @tparam W Typename for edge weight, must be numeric.
 */
template<typename P, typename W = float>
class Edge {
    static_assert(std::is_integral<W>::value || std::is_floating_point<W>::value,
            "Integer or floating-point number required for weight typename.");

public:
    Edge() : idx1_(-1), idx2_(-1), params_(), weight_()
    {
        ++default_creations;
    }

    Edge(const Edge<P, W> &edge) : idx1_(edge.idx1_), idx2_(edge.idx2_), params_(std::move(edge.params_)), weight_(edge.weight_)
    {
        ++copies;
    }

    Edge(Edge<P, W> &&edge) noexcept(noexcept(
                std::is_nothrow_move_constructible<W>::value
                    && std::is_nothrow_move_assignable<W>::value))
        : idx1_(edge.idx1_), idx2_(edge.idx2_), params_(std::move(edge.params_)), weight_(edge.weight_)
    {
        ++moves;
    }

    Edge(vertex_index_t idx1, vertex_index_t idx2, P params)
        : idx1_(idx1), idx2_(idx2), params_(std::move(params)), weight_(1)
    {
        ++creations;
    }

    Edge(vertex_index_t idx1, vertex_index_t idx2, P params, W weight)
        : idx1_(idx1), idx2_(idx2), params_(std::move(params)), weight_(weight)
    {
        ++creations;
    }

    virtual ~Edge() = default;

    Edge &operator=(const Edge<P, W> &edge)
    {
        ++assigns;
        if (this != &edge) {
            idx1_ = edge.idx1_;
            idx2_ = edge.idx2_;
            params_ = edge.params_;
            weight_ = edge.weight_;
        }
        return *this;
    }

    Edge &operator=(Edge<P, W> &&edge)
        noexcept(noexcept(
                std::is_nothrow_move_constructible<P>::value
                    && std::is_nothrow_move_assignable<P>::value))
    {
        ++moves;
        if (this != &edge) {
            idx1_ = edge.idx1_;
            idx2_ = edge.idx2_;
            std::swap(params_, edge.params_);
            std::swap(weight_, edge.weight_);
        }
        return *this;
    }

    vertex_index_t idx1() const { return idx1_; }
    vertex_index_t idx2() const { return idx2_; }
    P parameters() const { return params_; }
    const W &weight() const { return weight_; }

    void swap_vertices() { std::swap(idx1_, idx2_); }

/// counters to track down objects manipulations
public:
    static int default_creations;
    static int creations;
    static int copies;
    static int moves;
    static int assigns;

    static std::string stat()
    {
        return "creations: " + std::to_string(default_creations) + " + " + std::to_string(creations)
                + "; copies: " + std::to_string(copies)
                + "; moves: " + std::to_string(moves)
                + "; assigns: " + std::to_string(assigns);
    }

private:
    vertex_index_t idx1_;
    vertex_index_t idx2_;
    P params_;
    W weight_;
};

template<typename P, typename W> int Edge<P, W>::default_creations = 0;
template<typename P, typename W> int Edge<P, W>::creations = 0;
template<typename P, typename W> int Edge<P, W>::copies = 0;
template<typename P, typename W> int Edge<P, W>::moves = 0;
template<typename P, typename W> int Edge<P, W>::assigns = 0;

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
    explicit iterator(std::shared_ptr<IteratorImplBase<T>> b) : base_impl(b) {}

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

/**
 * Graph interface.
 *
 * @tparam Dir
 * @tparam V
 * @tparam E
 */
template<bool Dir, typename V, typename E, typename W>
class Graph {
protected:
    class EdgesWrapper {
    public:
        virtual iterator<Edge<E, W>> begin() = 0;
        virtual iterator<Edge<E, W>> end() = 0;
    };

public:
    // TODO add constructor from std::initializer_list
    virtual ~Graph() = default;

    virtual void add_vertex(Vertex<V> vertex) = 0;
    virtual void rm_vertex(vertex_index_t idx) = 0;
    // TODO measure performance
    virtual std::set<vertex_index_t> inbounds(vertex_index_t idx) const = 0;
    virtual std::set<vertex_index_t> outbounds(vertex_index_t idx, int mode) const = 0;

    virtual const Vertex<V> &vertex(vertex_index_t idx) const = 0;
    virtual size_t vertex_num() const = 0;

    virtual void add_edge(Edge<E, W> edge) = 0;
    virtual const Edge<E, W> &edge(vertex_index_t idx1, vertex_index_t idx2) const = 0;
    virtual bool edge_exists(Edge<E, W> edge) const = 0;
    virtual void rm_edge(Edge<E, W> edge) = 0;
    virtual size_t edge_num() const = 0;

    virtual bool filter_edge(Edge<E, W> edge) = 0;
    virtual bool filter_edges(const std::vector<Edge<E, W>> &edges) = 0;
    virtual bool restore_edge(Edge<E, W> edge) = 0;
    virtual bool restore_edges(const std::vector<Edge<E, W>> &edges) = 0;
    virtual void restore_edges() = 0;

    virtual EdgesWrapper &edges() = 0;
};

}  // namespace simple_graph
