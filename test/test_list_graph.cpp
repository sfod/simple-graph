#include <gtest/gtest.h>
#include <graph/list_graph.hpp>

namespace {

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    ListGraph<false, int> g;
};

TEST_F(ListGraphUndirectedTest, test_add_vertex)
{
    EXPECT_EQ(0, g.vertex_num());
    g.add_vertex(Vertex<int>(2));
    EXPECT_EQ(3, g.vertex_num());
    EXPECT_EQ(-1, g.add_vertex(Vertex<int>(2)));
}

TEST_F(ListGraphUndirectedTest, test_set_vertex)
{
    EXPECT_EQ(0, g.vertex_num());

    g.set_vertex(Vertex<int>(1, 4));
    EXPECT_EQ(2, g.vertex_num());
    EXPECT_EQ(4, g.vertex(1).data());

    g.set_vertex(Vertex<int>(1, 5));
    EXPECT_EQ(2, g.vertex_num());
    EXPECT_EQ(5, g.vertex(1).data());
}

TEST_F(ListGraphUndirectedTest, test_bfs_direct_order)
{
    for (int i = 0; i < 4; ++i) {
        g.set_vertex(Vertex<int>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 3);

    std::vector<int> path;
    int p = 3;
    std::function<bool(int)> f = [&](int c) { return c == p; };
    EXPECT_EQ(true, g.bfs(0, f, &path));
}

TEST_F(ListGraphUndirectedTest, test_bfs_reverse_order)
{
    for (int i = 0; i < 4; ++i) {
        g.set_vertex(Vertex<int>(i, i));
    }

    EXPECT_EQ(4, g.vertex_num());

    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 3);

    std::vector<int> path;
    int p = 0;
    std::function<bool(int)> f = [&](int c) { return c == p; };
    EXPECT_EQ(true, g.bfs(3, f, &path));
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
