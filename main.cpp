#include <iostream>
#include "graph.hpp"

int main()
{
    Graph g(3);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    std::vector<int> path;
    if (g.find_path(1, 2, &path)) {
        for (auto n : path) {
            std::cout << n << std::endl;
        }
    }
    else {
        std::cout << "no path" << std::endl;
    }

    return 0;
}
