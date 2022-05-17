import numpy as np
import pytest

import networkx as nx
import basegraph as bg
import networkx_additional_metrics as nx_add


bg_directed_graph = bg.io.load_directed_edgelist_from_text_file("tests/assets/directed_graph.txt")
nx_directed_graph = nx.read_edgelist("tests/assets/directed_graph.txt", create_using=nx.DiGraph)
bg_undirected_graph = bg.io.load_undirected_edgelist_from_text_file("tests/assets/undirected_graph.txt")
nx_undirected_graph = nx.read_edgelist("tests/assets/undirected_graph.txt")

bg_directed_graph.remove_self_loops()
nx_directed_graph.remove_edges_from(nx.selfloop_edges(nx_directed_graph))

bg_small_directed_graph = bg.io.load_directed_edgelist_from_text_file("tests/assets/small_directed_graph.txt")
nx_small_directed_graph = nx.read_edgelist("tests/assets/small_directed_graph.txt", create_using=nx.DiGraph)
bg_small_undirected_graph = bg.io.load_undirected_edgelist_from_text_file("tests/assets/small_undirected_graph.txt")
nx_small_undirected_graph = nx.read_edgelist("tests/assets/small_undirected_graph.txt")

nx_graphs_reversed = [nx_small_directed_graph.reverse(), nx_directed_graph.reverse(), nx_small_undirected_graph, nx_undirected_graph]
nx_graphs = [nx_small_directed_graph, nx_directed_graph, nx_small_undirected_graph, nx_undirected_graph]
bg_graphs = [bg_small_directed_graph, bg_directed_graph, bg_small_undirected_graph, bg_undirected_graph]
nx_small_graphs = [nx_small_directed_graph, nx_small_undirected_graph]
bg_small_graphs = [bg_small_directed_graph, bg_small_undirected_graph]


def get_graphs():
    return zip(nx_graphs, bg_graphs)

def get_reversed_graphs():
    return zip(nx_graphs_reversed, bg_graphs)

def get_small_graphs():
    return zip(nx_small_graphs, bg_small_graphs)


class TestPathAlgorithms:
    def test_shortest_paths(self):
        for nx_graph, bg_graph in get_small_graphs():
            vertex_labels = bg_graph.get_vertices()
            nx_shortest_paths = nx.algorithms.shortest_paths.generic.shortest_path(nx_graph)
            n = bg_graph.get_size()

            directed = isinstance(nx_graph, nx.DiGraph)

            for i in bg_graph:
                bg_path1 = [[vertex_labels[k] for k in path] for path in bg.metrics.find_geodesics_from_vertex_idx(bg_graph, i)]
                for j in bg_graph:
                    if i==j:
                        continue
                    if j>=i and not directed:
                       break

                    bg_path2 = [vertex_labels[k] for k in bg.metrics.find_geodesics_idx(bg_graph, i, j)]
                    vertex_j_accessible = vertex_labels[j] in nx_shortest_paths[vertex_labels[i]].keys()

                    if vertex_j_accessible:
                        assert bg_path1[j] == nx_shortest_paths[vertex_labels[i]][vertex_labels[j]]
                        assert bg_path2 == nx_shortest_paths[vertex_labels[i]][vertex_labels[j]]
                    else:
                        assert bg_path1[j] == []
                        assert bg_path2 == []

    def test_all_shortest_paths(self):
        for nx_graph, bg_graph in get_small_graphs():
            vertex_labels = bg_graph.get_vertices()
            n = bg_graph.get_size()

            directed = isinstance(nx_graph, nx.DiGraph)

            for i in bg_graph:
                bg_path1 = [[[vertex_labels[k] for k in path] for path in paths] for paths in bg.metrics.find_all_geodesics_from_vertex_idx(bg_graph, i)]
                for j in bg_graph:
                    if i==j:
                        continue
                    if j>=i and not directed:
                       break

                    bg_path2 = [[vertex_labels[k] for k in path] for path in bg.metrics.find_all_geodesics_idx(bg_graph, i, j)]

                    if bg_path2 != []:
                        nx_shortest_paths = list(nx.algorithms.shortest_paths.generic.all_shortest_paths(nx_graph, vertex_labels[i], vertex_labels[j])).sort()

                        assert bg_path1[j].sort() == nx_shortest_paths
                        assert bg_path2.sort() == nx_shortest_paths
                    else:
                        try:
                            list(nx.algorithms.shortest_paths.generic.all_shortest_paths(nx_graph, vertex_labels[i], vertex_labels[j]))
                            assert False
                        except nx.exception.NetworkXNoPath:
                            assert bg_path1[j] == []
                            assert bg_path2 == []


class TestGeneralMetrics:
    def test_shortest_path_lengths(self):
        for nx_graph, bg_graph in get_graphs():
            vertex_labels = bg_graph.get_vertices()
            nx_shortest_paths = nx_add.get_shortest_paths(nx_graph, vertex_labels)
            n = bg_graph.get_size()

            for i in bg_graph:
                bg_distances = bg.metrics.find_shortest_path_lengths_from_vertex_idx(bg_graph, i)
                bg_distances = list(map(lambda x: -1 if x>n else x, bg_distances))

                assert nx_shortest_paths[i] == bg_distances

    def test_average_shortest_paths(self):
        for nx_graph, bg_graph in get_graphs():
            vertex_labels = bg_graph.get_vertices()
            bg_average_shortest_paths = bg.metrics.get_shortest_path_averages(bg_graph)
            nx_average_shortest_paths = nx_add.get_shortest_path_averages(nx_graph, vertex_labels)

            for i in bg_graph:
                assert pytest.approx(nx_average_shortest_paths[i]) == bg_average_shortest_paths[i]

    def test_harmonic_average_shortest_paths(self):
        for nx_graph, bg_graph in get_graphs():
            vertex_labels = bg_graph.get_vertices()
            bg_harmonic_averages = bg.metrics.get_shortest_path_harmonic_averages(bg_graph)
            nx_harmonic_averages = nx_add.get_shortest_path_harmonic_averages(nx_graph, vertex_labels)

            for i in bg_graph:
                assert pytest.approx(nx_harmonic_averages[i]) == bg_harmonic_averages[i]

    def test_closeness_centrality(self):
        for nx_graph, bg_graph in get_reversed_graphs():
            nx_centralities = nx.closeness_centrality(nx_graph, wf_improved=False)
            bg_centralities = bg.metrics.get_closeness_centralities(bg_graph)

            for i, vertex_label in enumerate(bg_graph.get_vertices()):
                assert pytest.approx(nx_centralities[vertex_label]) == bg_centralities[i]

    def test_harmonic_centrality(self):
        for nx_graph, bg_graph in get_reversed_graphs():
            nx_centralities = nx.algorithms.centrality.harmonic_centrality(nx_graph)
            bg_centralities = bg.metrics.get_harmonic_centralities(bg_graph)

            for i, vertex_label in enumerate(bg_graph.get_vertices()):
                assert pytest.approx(nx_centralities[vertex_label]) == bg_centralities[i]

    def test_betweenness(self):
        for nx_graph, bg_graph in get_graphs():
            nx_centralities = nx.betweenness_centrality(nx_graph, k=bg_graph.get_size(), normalized=False)
            bg_centralities = bg.metrics.get_betweenness_centralities(bg_graph, True)

            for i, vertex_label in enumerate(bg_graph.get_vertices()):
                assert pytest.approx(nx_centralities[vertex_label]) == bg_centralities[i]

    def test_diameters(self):
        # Can't compare directed graph because networkx does not support weakly connected directed graphs
        nx_diameters = nx.algorithms.distance_measures.eccentricity(nx_undirected_graph)
        bg_diameters = bg.metrics.get_diameters(bg_undirected_graph)

        for i, vertex_label in enumerate(bg_undirected_graph.get_vertices()):
            assert pytest.approx(nx_diameters[vertex_label]) == bg_diameters[i]

    def test_connected_components(self):
        nx_components = list(nx.algorithms.components.connected_components(nx_undirected_graph))
        bg_components = [ set(bg_undirected_graph.get_vertices()[j] for j in i) for i in bg.metrics.find_connected_components(bg_undirected_graph) ]

        assert nx_components == bg_components


class TestUndirectedMetrics:
    def test_assortativity(self):
        nx_metric = nx.algorithms.assortativity.degree_pearson_correlation_coefficient(nx_undirected_graph)
        bg_metric = bg.metrics.get_degree_correlation(bg_undirected_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_local_clustering(self):
        nx_metrics = nx.algorithms.cluster.clustering(nx_undirected_graph)
        bg_metrics = bg.metrics.get_local_clustering_coefficients(bg_undirected_graph)

        for i, vertex_label in enumerate(bg_undirected_graph.get_vertices()):
            assert pytest.approx(nx_metrics[vertex_label]) == bg_metrics[i]

    def test_clustering_spectrum(self):
        nx_metrics = nx_add.get_clustering_spectrum(nx_undirected_graph)
        bg_metrics = bg.metrics.get_clustering_spectrum(bg_undirected_graph)
        assert pytest.approx(nx_metrics) == bg_metrics

    def test_global_clustering(self):
        nx_metric = nx.algorithms.cluster.transitivity(nx_undirected_graph)
        bg_metric = bg.metrics.get_global_clustering_coefficient(bg_undirected_graph)
        assert pytest.approx(nx_metric) == bg_metric

    @pytest.mark.skip()
    def test_kshells(self):
        nx_metrics = nx.algorithms.core.core_number(nx_undirected_graph)
        bg_metrics = bg.metrics.get_kshells(bg_undirected_graph)

        for i, vertex_label in enumerate(bg_undirected_graph.get_vertices()):
            assert nx_metrics[vertex_label] == bg_metrics[i]

    @pytest.mark.skip()
    def test_onion_decomposition(self):
        nx_metrics = nx.algorithms.core.onion_layers(nx_undirected_graph)
        bg_metrics = bg.metrics.get_onion_layers(bg_undirected_graph)

        for i, vertex_label in enumerate(bg_undirected_graph.get_vertices()):
            assert nx_metrics[vertex_label] == bg_metrics[i]

    @pytest.mark.skip()
    def test_onion_spectrum(self):
        nx_metric = nx_add.get_onion_spectrum(nx_undirected_graph, bg_undirected_graph.get_vertices())
        bg_metric = bg.metrics.get_onion_spectrum(bg_undirected_graph)
        assert nx_metric == bg_metric

    def test_triangle_number(self):
        nx_metrics = nx.algorithms.cluster.triangles(nx_undirected_graph)

        for i, vertex_label in enumerate(bg_undirected_graph.get_vertices()):
            assert nx_metrics[vertex_label] == bg.metrics.count_triangles_around_vertex_idx(bg_undirected_graph, i)
        triangle_total = sum(nx_metrics.values())/3
        assert triangle_total == bg.metrics.count_triangles(bg_undirected_graph)

    def test_modularity(self):
        n = bg_undirected_graph.get_size()
        vertices = bg_undirected_graph.get_vertices()

        bg_classes = [ i%2 for i in range(n) ]
        nx_classes = [ [vertices[i] for i in range(0, n, 2)],
                        [vertices[i] for i in range(1, n, 2)] ]

        nx_metric = nx.algorithms.community.quality.modularity(nx_undirected_graph, nx_classes)
        bg_metric = bg.metrics.get_modularity(bg_undirected_graph, bg_classes)
        assert pytest.approx(nx_metric) == bg_metric


class TestDirectedMetrics:
    def test_density(self):
        nx_metric = nx.classes.function.density(nx_directed_graph)
        bg_metric = bg.metrics.get_density(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_global_reciprocity(self):
        nx_metric = nx.algorithms.reciprocity(nx_directed_graph)
        bg_metric = bg.metrics.get_reciprocity(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_undirected_local_clustering(self):
        nx_metrics = nx.algorithms.cluster.clustering(nx_directed_graph.to_undirected())
        bg_metrics = bg.metrics.get_undirected_local_clustering_coefficients(bg_directed_graph)

        for i, vertex_label in enumerate(bg_directed_graph.get_vertices()):
            assert nx_metrics[vertex_label] == bg_metrics[i]

    def test_undirected_global_clustering(self):
        nx_metric = nx.algorithms.cluster.transitivity(nx_directed_graph.to_undirected())
        bg_metric = bg.metrics.get_undirected_global_clustering_coefficient(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric
