#include <iostream>
#include <vector>
#include "simple_graph/list_graph.hpp"

class EdgeProperties {
public:
    EdgeProperties() : data_(0)
    {
        ++default_creations;
    }

    EdgeProperties(int data) : data_(data)
    {
        ++creations;
    }

    EdgeProperties(EdgeProperties &&ep) noexcept(true)
    {
        ++moves;
        std::swap(data_, ep.data_);
    }

    EdgeProperties(const EdgeProperties &ep) : data_(ep.data_)
    {
        ++copies;
    }

    EdgeProperties &operator=(const EdgeProperties &ep)
    {
        ++assigns;
        if (this != &ep) {
            data_ = ep.data_;
        }

        return *this;
    }

    EdgeProperties &operator=(EdgeProperties &&ep) noexcept(true)
    {
        ++assign_moves;
        if (this != &ep) {
            std::swap(data_, ep.data_);
        }

        return *this;
    }

public:
    static int default_creations;
    static int creations;
    static int copies;
    static int moves;
    static int assign_moves;
    static int assigns;

    static std::string stat()
    {
        return "creations: " + std::to_string(default_creations) + " + " + std::to_string(creations)
                + "; copies: " + std::to_string(copies)
                + "; moves: " + std::to_string(moves)
                + "; assigns: " + std::to_string(assigns)
                + "; assign moves: " + std::to_string(assign_moves);
    }

private:
    float data_;
};

int EdgeProperties::default_creations = 0;
int EdgeProperties::creations = 0;
int EdgeProperties::copies = 0;
int EdgeProperties::moves = 0;
int EdgeProperties::assigns = 0;
int EdgeProperties::assign_moves = 0;


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
        simple_graph::ListGraph<false, int, EdgeProperties, int> g;
        for (int i = 0; i < X; ++i) {
            g.add_vertex(simple_graph::Vertex<int>(i));
        }
        for (auto &d : data) {
            g.add_edge(simple_graph::Edge<EdgeProperties, int>(d.first, d.second, 8));
        }
    }

    std::cout << "vertex stat: " << simple_graph::Vertex<int>::stat() << std::endl;
    std::cout << "edge stat: " << simple_graph::Edge<EdgeProperties, int>::stat() << std::endl;
    std::cout << "edge properties: " << EdgeProperties::stat() << std::endl;
}
