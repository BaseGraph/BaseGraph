import networkx as nx
import pgl
import pytest


bg_directed_graph = pgl.load_directed_edgelist_from_text_file("assets/directed_graph.txt")
nx_directed_graph = nx.read_edgelist("assets/directed_graph.txt", create_using=nx.DiGraph)
bg_undirected_graph = pgl.load_undirected_edgelist_from_text_file("assets/undirected_graph.txt")
nx_undirected_graph = nx.read_edgelist("assets/undirected_graph.txt")

nx_graphs = [nx_undirected_graph, nx_directed_graph]
bg_graphs = [bg_undirected_graph, bg_directed_graph]


def undirected_index(vertex_label):
    return bg_undirected_graph.find_vertex_index(vertex_label)

def directed_index(vertex_label):
    return bg_directed_graph.find_vertex_index(vertex_label)

indices = [undirected_index, directed_index]


class TestGeneralMetrics:

    def test_closeness_centrality(self):
        nx_centralities = nx.closeness_centrality(nx_undirected_graph, wf_improved=False)

        for vertex_label, nx_metric in nx_centralities.items():
            assert nx_metric == pgl.get_closeness_centrality_of_vertex_idx(bg_undirected_graph, undirected_index(vertex_label))


    def test_harmonic_centrality(self):
        nx_centralities = nx.algorithms.centrality.harmonic_centrality(nx_undirected_graph)

        for vertex_label, nx_metric in nx_centralities.items():
            assert pytest.approx(nx_metric) == pgl.get_harmonic_centrality_of_vertex_idx(bg_undirected_graph, undirected_index(vertex_label))


    # Networkx uses a sampling method, base_graph is exact
    def test_betweeness(self):
        nx_centralities = nx.betweenness_centrality(nx_undirected_graph, k=bg_undirected_graph.get_size(), normalized=False)
        bg_centralities = pgl.get_betweenesses(bg_undirected_graph, True)

        for vertex_label, nx_metric in nx_centralities.items():
            assert pytest.approx(nx_metric) == bg_centralities[undirected_index(vertex_label)]


    def test_diameters(self):
        nx_diameters = nx.algorithms.distance_measures.eccentricity(nx_undirected_graph)
        bg_diameters = pgl.get_diameters(bg_undirected_graph)

        for vertex_label, nx_metric in nx_diameters.items():
            assert nx_metric == bg_diameters[undirected_index(vertex_label)]


    def test_connected_components(self):
        nx_components = list(nx.algorithms.components.connected_components(nx_undirected_graph))
        bg_components = [ set(bg_undirected_graph.get_vertices()[j] for j in i) for i in pgl.find_connected_components(bg_undirected_graph)]

        assert nx_components == bg_components


class TestUndirectedMetrics:
    pass

    def test_assortativity(self):
        nx_metric = nx.algorithms.assortativity.degree_pearson_correlation_coefficient(nx_undirected_graph)
        bg_metric = pgl.get_degree_correlation(bg_undirected_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_local_clustering(self):
        nx_metrics = nx.algorithms.cluster.clustering(nx_undirected_graph)
        bg_metrics = pgl.get_local_clustering_coefficients(bg_undirected_graph)

        for vertex_label, nx_metric in nx_metrics.items():
            assert pytest.approx(nx_metric) == bg_metrics[undirected_index(vertex_label)]

    def test_global_clustering(self):
        nx_metric = nx.algorithms.cluster.transitivity(nx_undirected_graph)
        bg_metric = pgl.get_global_clustering_coefficient(bg_undirected_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_kshells(self):
        nx_metrics = nx.algorithms.core.core_number(nx_undirected_graph)
        bg_metrics = pgl.get_kshells(bg_undirected_graph)

        for vertex_label, nx_metric in nx_metrics.items():
            assert nx_metric == bg_metrics[undirected_index(vertex_label)]

    def test_onion_decomposition(self):
        nx_metrics = nx.algorithms.core.onion_layers(nx_undirected_graph)
        bg_metrics = pgl.get_onion_layers(bg_undirected_graph)

        for vertex_label, nx_metric in nx_metrics.items():
            assert nx_metric == bg_metrics[undirected_index(vertex_label)]

    def test_triangle_number(self):
        nx_metric = nx.algorithms.cluster.triangles(nx_undirected_graph)
        bg_metric = pgl.count_triangles(bg_undirected_graph)


class TestDirectedMetrics:
    def test_density(self):
        nx_metric = nx.classes.function.density(nx_directed_graph)
        bg_metric = pgl.get_density(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric

    def test_global_reciprocity(self):
        nx_metric = nx.algorithms.reciprocity(nx_directed_graph)
        bg_metric = pgl.get_reciprocity(bg_directed_graph)
        assert pytest.approx(nx_metric) == bg_metric
