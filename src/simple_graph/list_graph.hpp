#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <map>
#include <unordered_map>  // TODO Replace with something really fast.
#include <set>
#include "graph.hpp"

namespace simple_graph {

template <bool Dir, typename V, typename E, typename W>
class ListGraph : public Graph<Dir, V, E, W> {
    using Edges = std::unordered_map<vertex_index_t, std::unordered_map<vertex_index_t, Edge<E, W>>>;
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
        class EdgeIterator : public IIterator<Edge<E, W>> {
            using Iterator1 = typename Edges::iterator;
            using Iterator2 = typename Edges::mapped_type::iterator;

        public:
            /**
             * Constructor.
             *
             * @param edges All edges in the graph.
             * @param filtered_edges All filtered edges in the graph.
             * @param is_end Flag indicating that iterator traversed all edges.
             */
            EdgeIterator(Edges *edges, FilteredEdges *filtered_edges, bool is_end)
                : edges_(edges), filtered_edges_(filtered_edges), is_end_(is_end)
            {
                if (is_end_) {
                    return;
                }

                // TODO Use const_iterator.
                it1_ = edges_->begin();
                if (it1_ == edges_->end()) {
                    is_end_ = true;
                }
                else {
                    it2_ = it1_->second.begin();
                    assert(it2_ != it1_->second.end());
                }

                // FIXME
                if (!is_end_ && (filtered_edges_->count(it2_->second.idx1()) > 0)
                        && (filtered_edges_->at(it2_->second.idx1()).count(it2_->second.idx2()) > 0)) {
                    this->operator++();
                }
            }

            /**
             * Check if two iterators are equal to each other.
             *
             * @param it Iterator to compare with.
             * @return True if iterators contain the same edge, false otherwise.
             */
            bool operator==(const IIterator<Edge<E, W>> &it) const override
            {
                const auto *tmp = dynamic_cast<const EdgeIterator*>(&it);
                return tmp && is_end_ == tmp->is_end_;
            }

            /**
             * Check if two iterators differ from each other.
             *
             * @param it Iterator to compare with.
             * @return True if iterators differ, false otherwise.
             */
            bool operator!=(const IIterator<Edge<E, W>> &it) const override
            {
                return !this->operator==(it);
            }

            /**
             * Increment iterator.
             *
             * @return Reference to iterator itself.
             */
            IIterator<Edge<E, W>> &operator++() override
            {
                if (is_end_) {
                    return *this;
                }

                /// Loop is needed to ignore filtered edges.
                while (!is_end_) {
                    ++it2_;

                    /// Traversed all edges for an index, go to next index.
                    if (it2_ == it1_->second.end()) {
                        ++it1_;
                        if (it1_ != edges_->end()) {
                            it2_ = it1_->second.begin();
                            assert(it2_ != it1_->second.end());
                        }
                        /// Traversed all edges.
                        else {
                            is_end_ = true;
                        }
                    }

                    /// Ignore filtered edges.
                    if (!is_end_ && (filtered_edges_->count(it2_->second.idx1()) > 0)
                            && (filtered_edges_->at(it2_->second.idx1()).count(it2_->second.idx2()) > 0)) {
                        continue;
                    }

                    return *this;
                }

                return *this;
            }

            /**
             * Get underlying edge.
             *
             * @return Current edge.
             */
            Edge<E, W> &operator*() override
            {
                return it2_->second;
            }

        private:
            Edges *edges_;
            FilteredEdges *filtered_edges_;
            bool is_end_;
            Iterator1 it1_;
            Iterator2 it2_;
        };

    public:
        explicit ListEdgesWrapper(Edges *edges, FilteredEdges *filtered_edges)
            : edges_(edges), filtered_edges_(filtered_edges) {}

        IteratorWrapper<Edge<E, W>> begin() override
        {
            IteratorWrapper<Edge<E, W>> iter(std::make_shared<EdgeIterator>(
                    EdgeIterator(edges_, filtered_edges_, false)));
            return iter;
        }

        IteratorWrapper<Edge<E, W>> end() override
        {
            IteratorWrapper<Edge<E, W>> iter(std::make_shared<EdgeIterator>(
                    EdgeIterator(edges_, filtered_edges_, true)));
            return iter;
        }

    private:
        Edges *edges_;
        FilteredEdges *filtered_edges_;
    };

public:
    ListGraph() : vertex_num_(0), vertices_(), inbounds_(), outbounds_(), edges_(), filtered_edges_(),
            edges_wrapper_(&edges_, &filtered_edges_) {}

    void add_vertex(Vertex<V> vertex) override
    {
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

    void rm_vertex(vertex_index_t idx) override
    {
        if (vertices_.count(idx) == 0) {
            // FIXME Exception is highly ineffective.
            throw std::out_of_range("Vertex index is not presented");
        }

        // TODO Use inbounds_ and outbounds_.
        for (const auto &v : vertices_) {
            /// Remove edges 'idx -> some_vertex'.
            if ((edges_.count(idx) > 0) && (edges_.at(idx).count(v.first) > 0)) {
                rm_edge(edges_.at(idx).at(v.first));
            }
            /// Remove edges 'some_vertex -> idx'.
            if ((edges_.count(v.first) > 0) && (edges_.at(v.first).count(idx) > 0)) {
                rm_edge(edges_.at(v.first).at(idx));
            }
        }

        vertices_.erase(idx);
        assert(vertex_num_ > 0);
        --vertex_num_;
    }

    std::set<vertex_index_t> inbounds(vertex_index_t idx) const override
    {
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

    std::set<vertex_index_t> outbounds(vertex_index_t idx, int mode) const override
    {
        if (outbounds_.count(idx) == 0) {
            return {};
        }

        /// No filters were applied, return all outbounds.
        if ((mode == 1) || (filtered_edges_.count(idx) == 0)) {
            return outbounds_.at(idx);
        }

        /// Return all outbounds except filtered ones.
        std::set<vertex_index_t> res;
        for (const auto &idx2 : outbounds_.at(idx)) {
            if (filtered_edges_.at(idx).count(idx2) == 0) {
                res.insert(idx2);
            }
        }

        return res;
    }

    const Vertex<V> &vertex(vertex_index_t idx) const override
    {
        return vertices_.at(idx);
    }

    size_t vertex_num() const override { return vertex_num_; };

    void add_edge(Edge<E, W> edge) override
    {
        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
        }
        inbounds_[edge.idx2()].insert(edge.idx1());
        outbounds_[edge.idx1()].insert(edge.idx2());
        if (!Dir) {
            inbounds_[edge.idx1()].insert(edge.idx2());
            outbounds_[edge.idx2()].insert(edge.idx1());
        }

        /// Store undirected edge as min_idx->max_idx.
        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }

        auto idx2 = edge.idx2(); /// Make idx2 copy as edge will be moved.
        edges_[edge.idx1()].emplace(idx2, std::move(edge));
    }

    const Edge<E, W> &edge(vertex_index_t idx1, vertex_index_t idx2) const override
    {
        if (Dir) {
            return edges_.at(idx1).at(idx2);
        }
        else {
            auto p = std::minmax(idx1, idx2);
            return edges_.at(p.first).at(p.second);
        }
    }

    bool edge_exists(Edge<E, W> edge) const override
    {
        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }
        return (edges_.count(edge.idx1()) > 0) && (edges_.at(edge.idx1()).count(edge.idx2()) > 0);
    }

    /**
     * @brief Remove specified edge from the graph
     * @param edge Edge to remove.
     */
    void rm_edge(Edge<E, W> edge) override
    {
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
    size_t edge_num() const
    {
        return edges_.size();
    }

    /**
     * @brief Temporarily remove specified edge from the graph.
     * @param edge Edge to filter out.
     * @return False if edge is invalid or is not present in the graph, true otherwise.
     * @note If edge is valid, it is added to filtered edges regardless of it's presence in the graph.
     */
    bool filter_edge(Edge<E, W> edge) override
    {
        bool rc = true;

        if ((vertices_.count(edge.idx1()) == 0) || (vertices_.count(edge.idx2()) == 0)) {
            throw std::out_of_range("Vertex index is not presented");
        }

        if (!Dir && (edge.idx1() > edge.idx2())) {
            edge.swap_vertices();
        }

        filtered_edges_[edge.idx1()].insert(edge.idx2());

        /// Check if edge was actually filtered out.
        if (!edges_.count(edge.idx1()) || !edges_.at(edge.idx1()).count(edge.idx2())) {
            rc = false;
        }

        return rc;
    }

    /**
     * @brief Temporarily remove specified edges from the graph.
     * @param edges Edges to filter out.
     * @return True if all edges filtered out, false otherwise.
     */
    bool filter_edges(const std::vector<Edge<E, W>> &edges) override
    {
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
    bool restore_edge(Edge<E, W> edge) override
    {
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
    bool restore_edges(const std::vector<Edge<E, W>> &edges) override
    {
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
    void restore_edges() override
    {
        filtered_edges_.clear();
    }

    /**
     *
     * @return
     */
    typename Graph<Dir, V, E, W>::EdgesWrapper &edges() override
    {
        return edges_wrapper_;
    }

private:
    vertex_index_t vertex_num_;
    std::unordered_map<vertex_index_t, Vertex<V>> vertices_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> inbounds_;
    std::unordered_map<vertex_index_t, std::set<vertex_index_t>> outbounds_;
    Edges edges_;
    FilteredEdges filtered_edges_;
    ListEdgesWrapper edges_wrapper_;
};

}  // namespace simple_graph
