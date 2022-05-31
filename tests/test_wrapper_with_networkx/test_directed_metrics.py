import pytest

import networkx as nx
from basegraph import metrics

from fixtures import (
            small_directed_fixture, directed_fixture,
            eq, approx, eq_all_vertices, approx_all_vertices
        )
import networkx_additional_metrics as nx_add


def test_density(directed_fixture):
    approx(
        directed_fixture,
        metrics.get_density,
        nx.classes.function.density
    )


def test_reciprocity(directed_fixture):
    approx(
        directed_fixture,
        metrics.get_reciprocity,
        nx.algorithms.reciprocity
    )


def test_undirected_local_clustering(directed_fixture):
    eq_all_vertices(
        directed_fixture,
        metrics.get_undirected_local_clustering_coefficients,
        lambda g: nx.algorithms.cluster.clustering(g.to_undirected())
    )


def test_undirected_global_clustering(directed_fixture):
    approx(
        directed_fixture,
        metrics.get_undirected_global_clustering_coefficient,
        lambda g: nx.algorithms.cluster.transitivity(g.to_undirected())
    )
