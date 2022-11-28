import inspect
import pytest

import networkx as nx
from basegraph import _core


filenames = {
        "undirected": ["tests/assets/small_undirected_graph.txt", "tests/assets/undirected_graph.txt"],
        "directed": ["tests/assets/small_directed_graph.txt", "tests/assets/directed_graph.txt"]
    }
small_graph_threshold = 100


def get_graphs(filename, directed):
    if not directed:
        #bg_graph, vertices = _core.load_undirected_text_edgelist(filename, _core.vertex_counter)
        bg_graph, vertices = _core.load_undirected_text_edgelist(filename)
        return bg_graph, nx.read_edgelist(filename), vertices

    #bg_graph, vertices = _core.load_directed_text_edgelist(filename, _core.vertex_counter)
    bg_graph, vertices = _core.load_directed_text_edgelist(filename)
    bg_graph.remove_self_loops()

    nx_graph = nx.read_edgelist(filename, create_using=nx.DiGraph)
    nx_graph.remove_edges_from(nx.selfloop_edges(nx_graph))

    return bg_graph, nx_graph, vertices


@pytest.fixture
def directed_fixture():
    return [get_graphs(filename, directed=True) for filename in filenames["directed"]]


@pytest.fixture
def small_directed_fixture():
    graphs = [get_graphs(filename, directed=True) for filename in filenames["directed"]]
    return list(filter(lambda graph: graph[0].get_size()<small_graph_threshold, graphs))


@pytest.fixture
def undirected_fixture():
    return [get_graphs(filename, directed=False) for filename in filenames["undirected"]]


@pytest.fixture
def small_undirected_fixture():
    graphs = [get_graphs(filename, directed=False) for filename in filenames["undirected"]]
    return list(filter(lambda graph: graph[0].get_size()<small_graph_threshold, graphs))


def call_metric(metric, graph, vertices):
    try:
        if "vertices" in str(inspect.signature(metric)):
            return metric(graph, vertices)
    except ValueError:
        pass
    return metric(graph)


def eq(graphs, bg_metric, nx_metric):
    for bg_graph, nx_graph, vertices in graphs:
        assert call_metric(bg_metric, bg_graph, vertices) == nx_metric(nx_graph)


def approx(graphs, bg_metric, nx_metric):
    for bg_graph, nx_graph, vertices in graphs:
        bg_value = call_metric(bg_metric, bg_graph, vertices)
        assert pytest.approx(bg_value) == nx_metric(nx_graph)


def eq_all_vertices(graphs, bg_metric, nx_metric):
    for bg_graph, nx_graph, vertices in graphs:
        bg_values = call_metric(bg_metric, bg_graph, vertices)
        nx_values = nx_metric(nx_graph)
        for i, vertex in enumerate(vertices):
            assert bg_values[i] == nx_values[vertex]


def approx_all_vertices(graphs, bg_metric, nx_metric):
    for bg_graph, nx_graph, vertices in graphs:
        bg_values = call_metric(bg_metric, bg_graph, vertices)
        nx_values = nx_metric(nx_graph)
        for i, vertex in enumerate(vertices):
            assert pytest.approx(bg_values[i]) == nx_values[vertex]
