#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/bellman_ford.hpp"

namespace {

using simple_graph::vertex_index_t;

class UndirectedListGraphTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<false, int, int, ssize_t> undirected_graph;
};

class DirectedListGraphTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<true, int, int, ssize_t> directed_graph;
};


TEST_F(UndirectedListGraphTest, test_bellman_ford_long)
{
    for (vertex_index_t i = 0; i < 8; ++i) {
        undirected_graph.add_vertex(simple_graph::Vertex<int>(i));
    }
    ASSERT_EQ(8, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 1, 0, 1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(1, 2, 0, 2));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 3, 0, 3));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(3, 4, 0, 4));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(4, 7, 0, 5));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 5, 0, 20));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(5, 6, 0, 20));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(6, 7, 0, 20));

    std::vector<vertex_index_t> path;
    ASSERT_EQ(true, simple_graph::bellman_ford(undirected_graph, 0, 7, &path));
    ASSERT_EQ(6, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[4]);
}

TEST_F(UndirectedListGraphTest, test_bellman_ford_short)
{
    for (size_t i = 0; i < 8; ++i) {
        undirected_graph.add_vertex(simple_graph::Vertex<int>(i));
    }
    ASSERT_EQ(8, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 1, 0, 1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(1, 2, 0, 2));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 3, 0, 3));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(3, 4, 0, 4));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(4, 7, 0, 5));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 5, 0, 1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(5, 6, 0, 1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(6, 7, 0, 1));

    std::vector<vertex_index_t> path;
    ASSERT_EQ(true, simple_graph::bellman_ford(undirected_graph, 0, 7, &path));
    ASSERT_EQ(4, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(5, path[1]);
    EXPECT_EQ(6, path[2]);
    EXPECT_EQ(7, path[3]);
}

TEST_F(UndirectedListGraphTest, test_bellman_ford_no_path)
{
    for (size_t i = 0; i < 4; ++i) {
        undirected_graph.add_vertex(simple_graph::Vertex<int>(i));
    }
    ASSERT_EQ(4, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 1, 0, 1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 3, 0, 3));

    std::vector<vertex_index_t> path;
    EXPECT_EQ(false, simple_graph::bellman_ford(undirected_graph, 0, 3, &path));
    EXPECT_EQ(0, path.size());
}

TEST_F(UndirectedListGraphTest, test_bellman_ford_negative_weigths)
{
    for (size_t i = 0; i < 8; ++i) {
        undirected_graph.add_vertex(simple_graph::Vertex<int>(i));
    }
    ASSERT_EQ(8, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 1, 0, -1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(1, 2, 0, -2));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 3, 0, -3));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(3, 4, 0, -4));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(4, 7, 0, -5));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 5, 0, -1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(5, 6, 0, -1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(6, 7, 0, -1));

    std::vector<vertex_index_t> path;
    EXPECT_EQ(false, simple_graph::bellman_ford(undirected_graph, 0, 7, &path));
    EXPECT_EQ(0, path.size());
}

TEST_F(UndirectedListGraphTest, test_bellman_ford_negative_cycles)
{
    for (size_t i = 0; i < 5; ++i) {
        undirected_graph.add_vertex(simple_graph::Vertex<int>(i));
    }
    ASSERT_EQ(5, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 1, 0, -1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(1, 2, 0, -1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 0, 0, -1));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 3, 0, 2));
    undirected_graph.add_edge(simple_graph::Edge<int, ssize_t>(3, 4, 0, 2));

    std::vector<vertex_index_t> path;
    EXPECT_EQ(false, simple_graph::bellman_ford(undirected_graph, 0, 4, &path));
    EXPECT_EQ(0, path.size());
}

TEST_F(DirectedListGraphTest, test_bellman_ford_long)
{
    for (vertex_index_t i = 0; i < 8; ++i) {
        directed_graph.add_vertex(simple_graph::Vertex<int>(i));
    }
    ASSERT_EQ(8, directed_graph.vertex_num());

    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 1, 0, 1));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(1, 2, 0, 2));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 3, 0, 3));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(3, 4, 0, 4));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(4, 7, 0, 5));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(0, 5, 0, 20));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(5, 6, 0, 20));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(6, 7, 0, 20));

    std::vector<vertex_index_t> path;
    ASSERT_EQ(true, simple_graph::bellman_ford(directed_graph, 0, 7, &path));
    ASSERT_EQ(6, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[4]);
}

TEST_F(DirectedListGraphTest, test_bellman_ford_reverse)
{
    for (vertex_index_t i = 0; i < 8; ++i) {
        directed_graph.add_vertex(simple_graph::Vertex<int>(i));
    }
    ASSERT_EQ(8, directed_graph.vertex_num());

    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(1, 0, 0, 1));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(2, 1, 0, 2));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(3, 2, 0, 3));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(4, 3, 0, 4));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(7, 4, 0, 5));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(5, 0, 0, 20));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(6, 5, 0, 20));
    directed_graph.add_edge(simple_graph::Edge<int, ssize_t>(7, 6, 0, 20));

    std::vector<vertex_index_t> path;
    ASSERT_EQ(true, simple_graph::bellman_ford(directed_graph, 7, 0, &path));
    ASSERT_EQ(6, path.size());
    EXPECT_EQ(7, path[0]);
    EXPECT_EQ(4, path[1]);
    EXPECT_EQ(1, path[4]);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
