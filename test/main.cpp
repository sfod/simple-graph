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
    std::string ms = "two";
    std::function<bool(std::string)> mf = [&] (const std::string &d) { return d == ms; };
    if (mg.find_vertex(0, mf, &mpath)) {
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
    std::string ls = "two";
    std::function<bool(std::string)> lf = [&] (const std::string &d) { return d == ls; };
    if (lg.find_vertex(0, lf, &lpath)) {
        for (auto n : lpath) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    return 0;
}
