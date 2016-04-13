#include <iostream>
#include "benchmark/benchmark.h"
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/bellman_ford.hpp"

using simple_graph::vertex_index_t;

static void bench_bellman_ford(benchmark::State &state)
{
    simple_graph::ListGraph<true, int, ssize_t> g;
    for (int i = 0; i < state.range_x(); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range_x() - 1; ++i) {
        g.add_edge(simple_graph::Edge<ssize_t>(i, i + 1, 1));
    }

    while (state.KeepRunning()) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::bellman_ford(g, 0, state.range_x() - 1, &path));
    }
}
BENCHMARK(bench_bellman_ford)->Range(1<<10, 1<<16);

static void bench_bellman_ford_reversed(benchmark::State &state)
{
    simple_graph::ListGraph<true, int, ssize_t> g;
    for (int i = 0; i < state.range_x(); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range_x() - 1; ++i) {
        g.add_edge(simple_graph::Edge<ssize_t>(i + 1, i, 1));
    }

    while (state.KeepRunning()) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::bellman_ford(g, state.range_x() - 1, 0, &path));
    }
}
BENCHMARK(bench_bellman_ford_reversed)->Range(1<<10, 1<<16);

BENCHMARK_MAIN()
