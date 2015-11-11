#include <gtest/gtest.h>
#include <graph/list_graph.hpp>

namespace {

class ListGraphTest : public ::testing::Test {
protected:
    ListGraphTest() {}
    virtual ~ListGraphTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

}

TEST_F(ListGraphTest, add_vertex)
{
    ListGraph<false, int> g;
    EXPECT_EQ(0, g.vertex_num());

    g.add_vertex(Vertex<int>(2));
    EXPECT_EQ(3, g.vertex_num());

    EXPECT_EQ(-1, g.add_vertex(Vertex<int>(2)));
}

TEST_F(ListGraphTest, set_vertex)
{
    ListGraph<false, int> g;
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
