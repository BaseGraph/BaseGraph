from collections import Counter
import networkx as nx
from networkx.algorithms.shortest_paths.generic import shortest_path
import numpy as np


def get_diameters(graph):
    if graph.is_directed():
        subgraphs = [graph.subgraph(component).copy() for component in nx.strongly_connected_components(graph)]
    else:
        subgraphs = [graph.subgraph(component).copy() for component in nx.connected_components(graph)]

    diameters = {}
    for component in subgraphs:
        diameters.update(nx.algorithms.distance_measures.eccentricity(component))
    return diameters


def get_redundancy(graph):
    degrees = graph.degree
    redundancy = nx.algorithms.cluster.clustering(graph)

    for vertex in redundancy:
        redundancy[vertex] *= degrees[vertex]-1
    return redundancy


def get_sorted_triangles(graph):
    return [clique for clique in nx.enumerate_all_cliques(graph) if len(clique)==3]


def get_shortest_path_lengths(graph):
    n = graph.number_of_nodes()

    shortest_paths = {}
    for vertex in graph:
        shortest_paths[vertex] = nx.algorithms.shortest_paths.generic.shortest_path_length(graph, source=vertex)

    return shortest_paths


def get_shortest_path_averages(graph):
    averages = {}
    for vertex in graph:
        distances = nx.algorithms.shortest_paths.generic.shortest_path_length(graph, source=vertex)
        del distances[vertex]
        averages[vertex] = 0 if not distances else np.mean(list(distances.values()))

    return averages


def get_shortest_path_harmonic_averages(graph):
    averages = {}
    for vertex in graph:
        distances = nx.algorithms.shortest_paths.generic.shortest_path_length(graph, source=vertex)
        del distances[vertex]
        path_lengths = list(distances.values())
        averages[vertex] = 0 if not distances else\
                    sum(map(lambda x: 1/x, path_lengths))/len(path_lengths)

    return averages


def get_neighbourhood_degree_spectra(graph):
    degrees = graph.degree

    neighbourhood_degree_spectra = {}
    for vertex in graph:
        neighbour_degrees = [degrees[neighbour] for neighbour in graph.neighbors(vertex)]

        hist = dict(Counter(neighbour_degrees))
        for key in hist:
            hist[key] /= degrees[vertex]
        neighbourhood_degree_spectra[vertex] = hist

    return neighbourhood_degree_spectra


def get_onion_spectrum(graph):
    onion_layers = nx.algorithms.core.onion_layers(graph)
    kcore = nx.algorithms.core.core_number(graph)
    n = graph.number_of_nodes()

    nb_layers = max(onion_layers.values())
    odlayer2kshell = np.zeros(nb_layers + 1)
    onion_spectrum = np.zeros(nb_layers + 1)

    for vertex in onion_layers:
        odlayer2kshell[ onion_layers[vertex] ] = kcore[vertex]
        onion_spectrum[ onion_layers[vertex] ] += 1

    onion_spect = {}

    for layer in range(1, len(onion_spectrum)):
        if odlayer2kshell[layer-1] < odlayer2kshell[layer]:
          onion_spect[ odlayer2kshell[layer] ] = []
        onion_spect[ odlayer2kshell[layer] ].append(onion_spectrum[layer]/n)

    return onion_spect


def get_clustering_spectrum(graph):
    deg = graph.degree()
    clu = nx.clustering(graph)

    d = [deg[n] for n in graph.nodes()]
    c = [clu[n] for n in graph.nodes()]

    spectrum = {k : x/n for x, k, n in zip(
                     np.histogram(d, bins=np.arange(0.5, np.max(d)+1.5, 1), weights=c)[0],
                     np.arange(1, np.max(d)+2, 1),
                     np.histogram(d, bins=np.arange(0.5, np.max(d)+1.5, 1))[0])
                 if n > 0
               }
    del spectrum[1]
    return spectrum
