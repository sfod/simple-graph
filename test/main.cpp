#include <iostream>
#include "graph/graph.hpp"
#include "graph/list_graph.hpp"
#include "graph/matrix_graph.hpp"

int main()
{
    Graph<MatrixGraph, true> mg;
    mg.add_edge(0, 1);
    mg.add_edge(1, 2);
    std::vector<int> mpath;
    if (mg.find_path(2, 1, &mpath)) {
        for (auto n : mpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    Graph<ListGraph, false> lg;
    lg.add_edge(0, 1);
    lg.add_edge(1, 2);
    std::vector<int> lpath;
    if (lg.find_path(1, 2, &lpath)) {
        for (auto n : lpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    return 0;
}