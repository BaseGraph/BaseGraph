from networkx import exception
import pytest

import networkx as nx
from basegraph import metrics

from fixtures import small_directed_fixture, small_undirected_fixture


def test_shortest_paths(small_directed_fixture, small_undirected_fixture):
    for bg_graph, nx_graph, vertices in small_directed_fixture+small_undirected_fixture:
        nx_shortest_paths = nx.algorithms.shortest_paths.generic.shortest_path(nx_graph)

        for i, vertex in enumerate(vertices):
            bg_path1 = [[vertices[k] for k in path] for path in metrics.find_geodesics_from_vertex(bg_graph, i)]
            for j in bg_graph:
                if not (i<=j or nx_graph.is_directed()):  # Checking only i<=j for undirected graphs
                   break

                bg_path2 = [vertices[k] for k in metrics.find_geodesics(bg_graph, i, j)]

                nx_shortest_path = nx_shortest_paths[vertex].get(vertices[j], [])
                assert bg_path1[j] == nx_shortest_path
                assert bg_path2 == nx_shortest_path


def test_all_shortest_paths(small_directed_fixture, small_undirected_fixture):
    for bg_graph, nx_graph, vertices in small_directed_fixture+small_undirected_fixture:
        for i, vertex in enumerate(vertices):
            bg_path1 = [[[vertices[k] for k in path] for path in paths] for paths in metrics.find_all_geodesics_from_vertex(bg_graph, i)]
            for j in bg_graph:
                if not (i<=j or nx_graph.is_directed()):  # Checking only i<=j for undirected graphs
                   break

                bg_path2 = [[vertices[k] for k in path] for path in metrics.find_all_geodesics(bg_graph, i, j)]

                bg_path1[j].sort()
                bg_path2.sort()
                assert bg_path1[j] == bg_path2

                if bg_path2 == []:
                    with pytest.raises(nx.exception.NetworkXNoPath) as err:
                        list(nx.algorithms.shortest_paths.generic.all_shortest_paths(nx_graph, vertex, vertices[j]))
                else:
                    nx_shortest_paths = list(nx.algorithms.shortest_paths.generic.all_shortest_paths(nx_graph, vertex, vertices[j]))
                    nx_shortest_paths.sort()
                    assert bg_path2 == nx_shortest_paths
