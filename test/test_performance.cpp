#include <random>
#include "benchmark/benchmark.h"
#include "simple_graph/list_graph.hpp"
#include "simple_graph/algorithm/bfs.hpp"
#include "simple_graph/algorithm/dfs.hpp"
#include "simple_graph/algorithm/astar.hpp"

using simple_graph::vertex_index_t;

static void bench_creation(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, int, int> g;
    while (state.KeepRunning()) {
        state.PauseTiming();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, state.range(0) - 1);
        std::set<std::pair<size_t, size_t>> data;
        for (int j = 0; j < state.range(1); ++j) {
            data.insert(std::make_pair<int, int>(dist(gen), dist(gen)));
        }
        state.ResumeTiming();

        for (int i = 0; i < state.range(0); ++i) {
            g.add_vertex(simple_graph::Vertex<int>(i));
        }
        for (auto &d : data) {
            g.add_edge(simple_graph::Edge<int, int>(d.first, d.second, 0));
        }
    }

    state.SetComplexityN(state.range(0));
}
BENCHMARK(bench_creation)->RangePair(1<<10, 8<<16, 100, 1<<12)->Complexity();

static void bench_bfs_path_length(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, int,  ssize_t> g;
    for (int i = 0; i < state.range(0); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range(0) - 1; ++i) {
        g.add_edge(simple_graph::Edge<int, ssize_t>(i, i + 1, 0, 1));
    }

    std::function<bool(int)> f = [&](int c) { return c == state.range(0) - 1; };

    for (auto _ : state) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::bfs(g, 0, f, &path));
    }

    state.SetComplexityN(state.range(0));
}
BENCHMARK(bench_bfs_path_length)->Range(1<<10, 1<<20)->Complexity();

static void bench_dfs_path_length(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, int, ssize_t> g;
    for (int i = 0; i < state.range(0); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range(0) - 1; ++i) {
        g.add_edge(simple_graph::Edge<int, ssize_t>(i, i + 1, 0, 1));
    }

    std::function<bool(int)> f = [&](int c) { return c == state.range(0) - 1; };

    for (auto _ : state) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::dfs(g, 0, f, &path));
    }

    state.SetComplexityN(state.range(0));
}
BENCHMARK(bench_dfs_path_length)->Range(1<<10, 1<<20)->Complexity();

static void bench_astar_path_length(benchmark::State &state)
{
    simple_graph::ListGraph<false, int, int, ssize_t> g;
    for (int i = 0; i < state.range(0); ++i) {
        g.add_vertex(simple_graph::Vertex<int>(i, i));
    }
    for (int i = 0; i < state.range(0) - 1; ++i) {
        g.add_edge(simple_graph::Edge<int, ssize_t>(i, i + 1, 0, 1));
    }

    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        return r - c;
    };

    for (auto _ : state) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::astar(g, 0, state.range(0) - 1, heuristic, &path));
    }

    state.SetComplexityN(state.range(0));
}
BENCHMARK(bench_astar_path_length)->Range(1<<10, 1<<20)->Complexity();

static void bench_astar(benchmark::State &state)
{
    simple_graph::ListGraph<false, std::pair<int, int>, int, ssize_t> g;

    for (int i = 0; i < state.range(0); ++i) {
        for (int j = 0; j < state.range(0); ++j) {
            g.add_vertex(simple_graph::Vertex<std::pair<int, int>>(i * state.range(0) + j, {i, j}));
        }
    }

    /// TODO Use random.
    for (int i = 0; i < state.range(0) - 1; ++i) {
        for (int j = 0; j < state.range(0); ++j) {
            if (j != 0) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * state.range(0) + j, i * state.range(0) + j - 1, 0, 1));
            }
            if (j != state.range(0) - 1) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * state.range(0) + j, i * state.range(0) + j + 1, 0, 1));
            }
            if (i != 0) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * state.range(0) + j, (i - 1) * state.range(0) + j, 0, 1));
            }
            if (i != state.range(0) - 1) {
                g.add_edge(simple_graph::Edge<int, ssize_t>(i * state.range(0) + j, (i + 1) * state.range(0) + j, 0, 1));
            }
        }
    }

    std::function<float(vertex_index_t, vertex_index_t)> heuristic = [=](vertex_index_t c, vertex_index_t r) {
        int ci = c / state.range(0);
        int cj = c % state.range(0);

        int ri = r / state.range(0);
        int rj = r % state.range(0);

        return std::sqrt(std::pow(ci - ri, 2) + std::pow(cj - rj, 2));
    };

    for (auto _ : state) {
        std::vector<vertex_index_t> path;
        benchmark::DoNotOptimize(simple_graph::astar(g, 0, state.range(0) * state.range(0) - 1, heuristic, &path));
    }

    state.SetComplexityN(state.range(0));
}
BENCHMARK(bench_astar)->Range(1<<2, 1<<8)->Complexity();

BENCHMARK_MAIN();
