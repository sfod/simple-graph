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
    ListGraph() : vertex_num_(0), vertices_(), inbounds_(), outbounds_(), filtered_edges_(), edges_(), edges_wrapper_(&edges_) {}
    virtual ~ListGraph() = default;

    void add_vertex(Vertex<V> vertex) override {
        if (vertex.idx() == static_cast<vertex_index_t >(-1)) {
            throw std::out_of_range("Vertex with invalid index");
        }

        if (vertices_.count(vertex.idx()) == 0) {
            ++vertex_num_;
        }
        vertices_.emplace(vertex.idx(), std::move(vertex));
        inbounds_[vertex.idx()] = std::set<vertex_index_t>();
        outbounds_[vertex.idx()] = std::set<vertex_index_t>();
    }

    void rm_vertex(vertex_index_t idx) override {
        if (vertices_.count(idx) == 0) {
            throw std::out_of_range("Vertex index is not presented");
        }

        // TODO use inbounds_ and outbounds_
        for (const auto &v : vertices_) {
            // remove edges 'idx -> some_vertex'
            if ((edges_.count(idx) > 0) && (edges_.at(idx).count(v.first) > 0)) {
                rm_edge(edges_.at(idx).at(v.first));
            }
            // remove edges 'some_vertex -> idx'
            if ((edges_.count(v.first) > 0) && (edges_.at(v.first).count(idx) > 0)) {
                rm_edge(edges_.at(v.first).at(idx));
            }
        }

        vertices_.erase(idx);
        assert(vertex_num_ > 0);
        --vertex_num_;
    }

    const std::set<vertex_index_t> &inbounds(vertex_index_t idx) const override {
        return inbounds_.at(idx);
    }

    const std::set<vertex_index_t> &outbounds(vertex_index_t idx) const override {
        return outbounds_.at(idx);
    }

    const Vertex<V> &vertex(vertex_index_t idx) const override {
        return vertices_.at(idx);
    }

    size_t vertex_num() const override { return vertex_num_; };

    void add_edge(Edge<E> edge) override {
        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
        }
        inbounds_[edge.idx2()].insert(edge.idx1());
        outbounds_[edge.idx1()].insert(edge.idx2());
        if (!Dir) {
            inbounds_[edge.idx1()].insert(edge.idx2());
            outbounds_[edge.idx2()].insert(edge.idx1());
        }

        // store undirected edge as min_idx->max_idx
        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }

        edges_[edge.idx1()].emplace(edge.idx2(), std::move(edge));
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

    bool edge_exists(Edge<E> edge) const override {
        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }
        return (edges_.count(edge.idx1()) > 0) && (edges_.at(edge.idx1()).count(edge.idx2()) > 0);
    }

    /**
     * @brief Remove specified edge from the graph
     * @param edge - edge to remove
     * @return
     */
    void rm_edge(Edge<E> edge) override {
        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
        }

        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }

        if ((edges_.count(edge.idx1()) == 0) || (edges_.at(edge.idx1()).count(edge.idx2()) == 0)) {
            return;
        }

        outbounds_[edge.idx1()].erase(edge.idx2());
        if (!Dir) {
            outbounds_[edge.idx2()].erase(edge.idx1());
        }

        inbounds_[edge.idx2()].erase(edge.idx1());
        if (!Dir) {
            inbounds_[edge.idx1()].erase(edge.idx2());
        }

        edges_[edge.idx1()].erase(edge.idx2());
        if (edges_[edge.idx1()].size() == 0) {
            edges_.erase(edge.idx1());
        }
    }

    /**
     * @brief Temporarily remove specified edge from the graph.
     * @param edge - edge to filter out
     * @return false if edge is invalid or is not present in the graph, true otherwise
     * @note If edge is valid, it is added to filtered edges regardless of it is present or not in the graph.
     */
    bool filter_edge(const Edge<E> &edge) override {
        bool rc = true;

        if (std::max(edge.idx1(), edge.idx2()) >= vertex_num_) {
            return false;
        }

        vertex_index_t idx1;
        vertex_index_t idx2;

        if (Dir) {
            idx1 = edge.idx1();
            idx2 = edge.idx2();
        }
        // edges are stored as min_idx->max_idx in undirected graph
        else {
            auto p = std::minmax(edge.idx1(), edge.idx2());
            idx1 = p.first;
            idx2 = p.second;
        }

        filtered_edges_[idx1].insert(idx2);

        // check if edge was actually filtered out
        if (!edges_.count(idx1) || !edges_.at(idx1).count(idx2)) {
            rc = false;
        }

        return rc;
    }

    /**
     * @brief Temporarily removes specified edges from the graph.
     * @param edges - edges to filter out
     * @return true if all edges filtered out, false otherwise
     */
    virtual bool filter_edges(const std::vector<Edge<E>> &edges) {
        bool rc = true;

        for (const auto &edge : edges) {
            if (!filter_edge(edge)) {
                rc = false;
            }
        }

        return rc;
    }

    typename Graph<Dir, V, E>::EdgesWrapper &edges() override {
        return edges_wrapper_;
    }

private:
    size_t vertex_num_;
    std::unordered_map<vertex_index_t, Vertex<V>> vertices_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> inbounds_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> outbounds_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> filtered_edges_;
    Edges edges_;
    ListEdgesWrapper edges_wrapper_;
};

}  // namespace simple_graph
