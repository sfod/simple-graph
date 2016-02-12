#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/bfs.hpp"

namespace {

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<false, int, int> g;
};

TEST_F(ListGraphUndirectedTest, test_add_vertex)
{
    EXPECT_EQ(0, g.vertex_num());
    g.add_vertex(simple_graph::Vertex<int>(2));
    EXPECT_EQ(3, g.vertex_num());
    EXPECT_EQ(-1, g.add_vertex(simple_graph::Vertex<int>(2)));
}

TEST_F(ListGraphUndirectedTest, test_set_vertex)
{
    EXPECT_EQ(0, g.vertex_num());

    g.set_vertex(simple_graph::Vertex<int>(1, 4));
    EXPECT_EQ(2, g.vertex_num());
    EXPECT_EQ(4, g.vertex(1).data());

    g.set_vertex(simple_graph::Vertex<int>(1, 5));
    EXPECT_EQ(2, g.vertex_num());
    EXPECT_EQ(5, g.vertex(1).data());
}

TEST_F(ListGraphUndirectedTest, test_add_edge)
{
    for (int i = 0; i < 4; ++i) {
        g.set_vertex(simple_graph::Vertex<int>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<int>(0, 1, 11));
    g.add_edge(simple_graph::Edge<int>(1, 2, 12));
    g.add_edge(simple_graph::Edge<int>(2, 3, 13));

    simple_graph::Edge<int> e = g.edge(1, 2);
    EXPECT_EQ(12, e.weight());

    int expected_edges[3][3] = {{0, 1, 11}, {1, 2, 12}, {2, 3, 13}};
    int i = 0;
    for (auto it = g.begin(); it != g.end(); ++it) {
        simple_graph::Edge<int> ep = *it;
        EXPECT_EQ(expected_edges[i][0], ep.idx1());
        EXPECT_EQ(expected_edges[i][1], ep.idx2());
        EXPECT_EQ(expected_edges[i][2], ep.weight());
        ++i;
    }
}

TEST_F(ListGraphUndirectedTest, test_bfs_direct_order)
{
    for (int i = 0; i < 4; ++i) {
        g.set_vertex(simple_graph::Vertex<int>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<int>(0, 1));
    g.add_edge(simple_graph::Edge<int>(1, 2));
    g.add_edge(simple_graph::Edge<int>(2, 3));

    std::vector<int> path;
    int p = 3;
    std::function<bool(int)> f = [&](int c) { return c == p; };
    EXPECT_EQ(true, bfs(g, 0, f, &path));
}

TEST_F(ListGraphUndirectedTest, test_bfs_reverse_order)
{
    for (int i = 0; i < 4; ++i) {
        g.set_vertex(simple_graph::Vertex<int>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<int>(0, 1));
    g.add_edge(simple_graph::Edge<int>(1, 2));
    g.add_edge(simple_graph::Edge<int>(2, 3));

    std::vector<int> path;
    int p = 0;
    std::function<bool(int)> f = [&](int c) { return c == p; };
    EXPECT_EQ(true, bfs(g, 3, f, &path));
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
