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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
