#include <iostream>
#include "graph/graph.hpp"
#include "graph/list_graph.hpp"
#include "graph/matrix_graph.hpp"

int main()
{
    Graph<MatrixGraph, true, int> mg;
    mg.add_edge(Node<int>(0), Node<int>(1));
    mg.add_edge(Node<int>(1), Node<int>(2));
    std::vector<int> mpath;
    if (mg.find_path(Node<int>(1), Node<int>(2), &mpath)) {
        for (auto n : mpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    Graph<ListGraph, false, int> lg;
    lg.add_edge(Node<int>(0), Node<int>(1));
    lg.add_edge(Node<int>(1), Node<int>(2));
    std::vector<int> lpath;
    if (lg.find_path(Node<int>(2), Node<int>(1), &lpath)) {
        for (auto n : lpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    return 0;
}
