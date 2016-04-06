#include <random>
#include "benchmark/benchmark.h"
#include "simple_graph/list_graph.hpp"
#include "simple_graph/bfs.hpp"
#include "simple_graph/astar.hpp"

using simple_graph::vertex_index_t;

static void bench_creation(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, int> g;
    while (state.KeepRunning()) {
        state.PauseTiming();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, state.range_x() - 1);
        std::set<std::pair<size_t, size_t>> data;
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
BENCHMARK(bench_creation)->RangePair(1<<10, 8<<16, 100, 1<<12);

static void bench_bfs_path_length(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, ssize_t> g;
    for (int i = 0; i < state.range_x(); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range_x() - 1; ++i) {
        g.add_edge(simple_graph::Edge<ssize_t>(i, i + 1, 1));
    }

    std::function<bool(int)> f = [&](int c) { return c == state.range_x() - 1; };

    while (state.KeepRunning()) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::bfs(g, 0, f, &path));
    }
}
BENCHMARK(bench_bfs_path_length)->Range(1<<10, 1<<20);

static void bench_astar_path_length(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, ssize_t> g;
    for (int i = 0; i < state.range_x(); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range_x() - 1; ++i) {
        g.add_edge(simple_graph::Edge<ssize_t>(i, i + 1, 1));
    }

    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return r - c;
    };

    while (state.KeepRunning()) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::astar(g, 0, state.range_x() - 1, heuristic, &path));
    }
}
BENCHMARK(bench_astar_path_length)->Range(1<<10, 1<<20);

BENCHMARK_MAIN()
