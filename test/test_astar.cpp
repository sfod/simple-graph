#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/astar.hpp"

namespace {

using simple_graph::vertex_index_t;

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<false, std::pair<float, float>, float> g;
};

static float dist(const simple_graph::ListGraph<false, std::pair<float, float>, float> &g,
        vertex_index_t c, vertex_index_t r)
{
    const auto &cdata = g.vertex(c).data();
    const auto &rdata = g.vertex(r).data();
    float xdiff = rdata.first - cdata.first;
    float ydiff = rdata.second - cdata.second;
    return std::sqrt(std::pow(xdiff, 2) + std::pow(ydiff, 2));
}

TEST_F(ListGraphUndirectedTest, test_astar_long)
{
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(4, std::make_pair(7.5f, 7.5f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(5, std::make_pair(3.0f, 1.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(6, std::make_pair(10.0f, 1.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(7, std::make_pair(10.0f, 10.0f)));

    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(simple_graph::Edge<float>(0, 1, dist(g, 0, 1)));
    g.add_edge(simple_graph::Edge<float>(1, 2, dist(g, 1, 2)));
    g.add_edge(simple_graph::Edge<float>(2, 3, dist(g, 2, 3)));
    g.add_edge(simple_graph::Edge<float>(3, 4, dist(g, 3, 4)));
    g.add_edge(simple_graph::Edge<float>(4, 7, dist(g, 4, 7)));
    g.add_edge(simple_graph::Edge<float>(0, 5, dist(g, 0, 5)));
    g.add_edge(simple_graph::Edge<float>(5, 6, dist(g, 5, 6)));
    g.add_edge(simple_graph::Edge<float>(6, 7, dist(g, 6, 7)));

    std::vector<vertex_index_t> path;
    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return dist(g, c, r);
    };
    EXPECT_EQ(true, astar(g, 0, 7, heuristic, &path));
    EXPECT_EQ(6, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[4]);
}

TEST_F(ListGraphUndirectedTest, test_astar_short)
{
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(4, std::make_pair(15.0f, 15.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(5, std::make_pair(3.0f, 1.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(6, std::make_pair(10.0f, 1.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(7, std::make_pair(10.0f, 10.0f)));

    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(simple_graph::Edge<float>(0, 1, dist(g, 0, 1)));
    g.add_edge(simple_graph::Edge<float>(1, 2, dist(g, 1, 2)));
    g.add_edge(simple_graph::Edge<float>(2, 3, dist(g, 2, 3)));
    g.add_edge(simple_graph::Edge<float>(3, 4, dist(g, 3, 4)));
    g.add_edge(simple_graph::Edge<float>(4, 7, dist(g, 4, 7)));
    g.add_edge(simple_graph::Edge<float>(0, 5, dist(g, 0, 5)));
    g.add_edge(simple_graph::Edge<float>(5, 6, dist(g, 5, 6)));
    g.add_edge(simple_graph::Edge<float>(6, 7, dist(g, 6, 7)));

    std::vector<vertex_index_t> path;
    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return dist(g, c, r);
    };
    EXPECT_EQ(true, astar(g, 0, 7, heuristic, &path));
    EXPECT_EQ(4, path.size());
    EXPECT_EQ(5, path[1]);
    EXPECT_EQ(6, path[2]);
}

TEST_F(ListGraphUndirectedTest, test_astar_no_path)
{
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    g.add_vertex(simple_graph::Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));

    ASSERT_EQ(4, g.vertex_num());

    g.add_edge(simple_graph::Edge<float>(0, 1, dist(g, 0, 1)));
    g.add_edge(simple_graph::Edge<float>(2, 3, dist(g, 2, 3)));

    std::vector<vertex_index_t> path;
    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return dist(g, c, r);
    };
    EXPECT_EQ(false, astar(g, 0, 3, heuristic, &path));
    EXPECT_EQ(0, path.size());
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
