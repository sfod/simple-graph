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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
