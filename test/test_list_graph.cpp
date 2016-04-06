#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/bfs.hpp"

namespace {

using simple_graph::vertex_index_t;

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<false, size_t, size_t> g;
};

TEST_F(ListGraphUndirectedTest, test_add_vertex)
{
    EXPECT_EQ(0, g.vertex_num());
    g.add_vertex(simple_graph::Vertex<size_t>(2));
    EXPECT_EQ(3, g.vertex_num());
    EXPECT_EQ(-1, g.add_vertex(simple_graph::Vertex<size_t>(2)));
}

TEST_F(ListGraphUndirectedTest, test_set_vertex)
{
    EXPECT_EQ(0, g.vertex_num());

    g.set_vertex(simple_graph::Vertex<size_t>(1, 4));
    EXPECT_EQ(2, g.vertex_num());
    EXPECT_EQ(4, g.vertex(1).data());

    g.set_vertex(simple_graph::Vertex<size_t>(1, 5));
    EXPECT_EQ(2, g.vertex_num());
    EXPECT_EQ(5, g.vertex(1).data());
}

TEST_F(ListGraphUndirectedTest, test_add_edge)
{
    for (size_t i = 0; i < 4; ++i) {
        g.set_vertex(simple_graph::Vertex<size_t>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<size_t>(0, 1, 11));
    g.add_edge(simple_graph::Edge<size_t>(1, 2, 12));
    g.add_edge(simple_graph::Edge<size_t>(2, 3, 13));

    EXPECT_EQ(12, g.edge(1, 2).weight());

    std::map<std::pair<vertex_index_t, vertex_index_t>, size_t> expected_edges = {
            {{0, 1}, 11},
            {{1, 2}, 12},
            {{2, 3}, 13},
            {{1, 0}, 11},
            {{2, 1}, 12},
            {{3, 2}, 13}
    };

    std::map<std::pair<vertex_index_t, vertex_index_t>, int> visited_edges = {
            {{0, 1}, 0},
            {{1, 2}, 0},
            {{2, 3}, 0},
            {{1, 0}, 0},
            {{2, 1}, 0},
            {{3, 2}, 0}
    };

    int i = 0;
    for (const auto &edge : g.edges()) {
        const auto &e = std::make_pair(edge.idx1(), edge.idx2());
        EXPECT_EQ(expected_edges[e], edge.weight());
        EXPECT_EQ(1, visited_edges.count(e));
        visited_edges[e]++;
        ++i;
    }

    for (const auto &e : visited_edges) {
        int visited_num = e.second;
        EXPECT_EQ(1, visited_num) << "edge " << e.first.first << ":" << e.first.second;
    }

    EXPECT_EQ(6, i);
}

TEST_F(ListGraphUndirectedTest, test_add_edge_descending)
{
    for (size_t i = 0; i < 4; ++i) {
        g.set_vertex(simple_graph::Vertex<size_t>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<size_t>(3, 2, 13));
    g.add_edge(simple_graph::Edge<size_t>(2, 1, 12));
    g.add_edge(simple_graph::Edge<size_t>(1, 0, 11));

    EXPECT_EQ(12, g.edge(1, 2).weight());

    std::map<std::pair<vertex_index_t, vertex_index_t>, size_t> expected_edges = {
            {{0, 1}, 11},
            {{1, 2}, 12},
            {{2, 3}, 13},
            {{1, 0}, 11},
            {{2, 1}, 12},
            {{3, 2}, 13}
    };

    std::map<std::pair<vertex_index_t, vertex_index_t>, int> visited_edges = {
            {{0, 1}, 0},
            {{1, 2}, 0},
            {{2, 3}, 0},
            {{1, 0}, 0},
            {{2, 1}, 0},
            {{3, 2}, 0}
    };

    int i = 0;
    for (const auto &edge : g.edges()) {
        const auto &e = std::make_pair(edge.idx1(), edge.idx2());
        EXPECT_EQ(expected_edges[e], edge.weight());
        EXPECT_EQ(1, visited_edges.count(e));
        visited_edges[e]++;
        ++i;
    }

    for (const auto &e : visited_edges) {
        int visited_num = e.second;
        EXPECT_EQ(1, visited_num) << "edge " << e.first.first << ":" << e.first.second;
    }

    EXPECT_EQ(6, i);
}

TEST_F(ListGraphUndirectedTest, test_bfs_direct_order)
{
    for (size_t i = 0; i < 4; ++i) {
        g.add_vertex(simple_graph::Vertex<size_t>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<size_t>(0, 1));
    g.add_edge(simple_graph::Edge<size_t>(1, 2));
    g.add_edge(simple_graph::Edge<size_t>(2, 3));

    std::vector<vertex_index_t> path;
    size_t p = 3;
    std::function<bool(size_t)> f = [&](size_t c) { return c == p; };
    EXPECT_EQ(true, bfs(g, 0, f, &path));
}

TEST_F(ListGraphUndirectedTest, test_bfs_reverse_order)
{
    for (size_t i = 0; i < 4; ++i) {
        g.add_vertex(simple_graph::Vertex<size_t>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<size_t>(0, 1));
    g.add_edge(simple_graph::Edge<size_t>(1, 2));
    g.add_edge(simple_graph::Edge<size_t>(2, 3));

    std::vector<vertex_index_t> path;
    size_t p = 0;
    std::function<bool(size_t)> f = [&](size_t c) { return c == p; };
    EXPECT_EQ(true, bfs(g, 3, f, &path));
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
