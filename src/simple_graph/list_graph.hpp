#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <map>
#include <unordered_map>
#include <set>
#include "graph.hpp"

namespace simple_graph {

template <bool Dir, typename V, typename E>
class ListGraph : public Graph<Dir, V, E> {
    typedef std::map<vertex_index_t, std::map<vertex_index_t, Edge<E>>> Edges;

private:
    class ListEdgesWrapper : public Graph<Dir, V, E>::EdgesWrapper {
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
                    vertex_index_t idx1;
                    vertex_index_t idx2;

                    if (Dir) {
                        idx1 = current_->idx1();
                        idx2 = current_->idx2();
                    }
                    else {
                        auto p = std::minmax(current_->idx1(), current_->idx2());
                        idx1 = p.first;
                        idx2 = p.second;
                    }

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
            iterator<Edge<E>> iter(std::make_shared<EdgeIterator>(EdgeIterator(edges_, &edges_->begin()->second.begin()->second)));
            return iter;
        }

        virtual iterator<Edge<E>> end() override {
            iterator<Edge<E>> iter(std::make_shared<EdgeIterator>(EdgeIterator(edges_, NULL)));
            return iter;
        }

    private:
        Edges *edges_;
    };

public:
    ListGraph() : vertex_num_(0), vertices_(), neighbours_(), edges_(), edges_wrapper_(&edges_) {}
    virtual ~ListGraph() = default;

    virtual void add_vertex(Vertex<V> vertex) override {
        if (vertex.idx() < 0) {
            throw std::out_of_range("Negative vertex index");
        }
        if (vertices_.count(vertex.idx()) == 0) {
            ++vertex_num_;
        }
        vertices_.emplace(vertex.idx(), std::move(vertex));
        neighbours_[vertex.idx()] = std::set<vertex_index_t>();
    }

    void rm_vertex(vertex_index_t idx) override {
        if (idx < 0) {
            throw std::out_of_range("Negative vertex index");
        }
        if (vertices_.count(idx) == 0) {
            throw std::out_of_range("Vertex index is not presented");
        }
        for (auto v : neighbours_[idx]) {
            rm_edge(Edge<E>(v, idx));
            // if graph is directed we must specify exact order of vertices in the edge to delete it
            if (Dir) {
                rm_edge(Edge<E>(idx, v));
            }
        }
        vertices_.erase(idx);
        assert(vertex_num_ > 0);
        --vertex_num_;
    }

    const std::set<vertex_index_t> &adjacent_vertices(vertex_index_t idx) const override {
        return neighbours_.at(idx);
    }

    const Vertex<V> &vertex(vertex_index_t idx) const override {
        return vertices_.at(idx);
    }

    virtual size_t vertex_num() const override { return vertex_num_; };

    virtual void add_edge(Edge<E> edge) override {
        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
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
            auto p = std::minmax(edge.idx1(), edge.idx2());
            edges_[p.first].emplace(p.second, std::move(edge));
        }
    }

    const Edge<E> &edge(vertex_index_t idx1, vertex_index_t idx2) const override {
        if (Dir) {
            return edges_.at(idx1).at(idx2);
        }
        else {
            auto p = std::minmax(idx1, idx2);
            return edges_.at(p.first).at(p.second);
        }
    }

    /**
     * @brief Remove specified edge from the graph
     * @param edge - edge to remove
     * @return
     */
    virtual void rm_edge(const Edge<E> &edge) override {
        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
        }
        if (neighbours_.count(edge.idx1()) == 0) {
            throw std::out_of_range("Vertex index is not presented");
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
    }

    typename Graph<Dir, V, E>::EdgesWrapper &edges() override {
        return edges_wrapper_;
    }

private:
    size_t vertex_num_;
    std::unordered_map<vertex_index_t, Vertex<V>> vertices_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> neighbours_;
    Edges edges_;
    ListEdgesWrapper edges_wrapper_;
};

}  // namespace simple_graph
