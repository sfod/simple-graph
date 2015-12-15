#include <iostream>
#include "graph/graph.hpp"
#include "graph/list_graph.hpp"

int main()
{
    Graph<ListGraph, false, std::string, int> lg;
    lg.add_vertex(Vertex<std::string>(0, "zero"));
    lg.add_vertex(Vertex<std::string>(1, "one"));
    lg.add_vertex(Vertex<std::string>(2, "two"));
    lg.add_edge(Edge<int>(0, 1));
    lg.add_edge(Edge<int>(1, 2));
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
