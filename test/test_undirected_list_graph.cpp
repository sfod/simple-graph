#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/bfs.hpp"

namespace {

using simple_graph::vertex_index_t;

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        undirected_graph.add_vertex(simple_graph::Vertex<char>(2, 'a'));
        undirected_graph.add_vertex(simple_graph::Vertex<char>(4, 'b'));
        undirected_graph.add_vertex(simple_graph::Vertex<char>(6, 'c'));
        undirected_graph.add_vertex(simple_graph::Vertex<char>(23, 'd'));
    }

    simple_graph::ListGraph<false, char, int> undirected_graph;
    simple_graph::ListGraph<false, char, int> undirected_graph_empty;
};

TEST_F(ListGraphUndirectedTest, test_get_vertex)
{
    ASSERT_EQ(4, undirected_graph.vertex_num());

    EXPECT_EQ('a', undirected_graph.vertex(2).data());
    EXPECT_EQ('b', undirected_graph.vertex(4).data());
    EXPECT_EQ('c', undirected_graph.vertex(6).data());
    EXPECT_EQ('d', undirected_graph.vertex(23).data());

    EXPECT_THROW(undirected_graph.vertex(3), std::out_of_range);

    ASSERT_EQ(0, undirected_graph_empty.vertex_num());
    EXPECT_THROW(undirected_graph_empty.vertex(0), std::out_of_range);
}

TEST_F(ListGraphUndirectedTest, test_add_vertex)
{
    ASSERT_EQ(4, undirected_graph.vertex_num());

    EXPECT_THROW(undirected_graph.vertex(3), std::out_of_range);
    undirected_graph.add_vertex(simple_graph::Vertex<char>(3, 'c'));
    ASSERT_EQ(5, undirected_graph.vertex_num());
    EXPECT_EQ('c', undirected_graph.vertex(3).data());

    EXPECT_THROW(undirected_graph.add_vertex(simple_graph::Vertex<char>()), std::out_of_range);

    ASSERT_EQ(0, undirected_graph_empty.vertex_num());
    EXPECT_THROW(undirected_graph_empty.vertex(0), std::out_of_range);
    undirected_graph_empty.add_vertex(simple_graph::Vertex<char>(3, 'c'));
    ASSERT_EQ(1, undirected_graph_empty.vertex_num());
    EXPECT_THROW(undirected_graph_empty.vertex(0), std::out_of_range);
    EXPECT_EQ('c', undirected_graph_empty.vertex(3).data());
}

TEST_F(ListGraphUndirectedTest, test_rm_vertex)
{
    ASSERT_EQ(4, undirected_graph.vertex_num());

    EXPECT_THROW(undirected_graph.rm_vertex(-1), std::out_of_range);
    EXPECT_THROW(undirected_graph.rm_vertex(1), std::out_of_range);
    ASSERT_EQ(4, undirected_graph.vertex_num());

    undirected_graph.rm_vertex(2);
    ASSERT_EQ(3, undirected_graph.vertex_num());
    EXPECT_THROW(undirected_graph.vertex(2), std::out_of_range);

    ASSERT_EQ(0, undirected_graph_empty.vertex_num());
    EXPECT_THROW(undirected_graph_empty.rm_vertex(0), std::out_of_range);
    EXPECT_EQ(0, undirected_graph_empty.vertex_num());
}

TEST_F(ListGraphUndirectedTest, test_get_outbounds)
{
    ASSERT_EQ(4, undirected_graph.vertex_num());
    const auto &out = undirected_graph.outbounds(1, 0);
    EXPECT_EQ(0, out.size());

    const auto &adj = undirected_graph.outbounds(2, 0);
    EXPECT_EQ(0, adj.size());
}

TEST_F(ListGraphUndirectedTest, test_add_edge)
{
    ASSERT_EQ(4, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int>(2, 4, 11));
    undirected_graph.add_edge(simple_graph::Edge<int>(4, 6, 12));
    undirected_graph.add_edge(simple_graph::Edge<int>(6, 23, 13));

    EXPECT_EQ(12, undirected_graph.edge(4, 6).weight());

    std::map<std::pair<vertex_index_t, vertex_index_t>, int> expected_edges = {
            {{2, 4}, 11},
            {{4, 6}, 12},
            {{6, 23}, 13}
    };
    std::map<std::pair<vertex_index_t, vertex_index_t>, int> visited_edges = {
            {{2, 4}, 0},
            {{4, 6}, 0},
            {{6, 23}, 0}
    };

    int i = 0;
    for (const auto &edge : undirected_graph.edges()) {
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
    EXPECT_EQ(3, i);
}

TEST_F(ListGraphUndirectedTest, test_add_edge_descending)
{
    ASSERT_EQ(4, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int>(23, 6, 13));
    undirected_graph.add_edge(simple_graph::Edge<int>(6, 4, 12));
    undirected_graph.add_edge(simple_graph::Edge<int>(4, 2, 11));

    EXPECT_EQ(12, undirected_graph.edge(4, 6).weight());

    std::map<std::pair<vertex_index_t, vertex_index_t>, size_t> expected_edges = {
            {{2, 4}, 11},
            {{4, 6}, 12},
            {{6, 23}, 13}
    };

    std::map<std::pair<vertex_index_t, vertex_index_t>, int> visited_edges = {
            {{2, 4}, 0},
            {{4, 6}, 0},
            {{6, 23}, 0}
    };

    int i = 0;
    for (const auto &edge : undirected_graph.edges()) {
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

    EXPECT_EQ(3, i);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
