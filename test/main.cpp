#include <iostream>
#include "graph/graph.hpp"
#include "graph/list_graph.hpp"
#include "graph/matrix_graph.hpp"

int main()
{
    Graph<MatrixGraph, true, std::string> mg;
    mg.add_edge(Vertex<std::string>(0), Vertex<std::string>(1));
    mg.add_edge(Vertex<std::string>(1), Vertex<std::string>(2));
    std::vector<int> mpath;
    if (mg.find_path(Vertex<std::string>(1), Vertex<std::string>(2), &mpath)) {
        for (auto n : mpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    Graph<ListGraph, false, int> lg;
    lg.add_edge(Vertex<int>(0), Vertex<int>(1));
    lg.add_edge(Vertex<int>(1), Vertex<int>(2));
    std::vector<int> lpath;
    if (lg.find_path(Vertex<int>(2), Vertex<int>(1), &lpath)) {
        for (auto n : lpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    return 0;
}
