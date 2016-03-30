#pragma once

#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include "graph.hpp"

namespace simple_graph {

template <bool Dir, typename V, typename E>
class ListGraph : public Graph<V, E> {
    typedef std::map<size_t, std::map<size_t, Edge<E>>> Edges;

private:
    class ListEdgesWrapper : public Graph<V, E>::EdgesWrapper {
    private:
        class EdgeIterator : public IteratorImplBase<Edge<E>> {
        public:
            EdgeIterator(Edges *edges, Edge<E> *current) : edges_(edges), current_(current) {}
            virtual bool operator==(const IteratorImplBase<Edge<E>> &it) const override {
                const EdgeIterator *tmp = dynamic_cast<const EdgeIterator*>(&it);
                return tmp && current_ == tmp->current_;
            }

            virtual bool operator!=(const IteratorImplBase<Edge<E>> &it) const override {
                return !this->operator==(it);
            }

            virtual IteratorImplBase<Edge<E>> &operator++() override {
                if (current_ != NULL) {
                    size_t idx1 = current_->idx1();
                    size_t idx2 = current_->idx2();
                    auto it = edges_->at(idx1).upper_bound(idx2);
                    if (it != edges_->at(idx1).end()) {
                        current_ = &it->second;
                    }
                    else {
                        auto it1 = edges_->upper_bound(idx1);
                        if (it1 != edges_->end()) {
                            current_ = &it1->second.begin()->second;
                        }
                        else {
                            current_ = NULL;
                        }
                    }
                }
                return *this;
            }

            virtual Edge<E> &operator*() override {
                return *current_;
            }

        private:
            Edges *edges_;
            Edge<E> *current_;
        };
    public:
        explicit ListEdgesWrapper(Edges *edges) : edges_(edges) {}

        virtual iterator<Edge<E>> begin() override {
            iterator<Edge<E>> iter(new EdgeIterator(edges_, &edges_->begin()->second.begin()->second));
            return iter;
        }

        virtual iterator<Edge<E>> end() override {
            iterator<Edge<E>> iter(new EdgeIterator(edges_, NULL));
            return iter;
        }

    private:
        Edges *edges_;
    };

public:
    ListGraph() : vertex_num_(0), vertices_(), neighbours_(), edges_(), edges_wrapper_(&edges_) {}
    virtual ~ListGraph() = default;

    virtual int add_vertex(const Vertex<V> &vertex) override {
        if (vertex.idx() < 0) {
            return -1;
        }
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
        }
        if (vertices_.count(vertex.idx()) != 0) {
            return -1;
        }
        vertices_.insert(std::make_pair(vertex.idx(), vertex));
        neighbours_[vertex.idx()] = std::set<size_t>();
        return 0;
    }

    virtual int add_vertex(Vertex<V> &&vertex) override {
        if (vertex.idx() < 0) {
            return -1;
        }
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
        }
        if (vertices_.count(vertex.idx()) != 0) {
            return -1;
        }
        neighbours_[vertex.idx()] = std::set<size_t>();
        vertices_.emplace(vertex.idx(), std::move(vertex));
        return 0;
    }


    virtual int set_vertex(const Vertex<V> &vertex) override {
        if (vertex.idx() >= vertex_num_) {
            vertex_num_ = vertex.idx() + 1;
        }
        vertices_[vertex.idx()] = vertex;
        return 0;
    }

    void rm_vertex(const Vertex<V> &vertex) override {
        if (vertices_.count(vertex.idx()) > 0) {
            vertices_.erase(vertex.idx());
        }
        for (auto v : neighbours_[vertex.idx()]) {
            rm_edge(Edge<E>(v, vertex.idx()));
        }
    }

    virtual const std::set<size_t> &adjacent_vertices(size_t idx) const override {
        return neighbours_.at(idx);
    }

    virtual const Vertex<V> &vertex(size_t idx) const override {
        return vertices_.at(idx);
    }

    virtual size_t vertex_num() const override { return vertex_num_; };

    virtual int add_edge(const Edge<E> &edge) override {
        if (std::max(edge.idx1(), edge.idx2()) >= vertex_num_) {
            return -1;
        }
        neighbours_[edge.idx1()].insert(edge.idx2());
        if (!Dir) {
            neighbours_[edge.idx2()].insert(edge.idx1());
        }

        if (Dir) {
            edges_[edge.idx1()][edge.idx2()] = edge;
        }
        // store undirected edge as min_idx->max_idx
        else {
            std::pair<int, int> p = std::minmax(edge.idx1(), edge.idx2());
            edges_[p.first][p.second] = edge;
        }

        return 0;
    }

    virtual int add_edge(Edge<E> &&edge) override {
        if (std::max(edge.idx1(), edge.idx2()) >= vertex_num_) {
            return -1;
        }
        neighbours_[edge.idx1()].insert(edge.idx2());
        if (!Dir) {
            neighbours_[edge.idx2()].insert(edge.idx1());
        }

        if (Dir) {
            edges_[edge.idx1()].emplace(edge.idx2(), std::move(edge));
        }
        // store undirected edge as min_idx->max_idx
        else {
            std::pair<int, int> p = std::minmax(edge.idx1(), edge.idx2());
            edges_[p.first].emplace(p.second, std::move(edge));
        }

        return 0;
    }

    virtual const Edge<E> &edge(size_t idx1, size_t idx2) const override
    {
        if (Dir) {
            return edges_.at(idx1).at(idx2);
        }
        else {
            auto p = std::minmax(idx1, idx2);
            return edges_.at(p.first).at(p.second);
        }
    }

    virtual int rm_edge(const Edge<E> &edge) override {
        if (std::max(edge.idx1(), edge.idx2()) >= vertex_num_) {
            return -1;
        }
        if (neighbours_.count(edge.idx1()) == 0) {
            return -1;
        }
        neighbours_[edge.idx1()].erase(edge.idx2());
        if (!Dir) {
            neighbours_[edge.idx2()].erase(edge.idx1());
        }

        if (Dir) {
            edges_[edge.idx1()].erase(edge.idx2());
            if (edges_[edge.idx1()].size() == 0) {
                edges_.erase(edge.idx1());
            }
        }
        else {
            auto p = std::minmax(edge.idx1(), edge.idx2());
            edges_[p.first].erase(p.second);
            if (edges_[p.first].size() == 0) {
                edges_.erase(p.first);
            }
        }
        return 0;
    }

    virtual typename Graph<V, E>::EdgesWrapper &edges() override {
        return edges_wrapper_;
    }

private:
    size_t vertex_num_;
    std::unordered_map<size_t, Vertex<V>> vertices_;
    std::unordered_map<size_t, std::set<size_t>> neighbours_;
    Edges edges_;
    ListEdgesWrapper edges_wrapper_;
};

}  // namespace simple_graph
