import pytest

import networkx as nx
from basegraph import metrics

from fixtures import (
            small_undirected_fixture, undirected_fixture,
            eq, approx, eq_all_vertices, approx_all_vertices
        )
import networkx_additional_metrics as nx_add


def test_assortativity(undirected_fixture):
    approx(
        undirected_fixture,
        metrics.get_degree_correlation,
        nx.algorithms.assortativity.degree_pearson_correlation_coefficient
    )


def test_local_clustering(undirected_fixture):
    approx_all_vertices(
        undirected_fixture,
        metrics.get_local_clustering_coefficients,
        nx.algorithms.cluster.clustering
    )


def test_node_redundancy(undirected_fixture):
    approx_all_vertices(
        undirected_fixture,
        metrics.get_redundancy,
        nx_add.get_redundancy
    )


def test_clustering_spectrum(undirected_fixture):
    approx(
        undirected_fixture,
        metrics.get_clustering_spectrum,
        nx_add.get_clustering_spectrum
    )


def test_global_clustering(undirected_fixture):
    approx(
        undirected_fixture,
        metrics.get_global_clustering_coefficient,
        nx.algorithms.cluster.transitivity,
    )


def test_kshells(undirected_fixture):
    eq_all_vertices(
        undirected_fixture,
        metrics.get_kshells,
        nx.algorithms.core.core_number
    )


def test_onion_decomposition(undirected_fixture):
    eq_all_vertices(
        undirected_fixture,
        metrics.get_onion_layers,
        nx.algorithms.core.onion_layers
    )


def test_onion_spectrum(undirected_fixture):
    eq(
        undirected_fixture,
        metrics.get_onion_spectrum,
        nx_add.get_onion_spectrum
    )

def test_vertex_triangle_number(undirected_fixture):
    eq_all_vertices(
        undirected_fixture,
        lambda g: [metrics.count_triangles_around_vertex(g, i) for i in g],
        nx.algorithms.cluster.triangles
    )


def test_total_triangle_number(undirected_fixture):
    eq(
        undirected_fixture,
        metrics.count_triangles,
        lambda g: sum(nx.algorithms.cluster.triangles(g).values())/3
    )


def test_find_all_triangles(small_undirected_fixture):
    def bg_get_sorted_triangles(graph, vertices):
        triangles = []
        for triangle in metrics.find_all_triangles(graph):
            labeled_triangle = list(map(lambda i: vertices[i], triangle))
            triangles.append(labeled_triangle)

        return triangles

    eq(
        small_undirected_fixture,
        bg_get_sorted_triangles,
        nx_add.get_sorted_triangles
    )


def test_modularity(undirected_fixture):
    def class_from_vertex(vertex):
        return hash(vertex) % 2

    def bg_modularity(graph, vertices):
        classes = list(map(class_from_vertex, vertices))
        return metrics.get_modularity(graph, classes)

    def nx_modularity(graph):
        classes = [ [vertex for vertex in graph if class_from_vertex(vertex)==0],
                    [vertex for vertex in graph if class_from_vertex(vertex)==1] ]
        return nx.algorithms.community.quality.modularity(graph, classes)

    approx(
        undirected_fixture,
        bg_modularity,
        nx_modularity
    )


def test_neighbourhood_degrees(undirected_fixture):
    def bg_sorted_neighbourhood_degrees(graph, vertices):
        neighbourhood_degrees = {}

        for i, vertex in enumerate(vertices):
            neighbour_degrees = metrics.get_neighbourhood_degrees_of_vertex(graph, i)
            neighbour_degrees.sort()
            neighbourhood_degrees[vertex] = neighbour_degrees
        return neighbourhood_degrees

    def nx_sorted_neighbourhood_degrees(graph):
        degrees = graph.degree
        neighbourhood_degrees = {}

        for vertex in graph:
            neighbour_degrees = [degrees[neighbour] for neighbour in graph.neighbors(vertex)]
            neighbour_degrees.sort()
            neighbourhood_degrees[vertex] = neighbour_degrees
        return neighbourhood_degrees

    eq(
        undirected_fixture,
        bg_sorted_neighbourhood_degrees,
        nx_sorted_neighbourhood_degrees
    )


def test_average_neighbour_degree(undirected_fixture):
    approx_all_vertices(
        undirected_fixture,
        lambda g: metrics.get_neighbourhood_degree_spectrum(g, False),
        nx.average_neighbor_degree
    )


def test_connected_components(undirected_fixture):
    eq(
        undirected_fixture,
        lambda g, vertices: [ set(vertices[i] for i in component) for component in metrics.find_connected_components(g) ],
        lambda g: list(nx.algorithms.components.connected_components(g))
    )
