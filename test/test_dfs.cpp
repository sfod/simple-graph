#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/dfs.hpp"

namespace {

using simple_graph::vertex_index_t;

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<false, size_t, size_t> undirected_graph;
};

TEST_F(ListGraphUndirectedTest, test_bfs_direct_order)
{
    for (size_t i = 0; i < 6; ++i) {
        undirected_graph.add_vertex(simple_graph::Vertex<size_t>(i, i));
    }
    ASSERT_EQ(6, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<size_t>(0, 1));
    undirected_graph.add_edge(simple_graph::Edge<size_t>(1, 2));
    undirected_graph.add_edge(simple_graph::Edge<size_t>(2, 3));
    undirected_graph.add_edge(simple_graph::Edge<size_t>(1, 4));
    undirected_graph.add_edge(simple_graph::Edge<size_t>(4, 5));

    std::vector<vertex_index_t> path;
    size_t p = 5;
    std::function<bool(size_t)> f = [&](size_t c) { return c == p; };
    ASSERT_EQ(true, dfs(undirected_graph, 0, f, &path));
    ASSERT_EQ(4, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[2]);
    EXPECT_EQ(5, path[3]);
}

TEST_F(ListGraphUndirectedTest, test_bfs_reverse_order)
{
    for (size_t i = 0; i < 4; ++i) {
        undirected_graph.add_vertex(simple_graph::Vertex<size_t>(i, i));
    }

    EXPECT_EQ(4, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<size_t>(0, 1));
    undirected_graph.add_edge(simple_graph::Edge<size_t>(1, 2));
    undirected_graph.add_edge(simple_graph::Edge<size_t>(2, 3));

    std::vector<vertex_index_t> path;
    size_t p = 0;
    std::function<bool(size_t)> f = [&](size_t c) { return c == p; };
    EXPECT_EQ(true, dfs(undirected_graph, 3, f, &path));
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
