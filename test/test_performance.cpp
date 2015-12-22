#include <random>
#include "benchmark/benchmark.h"
#include "simple_graph/list_graph.hpp"

static void BM_Construction(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, int> g;
    while (state.KeepRunning()) {
        state.PauseTiming();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, state.range_x() - 1);
        std::set<std::pair<int, int>> data;
        for (int j = 0; j < state.range_y(); ++j) {
            data.insert(std::make_pair<int, int>(dist(gen), dist(gen)));
        }
        state.ResumeTiming();

        for (int i = 0; i < state.range_x(); ++i) {
            g.add_vertex(simple_graph::Vertex<int>(i));
        }
        for (auto &d : data) {
            g.add_edge(simple_graph::Edge<int>(d.first, d.second));
        }
    }
}
BENCHMARK(BM_Construction)->RangePair(1<<10, 8<<16, 100, 1<<12);

BENCHMARK_MAIN()

