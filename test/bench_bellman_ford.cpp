#include <algorithm>
#include <iostream>
#include "benchmark/benchmark.h"
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/bellman_ford.hpp"

using simple_graph::vertex_index_t;

static void bench_bellman_ford(benchmark::State &state)
{
    simple_graph::ListGraph<true, int, int, ssize_t> g;
    for (int i = 0; i < state.range_x(); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range_x() - 1; ++i) {
        g.add_edge(simple_graph::Edge<int, ssize_t>(i, i + 1, 0, 1));
    }

    while (state.KeepRunning()) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::bellman_ford(g, 0, state.range_x() - 1, &path));
    }
}
BENCHMARK(bench_bellman_ford)->Range(1<<10, 1<<16);

static void bench_bellman_ford_reversed(benchmark::State &state)
{
    simple_graph::ListGraph<true, int, int, ssize_t> g;
    for (int i = 0; i < state.range_x(); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range_x() - 1; ++i) {
        g.add_edge(simple_graph::Edge<int, ssize_t>(i + 1, i, 0, 1));
    }

    while (state.KeepRunning()) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::bellman_ford(g, state.range_x() - 1, 0, &path));
    }
}
BENCHMARK(bench_bellman_ford_reversed)->Range(1<<10, 1<<16);

static void bench_bellman_ford_random(benchmark::State &state)
{
    std::vector<vertex_index_t> vertices;
    simple_graph::ListGraph<true, int, int, ssize_t> g;
    for (int i = 0; i < state.range_x(); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
        vertices.push_back(i);
    }
    std::random_shuffle(vertices.begin(), vertices.end());
    for (int i = 0; i < state.range_x() - 1; ++i) {
        g.add_edge(simple_graph::Edge<int, ssize_t>(vertices[i], vertices[i + 1], 0, 1));
    }

    while (state.KeepRunning()) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::bellman_ford(g, vertices[0], vertices[state.range_x() - 1], &path));
    }
}
BENCHMARK(bench_bellman_ford_random)->Range(1<<10, 1<<13);

BENCHMARK_MAIN()
