#include <gtest/gtest.h>
#include <graph/list_graph.hpp>

namespace {

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    ListGraph<false, int, int> g;
};

TEST_F(ListGraphUndirectedTest, test_astar)
{
    for (int i = 0; i < 8; ++i) {
        g.set_vertex(Vertex<int>(i, i));
    }

    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(Edge<int>(0, 1));
    g.add_edge(Edge<int>(1, 2));
    g.add_edge(Edge<int>(2, 3));
    g.add_edge(Edge<int>(3, 4));
    g.add_edge(Edge<int>(4, 5));
    g.add_edge(Edge<int>(5, 6));
    g.add_edge(Edge<int>(6, 7));
    g.add_edge(Edge<int>(3, 5));


    std::vector<int> path;
    std::function<float(int, int)> f = [&](int c, int r) { return 1; };
    EXPECT_EQ(true, g.astar(0, 7, f, &path));
    EXPECT_EQ(7, path.size());
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
