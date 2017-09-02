#include <iostream>
#include <vector>
#include "simple_graph/list_graph.hpp"

int main()
{
    int X = 1<<16;
    int Y = 1<<12;
    int K = 1;

    int f = X / Y;

    std::vector<std::pair<int, int>> data;
    for (int j = 0; j < Y - 2; j += f) {
        data.push_back(std::make_pair<int, int>(j + 1, j + 1000));
    }

    for (int k = 0; k < K; ++k) {
        simple_graph::ListGraph<false, int, int> g;
        for (int i = 0; i < X; ++i) {
            g.add_vertex(simple_graph::Vertex<int>(i));
        }
        for (auto &d : data) {
            g.add_edge(simple_graph::Edge<int>(d.first, d.second));
        }
    }

    std::cout << "vertex stat: " << simple_graph::Vertex<int>::stat() << std::endl;
    std::cout << "edge stat: " << simple_graph::Edge<int>::stat() << std::endl;
}
