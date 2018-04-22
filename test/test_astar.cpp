#include <cmath>
#include <gtest/gtest.h>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/astar.hpp"

namespace {

using simple_graph::vertex_index_t;

class ListGraphTest : public ::testing::Test {
protected:
    simple_graph::ListGraph<false, std::pair<float, float>, int, float> undirected_graph;
    simple_graph::ListGraph<true, std::pair<float, float>, int, float> directed_graph;
};

static float dist(const simple_graph::ListGraph<false, std::pair<float, float>, int, float> &g,
        vertex_index_t c, vertex_index_t r)
{
    const auto &cdata = g.vertex(c).data();
    const auto &rdata = g.vertex(r).data();
    float xdiff = rdata.first - cdata.first;
    float ydiff = rdata.second - cdata.second;
    return std::sqrt(std::pow(xdiff, 2) + std::pow(ydiff, 2));
}

static float dist(const simple_graph::ListGraph<true, std::pair<float, float>, int, float> &g,
        vertex_index_t c, vertex_index_t r)
{
    const auto &cdata = g.vertex(c).data();
    const auto &rdata = g.vertex(r).data();
    float xdiff = rdata.first - cdata.first;
    float ydiff = rdata.second - cdata.second;
    return std::sqrt(std::pow(xdiff, 2) + std::pow(ydiff, 2));
}

TEST_F(ListGraphTest, test_undirected_astar_long)
{
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(4, std::make_pair(7.5f, 7.5f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(5, std::make_pair(3.0f, 1.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(6, std::make_pair(10.0f, 1.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(7, std::make_pair(10.0f, 10.0f)));

    EXPECT_EQ(8, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, float>(0, 1, 0, dist(undirected_graph, 0, 1)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(1, 2, 0, dist(undirected_graph, 1, 2)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(2, 3, 0, dist(undirected_graph, 2, 3)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(3, 4, 0, dist(undirected_graph, 3, 4)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(4, 7, 0, dist(undirected_graph, 4, 7)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(0, 5, 0, dist(undirected_graph, 0, 5)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(5, 6, 0, dist(undirected_graph, 5, 6)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(6, 7, 0, dist(undirected_graph, 6, 7)));

    std::vector<vertex_index_t> path;
    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return dist(undirected_graph, c, r);
    };
    EXPECT_EQ(true, astar(undirected_graph, 0, 7, heuristic, &path));
    EXPECT_EQ(6, path.size());
    EXPECT_EQ(0, path[0]);
    EXPECT_EQ(1, path[1]);
    EXPECT_EQ(4, path[4]);
}

TEST_F(ListGraphTest, test_undirected_astar_short)
{
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(4, std::make_pair(15.0f, 15.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(5, std::make_pair(3.0f, 1.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(6, std::make_pair(10.0f, 1.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(7, std::make_pair(10.0f, 10.0f)));

    EXPECT_EQ(8, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, float>(0, 1, 0, dist(undirected_graph, 0, 1)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(1, 2, 0, dist(undirected_graph, 1, 2)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(2, 3, 0, dist(undirected_graph, 2, 3)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(3, 4, 0, dist(undirected_graph, 3, 4)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(4, 7, 0, dist(undirected_graph, 4, 7)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(0, 5, 0, dist(undirected_graph, 0, 5)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(5, 6, 0, dist(undirected_graph, 5, 6)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(6, 7, 0, dist(undirected_graph, 6, 7)));

    std::vector<vertex_index_t> path;
    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return dist(undirected_graph, c, r);
    };
    EXPECT_EQ(true, astar(undirected_graph, 0, 7, heuristic, &path));
    EXPECT_EQ(4, path.size());
    EXPECT_EQ(5, path[1]);
    EXPECT_EQ(6, path[2]);
}

TEST_F(ListGraphTest, test_undirected_astar_no_path)
{
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    undirected_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));

    ASSERT_EQ(4, undirected_graph.vertex_num());

    undirected_graph.add_edge(simple_graph::Edge<int, float>(0, 1, 0, dist(undirected_graph, 0, 1)));
    undirected_graph.add_edge(simple_graph::Edge<int, float>(2, 3, 0, dist(undirected_graph, 2, 3)));

    std::vector<vertex_index_t> path;
    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return dist(undirected_graph, c, r);
    };
    EXPECT_EQ(false, astar(undirected_graph, 0, 3, heuristic, &path));
    EXPECT_EQ(0, path.size());
}

TEST_F(ListGraphTest, test_directed_astar_long)
{
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(0, std::make_pair(0.0f, 0.0f)));
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(1, std::make_pair(1.0f, 1.0f)));
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(2, std::make_pair(2.0f, 2.0f)));
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(3, std::make_pair(5.0f, 5.0f)));
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(4, std::make_pair(7.5f, 7.5f)));
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(5, std::make_pair(3.0f, 1.0f)));
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(6, std::make_pair(10.0f, 1.0f)));
    directed_graph.add_vertex(simple_graph::Vertex<std::pair<float, float>>(7, std::make_pair(10.0f, 10.0f)));

    EXPECT_EQ(8, directed_graph.vertex_num());

    directed_graph.add_edge(simple_graph::Edge<int, float>(0, 1, 0, dist(directed_graph, 0, 1)));
    directed_graph.add_edge(simple_graph::Edge<int, float>(1, 2, 0, dist(directed_graph, 1, 2)));
    directed_graph.add_edge(simple_graph::Edge<int, float>(2, 3, 0, dist(directed_graph, 2, 3)));
    directed_graph.add_edge(simple_graph::Edge<int, float>(3, 4, 0, dist(directed_graph, 3, 4)));
    directed_graph.add_edge(simple_graph::Edge<int, float>(4, 7, 0, dist(directed_graph, 4, 7)));
    directed_graph.add_edge(simple_graph::Edge<int, float>(0, 5, 0, dist(directed_graph, 0, 5)));
    directed_graph.add_edge(simple_graph::Edge<int, float>(5, 6, 0, dist(directed_graph, 5, 6)));
    directed_graph.add_edge(simple_graph::Edge<int, float>(6, 7, 0, dist(directed_graph, 6, 7)));

    std::vector<vertex_index_t> path;
    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return dist(directed_graph, c, r);
    };
    EXPECT_EQ(true, astar(directed_graph, 0, 7, heuristic, &path));
    EXPECT_EQ(6, path.size());
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
