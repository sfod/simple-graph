#include <iostream>
#include "graph/graph.hpp"
#include "graph/list_graph.hpp"
#include "graph/matrix_graph.hpp"

int main()
{
    Graph<MatrixGraph, true, std::string> mg;
    mg.add_vertex(Vertex<std::string>(0, "zero"));
    mg.add_vertex(Vertex<std::string>(1, "one"));
    mg.add_vertex(Vertex<std::string>(2, "two"));
    mg.add_edge(0, 1);
    mg.add_edge(1, 2);
    std::vector<int> mpath;
    if (mg.find_path(0, 2, &mpath)) {
        for (auto n : mpath) {
            std::cout << mg.vertex(n).data() << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    Graph<ListGraph, false, std::string> lg;
    lg.add_vertex(Vertex<std::string>(0, "zero"));
    lg.add_vertex(Vertex<std::string>(1, "one"));
    lg.add_vertex(Vertex<std::string>(2, "two"));
    lg.add_edge(0, 1);
    lg.add_edge(1, 2);
    std::vector<int> lpath;
    if (lg.find_path(2, 1, &lpath)) {
        for (auto n : lpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    return 0;
}
