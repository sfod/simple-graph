#include <cmath>
#include <iostream>
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/astar.hpp"
#include "simple_graph/algorithm/dijkstra.hpp"

using simple_graph::vertex_index_t;

int main()
{
    constexpr int size = 10;

    simple_graph::ListGraph<false, std::pair<int, int>, int, ssize_t> g;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            g.add_vertex(simple_graph::Vertex<std::pair<int, int>>(i * size + j, {i, j}));
        }
    }

    /// TODO Use random.
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size; ++j) {
            if (j != 0) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * size + j, i * size + j - 1, 0, 1));
            }
            if (j != size - 1) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * size + j, i * size + j + 1, 0, 1));
            }
            if (i != 0) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * size + j, (i - 1) * size + j, 0, 1));
            }
            if (i != size - 1) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * size + j, (i + 1) * size + j, 0, 1));
            }
        }
    }

    auto heuristic = [](vertex_index_t c, vertex_index_t r) {
        int ci = c / size;
        int cj = c % size;

        int ri = r / size;
        int rj = r % size;

        return std::sqrt(std::pow(ci - ri, 2) + std::pow(cj - rj, 2));
    };

    std::cout << "traversing..." << std::endl;

    uint64_t sum = 0;
    for (int i = 0; i < 1; ++i) {
        for (const auto &edge : g.edges()) {
            sum += edge.idx1();
        }
    }

    std::cout << sum << std::endl;

    std::vector<vertex_index_t> path;
    bool rc = simple_graph::astar(g, 0, size * size - 1, heuristic, &path);

    std::cout << rc << std::endl;
}
