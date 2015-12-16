#include <gtest/gtest.h>
#include <graph/list_graph.hpp>

namespace {

class ListGraphUndirectedTest : public ::testing::Test {
protected:
    ListGraph<false, std::pair<float, float>, float> g;
};

static float dist(const ListGraph<false, std::pair<float, float>, float> &g, int c, int r)
{
    const auto &cdata = g.vertex(c).data();
    const auto &rdata = g.vertex(r).data();
    float xdiff = rdata.first - cdata.first;
    float ydiff = rdata.second - cdata.second;
    return std::sqrt(std::pow(xdiff, 2) + std::pow(ydiff, 2));
}

TEST_F(ListGraphUndirectedTest, test_astar)
{
    g.set_vertex(Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(4, std::make_pair(7.5f, 7.5f)));
    g.set_vertex(Vertex<std::pair<float, float>>(5, std::make_pair(3.0f, 1.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(6, std::make_pair(10.0f, 1.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(7, std::make_pair(10.0f, 10.0f)));

    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(Edge<float>(0, 1, dist(g, 0, 1)));
    g.add_edge(Edge<float>(1, 2, dist(g, 1, 2)));
    g.add_edge(Edge<float>(2, 3, dist(g, 2, 3)));
    g.add_edge(Edge<float>(3, 4, dist(g, 3, 4)));
    g.add_edge(Edge<float>(4, 7, dist(g, 4, 7)));
    g.add_edge(Edge<float>(0, 5, dist(g, 0, 5)));
    g.add_edge(Edge<float>(5, 6, dist(g, 5, 6)));
    g.add_edge(Edge<float>(6, 7, dist(g, 6, 7)));

    std::vector<int> path;
    std::function<float(int, int)> heuristic = [&](int c, int r) {
        return dist(g, c, r);
    };
    EXPECT_EQ(true, g.astar(0, 7, heuristic, &path));
    EXPECT_EQ(6, path.size());
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[4]);
}

TEST_F(ListGraphUndirectedTest, test_astar_neg)
{
    g.set_vertex(Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(4, std::make_pair(15.0f, 15.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(5, std::make_pair(3.0f, 1.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(6, std::make_pair(10.0f, 1.0f)));
    g.set_vertex(Vertex<std::pair<float, float>>(7, std::make_pair(10.0f, 10.0f)));

    EXPECT_EQ(8, g.vertex_num());

    g.add_edge(Edge<float>(0, 1, dist(g, 0, 1)));
    g.add_edge(Edge<float>(1, 2, dist(g, 1, 2)));
    g.add_edge(Edge<float>(2, 3, dist(g, 2, 3)));
    g.add_edge(Edge<float>(3, 4, dist(g, 3, 4)));
    g.add_edge(Edge<float>(4, 7, dist(g, 4, 7)));
    g.add_edge(Edge<float>(0, 5, dist(g, 0, 5)));
    g.add_edge(Edge<float>(5, 6, dist(g, 5, 6)));
    g.add_edge(Edge<float>(6, 7, dist(g, 6, 7)));

    std::vector<int> path;
    std::function<float(int, int)> heuristic = [&](int c, int r) {
        return dist(g, c, r);
    };
    EXPECT_EQ(true, g.astar(0, 7, heuristic, &path));
    EXPECT_EQ(4, path.size());
    EXPECT_EQ(5, path[1]);
    EXPECT_EQ(6, path[2]);
}

}  // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
