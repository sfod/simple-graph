#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/bfs.hpp"

namespace {

using simple_graph::vertex_index_t;

class ListGraphDirectedTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        directed_graph.add_vertex(simple_graph::Vertex<char>(2, 'a'));
        directed_graph.add_vertex(simple_graph::Vertex<char>(4, 'b'));
        directed_graph.add_vertex(simple_graph::Vertex<char>(6, 'c'));
        directed_graph.add_vertex(simple_graph::Vertex<char>(23, 'd'));
    }

    simple_graph::ListGraph<true, char, int> directed_graph;
    simple_graph::ListGraph<true, char, int> directed_graph_empty;
};

TEST_F(ListGraphDirectedTest, test_get_vertex)
{
    ASSERT_EQ(4, directed_graph.vertex_num());

    EXPECT_EQ('a', directed_graph.vertex(2).data());
    EXPECT_EQ('b', directed_graph.vertex(4).data());
    EXPECT_EQ('c', directed_graph.vertex(6).data());
    EXPECT_EQ('d', directed_graph.vertex(23).data());

    EXPECT_THROW(directed_graph.vertex(3), std::out_of_range);

    ASSERT_EQ(0, directed_graph_empty.vertex_num());
    EXPECT_THROW(directed_graph_empty.vertex(0), std::out_of_range);
}

TEST_F(ListGraphDirectedTest, test_add_vertex)
{
    ASSERT_EQ(4, directed_graph.vertex_num());

    EXPECT_THROW(directed_graph.vertex(3), std::out_of_range);
    directed_graph.add_vertex(simple_graph::Vertex<char>(3, 'c'));
    ASSERT_EQ(5, directed_graph.vertex_num());
    EXPECT_EQ('c', directed_graph.vertex(3).data());

    EXPECT_THROW(directed_graph.add_vertex(simple_graph::Vertex<char>()), std::out_of_range);

    ASSERT_EQ(0, directed_graph_empty.vertex_num());
    EXPECT_THROW(directed_graph_empty.vertex(0), std::out_of_range);
    directed_graph_empty.add_vertex(simple_graph::Vertex<char>(3, 'c'));
    ASSERT_EQ(1, directed_graph_empty.vertex_num());
    EXPECT_THROW(directed_graph_empty.vertex(0), std::out_of_range);
    EXPECT_EQ('c', directed_graph_empty.vertex(3).data());
}

TEST_F(ListGraphDirectedTest, test_rm_vertex)
{
    directed_graph.add_edge(simple_graph::Edge<int>(2, 4, 11));
    directed_graph.add_edge(simple_graph::Edge<int>(4, 6, 12));
    directed_graph.add_edge(simple_graph::Edge<int>(6, 23, 13));

    ASSERT_EQ(4, directed_graph.vertex_num());

    EXPECT_THROW(directed_graph.rm_vertex(-1), std::out_of_range);
    EXPECT_THROW(directed_graph.rm_vertex(1), std::out_of_range);
    ASSERT_EQ(4, directed_graph.vertex_num());

    directed_graph.rm_vertex(2);
    ASSERT_EQ(3, directed_graph.vertex_num());

    int visited_edges_num = 0;
    for (const auto &edge : directed_graph.edges()) {
        (void) edge;
        ++visited_edges_num;
    }
    EXPECT_EQ(2, visited_edges_num);

    EXPECT_THROW(directed_graph.vertex(2), std::out_of_range);

    auto neighbours = directed_graph.adjacent_vertices(4);
    EXPECT_EQ(1, neighbours.size());
    directed_graph.rm_vertex(6);
    ASSERT_EQ(2, directed_graph.vertex_num());
    neighbours = directed_graph.adjacent_vertices(4);
    EXPECT_EQ(0, neighbours.size());

    ASSERT_EQ(0, directed_graph_empty.vertex_num());
    EXPECT_THROW(directed_graph_empty.rm_vertex(0), std::out_of_range);
    EXPECT_EQ(0, directed_graph_empty.vertex_num());
}

TEST_F(ListGraphDirectedTest, test_get_adjacent_vertices)
{
    ASSERT_EQ(4, directed_graph.vertex_num());
    EXPECT_THROW(directed_graph.adjacent_vertices(1), std::out_of_range);
    const auto &adj = directed_graph.adjacent_vertices(2);
    EXPECT_EQ(0, adj.size());

    ASSERT_EQ(0, directed_graph_empty.vertex_num());
    EXPECT_THROW(directed_graph_empty.adjacent_vertices(1), std::out_of_range);
}

TEST_F(ListGraphDirectedTest, test_add_edge)
{
    ASSERT_EQ(4, directed_graph.vertex_num());

    directed_graph.add_edge(simple_graph::Edge<int>(2, 4, 11));
    directed_graph.add_edge(simple_graph::Edge<int>(4, 6, 12));
    directed_graph.add_edge(simple_graph::Edge<int>(6, 23, 13));

    EXPECT_EQ(12, directed_graph.edge(4, 6).weight());

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

    int visited_edges_num = 0;
    for (const auto &edge : directed_graph.edges()) {
        const auto &e = std::make_pair(edge.idx1(), edge.idx2());
        EXPECT_EQ(expected_edges[e], edge.weight());
        EXPECT_EQ(1, visited_edges.count(e));
        visited_edges[e]++;
        ++visited_edges_num;
    }

    EXPECT_EQ(3, visited_edges_num);

    for (const auto &e : visited_edges) {
        int visited_num = e.second;
        EXPECT_EQ(1, visited_num) << "edge " << e.first.first << ":" << e.first.second;
    }
}

TEST_F(ListGraphDirectedTest, test_add_edge_descending)
{
    ASSERT_EQ(4, directed_graph.vertex_num());

    directed_graph.add_edge(simple_graph::Edge<int>(23, 6, 13));
    directed_graph.add_edge(simple_graph::Edge<int>(6, 4, 12));
    directed_graph.add_edge(simple_graph::Edge<int>(4, 2, 11));

    EXPECT_EQ(12, directed_graph.edge(6, 4).weight());

    std::map<std::pair<vertex_index_t, vertex_index_t>, size_t> expected_edges = {
            {{4, 2}, 11},
            {{6, 4}, 12},
            {{23, 6}, 13}
    };

    std::map<std::pair<vertex_index_t, vertex_index_t>, int> visited_edges = {
            {{4, 2}, 0},
            {{6, 4}, 0},
            {{23, 6}, 0}
    };

    int i = 0;
    for (const auto &edge : directed_graph.edges()) {
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

TEST_F(ListGraphDirectedTest, test_rm_edge)
{
    ASSERT_EQ(4, directed_graph.vertex_num());

    directed_graph.add_edge(simple_graph::Edge<int>(2, 4, 11));
    directed_graph.add_edge(simple_graph::Edge<int>(4, 6, 12));
    directed_graph.add_edge(simple_graph::Edge<int>(6, 23, 13));

    // adjacent vertices before any removes
    auto neighbours = directed_graph.adjacent_vertices(4);
    EXPECT_EQ(1, neighbours.size());

    neighbours = directed_graph.adjacent_vertices(6);
    EXPECT_EQ(1, neighbours.size());

    directed_graph.rm_edge(simple_graph::Edge<int>(4, 6));

    // adjacent vertices before after the first remove
    neighbours = directed_graph.adjacent_vertices(4);
    EXPECT_EQ(0, neighbours.size());

    neighbours = directed_graph.adjacent_vertices(6);
    EXPECT_EQ(1, neighbours.size());

    EXPECT_EQ(4, directed_graph.vertex_num());

    directed_graph.rm_edge(simple_graph::Edge<int>(6, 23));

    EXPECT_EQ(4, directed_graph.vertex_num());

    neighbours = directed_graph.adjacent_vertices(4);
    EXPECT_EQ(0, neighbours.size());

    neighbours = directed_graph.adjacent_vertices(6);
    EXPECT_EQ(0, neighbours.size());


    std::map<std::pair<vertex_index_t, vertex_index_t>, int> expected_edges = {
            {{2, 4}, 11}
    };
    std::map<std::pair<vertex_index_t, vertex_index_t>, int> visited_edges = {
            {{2, 4}, 0}
    };

    int visited_edges_num = 0;
    for (const auto &edge : directed_graph.edges()) {
        const auto &e = std::make_pair(edge.idx1(), edge.idx2());
        EXPECT_EQ(expected_edges[e], edge.weight());
        EXPECT_EQ(1, visited_edges.count(e));
        visited_edges[e]++;
        ++visited_edges_num;
    }

    EXPECT_EQ(1, visited_edges_num);

    for (const auto &e : visited_edges) {
        int visited_num = e.second;
        EXPECT_EQ(1, visited_num) << "edge " << e.first.first << ":" << e.first.second;
    }
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
