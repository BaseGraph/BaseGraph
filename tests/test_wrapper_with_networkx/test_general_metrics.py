import pytest

import networkx as nx
from basegraph import metrics


from fixtures import (
            small_directed_fixture, directed_fixture,
            small_undirected_fixture, undirected_fixture,
            eq, approx, eq_all_vertices, approx_all_vertices
        )
import networkx_additional_metrics as nx_add


def test_closeness_centrality(directed_fixture, undirected_fixture):
    approx_all_vertices(
        directed_fixture+undirected_fixture,
        metrics.get_closeness_centralities,
        lambda g: nx.closeness_centrality(g.reverse() if g.is_directed() else g, wf_improved=False)
    )


def test_harmonic_centrality(directed_fixture, undirected_fixture):
    approx_all_vertices(
        directed_fixture+undirected_fixture,
        metrics.get_harmonic_centralities,
        lambda g: nx.algorithms.centrality.harmonic_centrality(g.reverse() if g.is_directed() else g)
    )


def test_betweenness(small_directed_fixture, small_undirected_fixture):
    approx_all_vertices(
        small_directed_fixture+small_undirected_fixture,
        lambda g: metrics.get_betweenness_centralities(g, True),
        lambda g: nx.betweenness_centrality(g, k=g.number_of_nodes(), normalized=False)
    )


@pytest.mark.skip(reason="current BaseGraph implementation has to be validated for disconnected graphs")
def test_diameters(directed_fixture, undirected_fixture):
    eq_all_vertices(
        directed_fixture+undirected_fixture,
        metrics.get_diameters,
        nx_add.get_diameters
    )


def test_shortest_path_lengths(directed_fixture, undirected_fixture):
    # Need to filter invalid shortest paths (length >= n)
    def bg_shortest_path_lengths(graph, vertices):
        n = graph.get_size()
        shortest_path_lengths = {}

        for i, vertex in enumerate(vertices):
            path_lengths = metrics.get_shortest_path_lengths_from_vertex(graph, i)
            shortest_path_lengths[vertex] = {vertices[j]: length for j, length in enumerate(path_lengths) if length<n}
        return shortest_path_lengths

    eq(
        directed_fixture+undirected_fixture,
        bg_shortest_path_lengths,
        nx_add.get_shortest_path_lengths
    )


def test_average_shortest_paths(directed_fixture, undirected_fixture):
    approx_all_vertices(
        directed_fixture+undirected_fixture,
        metrics.get_shortest_path_averages,
        nx_add.get_shortest_path_averages
    )


def test_harmonic_average_shortest_paths(directed_fixture, undirected_fixture):
    approx_all_vertices(
        directed_fixture+undirected_fixture,
        metrics.get_shortest_path_harmonic_averages,
        nx_add.get_shortest_path_harmonic_averages
    )
