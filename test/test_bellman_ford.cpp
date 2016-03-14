#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/bellman_ford.hpp"

namespace {

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<false, int, ssize_t> g;
};

TEST_F(ListGraphUndirectedTest, test_bellman_ford_long)
{
    for (size_t i = 0; i < 8; ++i) {
        g.set_vertex(simple_graph::Vertex<int>(i));
    }
    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(simple_graph::Edge<ssize_t>(0, 1, 1));
    g.add_edge(simple_graph::Edge<ssize_t>(1, 2, 2));
    g.add_edge(simple_graph::Edge<ssize_t>(2, 3, 3));
    g.add_edge(simple_graph::Edge<ssize_t>(3, 4, 4));
    g.add_edge(simple_graph::Edge<ssize_t>(4, 7, 5));
    g.add_edge(simple_graph::Edge<ssize_t>(0, 5, 20));
    g.add_edge(simple_graph::Edge<ssize_t>(5, 6, 20));
    g.add_edge(simple_graph::Edge<ssize_t>(6, 7, 20));

    std::vector<size_t> path;
    EXPECT_EQ(true, simple_graph::bellman_ford(g, 0, 7, &path));
    EXPECT_EQ(6, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[4]);
}

TEST_F(ListGraphUndirectedTest, test_bellman_ford_short)
{
    for (size_t i = 0; i < 8; ++i) {
        g.set_vertex(simple_graph::Vertex<int>(i));
    }
    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(simple_graph::Edge<ssize_t>(0, 1, 1));
    g.add_edge(simple_graph::Edge<ssize_t>(1, 2, 2));
    g.add_edge(simple_graph::Edge<ssize_t>(2, 3, 3));
    g.add_edge(simple_graph::Edge<ssize_t>(3, 4, 4));
    g.add_edge(simple_graph::Edge<ssize_t>(4, 7, 5));
    g.add_edge(simple_graph::Edge<ssize_t>(0, 5, 1));
    g.add_edge(simple_graph::Edge<ssize_t>(5, 6, 1));
    g.add_edge(simple_graph::Edge<ssize_t>(6, 7, 1));

    std::vector<size_t> path;
    EXPECT_EQ(true, simple_graph::bellman_ford(g, 0, 7, &path));
    EXPECT_EQ(4, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(5, path[1]);
    EXPECT_EQ(6, path[2]);
    EXPECT_EQ(7, path[3]);
}

TEST_F(ListGraphUndirectedTest, test_bellman_ford_negative_weigths)
{
    for (size_t i = 0; i < 8; ++i) {
        g.set_vertex(simple_graph::Vertex<int>(i));
    }
    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(simple_graph::Edge<ssize_t>(0, 1, -1));
    g.add_edge(simple_graph::Edge<ssize_t>(1, 2, -2));
    g.add_edge(simple_graph::Edge<ssize_t>(2, 3, -3));
    g.add_edge(simple_graph::Edge<ssize_t>(3, 4, -4));
    g.add_edge(simple_graph::Edge<ssize_t>(4, 7, -5));
    g.add_edge(simple_graph::Edge<ssize_t>(0, 5, -1));
    g.add_edge(simple_graph::Edge<ssize_t>(5, 6, -1));
    g.add_edge(simple_graph::Edge<ssize_t>(6, 7, -1));

    std::vector<size_t> path;
    ASSERT_EQ(true, simple_graph::bellman_ford(g, 0, 7, &path));
    ASSERT_EQ(6, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[4]);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
