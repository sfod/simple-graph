#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <map>
#include <unordered_map>
#include <set>
#include "graph.hpp"

namespace simple_graph {

template <bool Dir, typename V, typename E, typename W>
class ListGraph : public Graph<Dir, V, E, W> {
    using Edges = std::map<vertex_index_t, std::map<vertex_index_t, Edge<E, W>>>;
    using FilteredEdges = std::unordered_map<vertex_index_t, std::set<vertex_index_t>>;

private:
    /**
     * Service class to implement iterator.
     */
    class ListEdgesWrapper : public Graph<Dir, V, E, W>::EdgesWrapper {
    private:
        /**
         * Iterator implementation for edges.
         */
        class EdgeIterator : public IteratorImplBase<Edge<E, W>> {
        public:
            /**
             * Constructor.
             *
             * @param edges Set of all edges in the graph.
             * @param filtered_edges Set of filtered edges in the graph.
             * @param current Edge to start iteration with.
             */
            EdgeIterator(Edges *edges, FilteredEdges *filtered_edges, Edge<E, W> *current)
                : edges_(edges), filtered_edges_(filtered_edges), current_(current)
            {
            }

            /**
             * Check if two iterators are equal to each other.
             *
             * @param it Iterator to compare with.
             * @return True if iterators contain the same edge, false otherwise.
             */
            bool operator==(const IteratorImplBase<Edge<E, W>> &it) const override {
                const auto *tmp = dynamic_cast<const EdgeIterator*>(&it);
                return tmp && current_ == tmp->current_;
            }

            /**
             * Check if two iterators differs from each other.
             *
             * @param it Iterator to compare with.
             * @return True if iterators differ, false otherwise.
             */
            bool operator!=(const IteratorImplBase<Edge<E, W>> &it) const override {
                return !this->operator==(it);
            }

            /**
             * Increment iterator.
             *
             * @return Reference to iterator itself.
             */
            IteratorImplBase<Edge<E, W>> &operator++() override {
                while (true) {
                    if (current_ != nullptr) {
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
                            // traversed all edges
                            else {
                                current_ = nullptr;
                            }
                        }
                    }

                    // skip filtered edges
                    if ((current_ == nullptr) || (filtered_edges_->count(current_->idx1()) == 0)
                            || (filtered_edges_->at(current_->idx1()).count(current_->idx2()) == 0)) {
                        break;
                    }
                }

                return *this;
            }

            /**
             * Get underlying edge.
             *
             * @return Current edge.
             */
            Edge<E, W> &operator*() override {
                return *current_;
            }

        private:
            Edges *edges_;
            FilteredEdges *filtered_edges_;
            Edge<E, W> *current_;
        };

    public:
        explicit ListEdgesWrapper(Edges *edges, FilteredEdges *filtered_edges)
            : edges_(edges), filtered_edges_(filtered_edges) {}

        iterator<Edge<E, W>> begin() override {
            iterator<Edge<E, W>> iter(std::make_shared<EdgeIterator>(EdgeIterator(edges_, filtered_edges_, &edges_->begin()->second.begin()->second)));
            return iter;
        }

        iterator<Edge<E, W>> end() override {
            iterator<Edge<E, W>> iter(std::make_shared<EdgeIterator>(EdgeIterator(edges_, filtered_edges_, NULL)));
            return iter;
        }

    private:
        Edges *edges_;
        FilteredEdges *filtered_edges_;
    };

public:
    ListGraph() : vertex_num_(0), vertices_(), inbounds_(), outbounds_(), edges_(), filtered_edges_(),
            edges_wrapper_(&edges_, &filtered_edges_) {}

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

    std::set<vertex_index_t> inbounds(vertex_index_t idx) const override {
        if (inbounds_.count(idx) == 0) {
            return {};
        }

        std::set<vertex_index_t> res;
        for (const auto &idx2 : inbounds_.at(idx)) {
            if ((filtered_edges_.count(idx2) == 0) || (filtered_edges_.at(idx2).count(idx) == 0)) {
                res.insert(idx2);
            }
        }

        return res;
    }

    std::set<vertex_index_t> outbounds(vertex_index_t idx, int mode) const override {
        if (outbounds_.count(idx) == 0) {
            return {};
        }

        // no filters were applied, return all outbounds
        if ((mode == 1) || (filtered_edges_.count(idx) == 0)) {
            return outbounds_.at(idx);
        }

        // return all outbounds except filtered ones
        std::set<vertex_index_t> res;
        for (const auto &idx2 : outbounds_.at(idx)) {
            if (filtered_edges_.at(idx).count(idx2) == 0) {
                res.insert(idx2);
            }
        }

        return res;
    }

    const Vertex<V> &vertex(vertex_index_t idx) const override {
        return vertices_.at(idx);
    }

    size_t vertex_num() const override { return vertex_num_; };

    void add_edge(Edge<E, W> edge) override {
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

    const Edge<E, W> &edge(vertex_index_t idx1, vertex_index_t idx2) const override {
        if (Dir) {
            return edges_.at(idx1).at(idx2);
        }
        else {
            auto p = std::minmax(idx1, idx2);
            return edges_.at(p.first).at(p.second);
        }
    }

    bool edge_exists(Edge<E, W> edge) const override {
        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }
        return (edges_.count(edge.idx1()) > 0) && (edges_.at(edge.idx1()).count(edge.idx2()) > 0);
    }

    /**
     * @brief Remove specified edge from the graph
     * @param edge Edge to remove.
     */
    void rm_edge(Edge<E, W> edge) override {
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
     * Get number of edges in the graph.
     *
     * @return Number of edges.
     * @note Filtered edges are not taken into consideration.
     */
    size_t edge_num() const {
        return edges_.size();
    }

    /**
     * @brief Temporarily remove specified edge from the graph.
     * @param edge Edge to filter out.
     * @return False if edge is invalid or is not present in the graph, true otherwise.
     * @note If edge is valid, it is added to filtered edges regardless of it's presence in the graph.
     */
    bool filter_edge(Edge<E, W> edge) override {
        bool rc = true;

        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
        }

        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }

        filtered_edges_[edge.idx1()].insert(edge.idx2());

        // check if edge was actually filtered out
        if (!edges_.count(edge.idx1()) || !edges_.at(edge.idx1()).count(edge.idx2())) {
            rc = false;
        }

        return rc;
    }

    /**
     * @brief Temporarily removes specified edges from the graph.
     * @param edges Edges to filter out.
     * @return True if all edges filtered out, false otherwise.
     */
    bool filter_edges(const std::vector<Edge<E, W>> &edges) override {
        bool rc = true;

        for (const auto &edge : edges) {
            if (!filter_edge(edge)) {
                rc = false;
            }
        }

        return rc;
    }

    /**
     * @brief Restore temporarily removed edge.
     * @param edge Edge to restore.
     */
    bool restore_edge(Edge<E, W> edge) override {
        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
        }

        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }

        if ((filtered_edges_.count(edge.idx1()) > 0) && (filtered_edges_.at(edge.idx1()).count(edge.idx2()) > 0)) {
            filtered_edges_[edge.idx1()].erase(edge.idx2());
            if (filtered_edges_.at(edge.idx1()).size() == 0) {
                filtered_edges_.erase(edge.idx1());
            }
            return true;
        }

        return false;
    }

    /**
     * @brief Restore temporarily removed edges.
     * @param edges
     */
    bool restore_edges(const std::vector<Edge<E, W>> &edges) override {
        bool rc = true;

        for (const auto &edge : edges) {
            if (!restore_edge(edge)) {
                rc = false;
            }
        }

        return rc;
    }

    /**
     * @brief Restore all temporarily removed edges.
     * @param edges
     */
    void restore_edges() override {
        filtered_edges_.clear();
    }

    /**
     *
     * @return
     */
    typename Graph<Dir, V, E, W>::EdgesWrapper &edges() override {
        return edges_wrapper_;
    }

private:
    size_t vertex_num_;
    std::unordered_map<vertex_index_t, Vertex<V>> vertices_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> inbounds_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> outbounds_;
    Edges edges_;
    FilteredEdges filtered_edges_;
    ListEdgesWrapper edges_wrapper_;
};

}  // namespace simple_graph
