import numpy as np
import pytest

import networkx as nx
import basegraph as bg


bg_directed_graph = bg.load_directed_edgelist_from_text_file("assets/directed_graph.txt")
nx_directed_graph = nx.read_edgelist("assets/directed_graph.txt", create_using=nx.DiGraph)
bg_directed_graph.remove_self_loops()
nx_directed_graph.remove_edges_from(nx.selfloop_edges(nx_directed_graph))

bg_undirected_graph = bg.load_undirected_edgelist_from_text_file("assets/undirected_graph.txt")
nx_undirected_graph = nx.read_edgelist("assets/undirected_graph.txt")


nx_graphs = [nx_directed_graph, nx_undirected_graph]
bg_graphs = [bg_directed_graph, bg_undirected_graph]


def undirected_index(vertex_label):
    return bg_undirected_graph.find_vertex_index(vertex_label)

def directed_index(vertex_label):
    return bg_directed_graph.find_vertex_index(vertex_label)

indices = [directed_index, undirected_index]


def get_graphs_and_index():
    return zip(nx_graphs, bg_graphs, indices)

def get_reversed_graphs_and_index():
    return zip([nx_directed_graph.reverse(), nx_undirected_graph], bg_graphs, indices)


def get_networkx_metric_handler(values, default=0):
    def f(key):
        try:
            return values[key]
        except KeyError:
            return default
    return f


class TestGeneralMetrics:
    def test_shortest_path_lengths(self):
        for nx_graph, bg_graph, index in get_graphs_and_index():
            vertices = bg_graph.get_vertices()
            n = bg_graph.get_size()

            for vertex_label, nx_distances in nx.algorithms.shortest_paths.generic.shortest_path_length(nx_graph):
                ordered_distances = list(map(get_networkx_metric_handler(nx_distances, -1), vertices))

                bg_distances = bg.find_shortest_path_lengths_from_vertex_idx(bg_graph, index(vertex_label))
                bg_distances = list(map(lambda x: -1 if x>n else x, bg_distances))

                assert ordered_distances == bg_distances

    def test_closeness_centrality(self):
        for nx_graph, bg_graph, index in get_reversed_graphs_and_index():
            nx_centralities = nx.closeness_centrality(nx_graph, wf_improved=False)

            for vertex_label, nx_centrality in nx_centralities.items():
                assert nx_centrality == bg.get_closeness_centrality_of_vertex_idx(bg_graph, index(vertex_label))

    def test_harmonic_centrality(self):
        for nx_graph, bg_graph, index in get_reversed_graphs_and_index():
            nx_centralities = nx.algorithms.centrality.harmonic_centrality(nx_graph)

            for vertex_label, nx_metric in nx_centralities.items():
                assert pytest.approx(nx_metric) == bg.get_harmonic_centrality_of_vertex_idx(bg_graph, index(vertex_label))

    def test_betweenness(self):
        for nx_graph, bg_graph, index in get_graphs_and_index():
            nx_centralities = nx.betweenness_centrality(nx_graph, k=bg_graph.get_size(), normalized=False)
            bg_centralities = bg.get_betweennesses(bg_graph, True)

            for vertex_label, nx_metric in nx_centralities.items():
                assert pytest.approx(nx_metric) == bg_centralities[index(vertex_label)]

    def test_diameters(self):
        # Can't compare directed graph because networkx does not support weakly connected directed graphs
        nx_diameters = nx.algorithms.distance_measures.eccentricity(nx_undirected_graph)
        bg_diameters = bg.get_diameters(bg_undirected_graph)

        for vertex_label, nx_metric in nx_diameters.items():
            assert nx_metric == bg_diameters[undirected_index(vertex_label)]

    def test_connected_components(self):
        nx_components = list(nx.algorithms.components.connected_components(nx_undirected_graph))
        bg_components = [ set(bg_undirected_graph.get_vertices()[j] for j in i) for i in bg.find_connected_components(bg_undirected_graph) ]

        assert nx_components == bg_components


class TestUndirectedMetrics:
    def test_assortativity(self):
        nx_metric = nx.algorithms.assortativity.degree_pearson_correlation_coefficient(nx_undirected_graph)
        bg_metric = bg.get_degree_correlation(bg_undirected_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_local_clustering(self):
        nx_metrics = nx.algorithms.cluster.clustering(nx_undirected_graph)
        bg_metrics = bg.get_local_clustering_coefficients(bg_undirected_graph)

        for vertex_label, nx_metric in nx_metrics.items():
            assert pytest.approx(nx_metric) == bg_metrics[undirected_index(vertex_label)]

    def test_global_clustering(self):
        nx_metric = nx.algorithms.cluster.transitivity(nx_undirected_graph)
        bg_metric = bg.get_global_clustering_coefficient(bg_undirected_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_kshells(self):
        nx_metrics = nx.algorithms.core.core_number(nx_undirected_graph)
        bg_metrics = bg.get_kshells(bg_undirected_graph)

        for vertex_label, nx_metric in nx_metrics.items():
            assert nx_metric == bg_metrics[undirected_index(vertex_label)]

    def test_onion_decomposition(self):
        nx_metrics = nx.algorithms.core.onion_layers(nx_undirected_graph)
        bg_metrics = bg.get_onion_layers(bg_undirected_graph)

        for vertex_label, nx_metric in nx_metrics.items():
            assert nx_metric == bg_metrics[undirected_index(vertex_label)]

    def test_triangle_number(self):
        nx_metric = nx.algorithms.cluster.triangles(nx_undirected_graph)
        bg_metric = bg.count_triangles(bg_undirected_graph)

    def test_modularity(self):
        n = bg_undirected_graph.get_size()
        vertices = bg_undirected_graph.get_vertices()

        bg_classes = [ i%2 for i in range(n) ]
        nx_classes = [ [vertices[i] for i in range(0, n, 2)],
                        [vertices[i] for i in range(1, n, 2)] ]

        nx_metric = nx.algorithms.community.quality.modularity(nx_undirected_graph, nx_classes)
        bg_metric = bg.get_modularity(bg_undirected_graph, bg_classes)
        assert pytest.approx(nx_metric) == bg_metric


class TestDirectedMetrics:
    def test_density(self):
        nx_metric = nx.classes.function.density(nx_directed_graph)
        bg_metric = bg.get_density(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_global_reciprocity(self):
        nx_metric = nx.algorithms.reciprocity(nx_directed_graph)
        bg_metric = bg.get_reciprocity(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_undirected_local_clustering(self):
        nx_metrics = nx.algorithms.cluster.clustering(nx_directed_graph.to_undirected())
        bg_metrics = bg.get_undirected_local_clustering_coefficients(bg_directed_graph)

        for vertex_label, nx_metric in nx_metrics.items():
            assert pytest.approx(nx_metric) == bg_metrics[directed_index(vertex_label)]

    def test_undirected_global_clustering(self):
        nx_metric = nx.algorithms.cluster.transitivity(nx_directed_graph.to_undirected())
        bg_metric = bg.get_undirected_global_clustering_coefficient(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric
