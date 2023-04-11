from time import perf_counter_ns

import basegraph as bg
import graph_tool.all as gt
import networkx as nx
import numpy as np


class Timer:

    def __init__(self):
        self.begin = None
        self.end = None

    def start(self):
        self.begin = perf_counter_ns()

    def stop(self):
        self.end = perf_counter_ns()

    def get_time_ms(self):
        if self.begin is None or self.end is None:
            raise ValueError("Timer was not started or stopped.")
        return (self.end - self.begin) / 1000


def benchmark(func, library, n):
    times = []
    for _ in range(n):
        timer = Timer()
        func(timer)
        times.append(timer.get_time_ms())
    print(
        f"{library}:\t{np.average(times):.4f}±{np.std(times)/np.sqrt(n):.4f} ms"
    )


def basic_timed_func(func):

    def timed_func(timer):
        timer.start()
        func()
        timer.stop()

    return timed_func


if __name__ == "__main__":
    edgelist_name = "assets/undirected_graph.txt"

    bg.random.seed(420)
    bg_graph = bg.random.generate_small_world_random_graph(10000, 3, 0.001)

    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(range(bg_graph.get_size()))
    gt_graph = gt.Graph()
    gt_vertices = [gt_graph.add_vertex() for _ in bg_graph]
    for edge in bg_graph.edges():
        nx_graph.add_edge(edge[0], edge[1])
        gt_graph.add_edge(gt_vertices[edge[0]], gt_vertices[edge[1]])

    source_vertex = 42
    benchmark_iterations = 20

    basegraph_name = "BaseGraph"
    networkx_name = "NetworkX"
    graphtool_name = "graph-tool"
    print(f"Benchmarks using {benchmark_iterations} iterations.\n")

    print("Benchmark - Shortest paths")
    benchmark(
        basic_timed_func(
            lambda: bg.metrics.get_shortest_path_lengths_from_vertex(
                bg_graph, source_vertex)), basegraph_name,
        benchmark_iterations)
    benchmark(
        basic_timed_func(lambda: gt.shortest_distance(
            gt_graph, gt_vertices[source_vertex])), graphtool_name,
        benchmark_iterations)
    benchmark(
        basic_timed_func(lambda: nx.algorithms.shortest_paths.generic.
                         shortest_path_length(nx_graph, source=source_vertex)),
        networkx_name, benchmark_iterations)

    print("\nBenchmark - Global clustering")
    benchmark(
        basic_timed_func(
            lambda: bg.metrics.get_global_clustering_coefficient(bg_graph)),
        basegraph_name, benchmark_iterations)
    benchmark(basic_timed_func(lambda: gt.global_clustering(gt_graph)),
              graphtool_name, benchmark_iterations)
    benchmark(
        basic_timed_func(lambda: nx.algorithms.cluster.transitivity(nx_graph)),
        networkx_name, benchmark_iterations)

    print("\nBenchmark - Local clustering")
    benchmark(
        basic_timed_func(
            lambda: bg.metrics.get_local_clustering_coefficients(bg_graph)),
        basegraph_name, benchmark_iterations)
    benchmark(basic_timed_func(lambda: gt.local_clustering(gt_graph)),
              graphtool_name, benchmark_iterations)
    benchmark(basic_timed_func(lambda: nx.clustering(nx_graph)), networkx_name,
              benchmark_iterations)
