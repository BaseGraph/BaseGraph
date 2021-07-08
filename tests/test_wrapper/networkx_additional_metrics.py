import networkx as nx
import numpy as np


def get_networkx_shortestpaths_handler(values, default):
    def f(key):
        try:
            return values[key]
        except KeyError:
            return default
    return f

def get_shortest_paths(graph, vertex_labels):
    n = graph.number_of_nodes()

    shortest_paths = []
    for vertex_label in vertex_labels:
        distances = nx.algorithms.shortest_paths.generic.shortest_path_length(graph, source=vertex_label)
        ordered_distances = list(map(get_networkx_shortestpaths_handler(distances, -1), vertex_labels))
        shortest_paths.append(ordered_distances)

    return shortest_paths

def get_shortest_path_averages(graph, vertex_labels):
    n = graph.number_of_nodes()

    averages = []
    for vertex_label in vertex_labels:
        distances = nx.algorithms.shortest_paths.generic.shortest_path_length(graph, source=vertex_label)
        nonzero_distances = list(filter( lambda x: 0<x<n, distances.values() ))
        
        if nonzero_distances == []:
            averages.append(0)
        else:
            averages.append(np.mean(nonzero_distances))

    return averages

def get_shortest_path_harmonic_averages(graph, vertex_labels):
    n = graph.number_of_nodes()

    harmonic_averages = []
    for vertex_label in vertex_labels:
        distances = nx.algorithms.shortest_paths.generic.shortest_path_length(graph, source=vertex_label)
        nonzero_distances = list(filter( lambda x: 0<x<n, distances.values() ))
        
        if nonzero_distances == []:
            harmonic_averages.append(0)
        else:
            harmonic_averages.append(sum(map(lambda x: 1/x, nonzero_distances))/len(nonzero_distances))

    return harmonic_averages


def get_onion_spectrum(graph, vertices):
    layer = nx.algorithms.core.onion_layers(graph)
    kcore = nx.algorithms.core.core_number(graph)

    nb_layers = max(layer.values())
    odlayer2kshell = np.zeros(nb_layers + 1)
    onion_spectrum = np.zeros(nb_layers + 1)

    for n in layer:
        odlayer2kshell[ layer[n] ] = kcore[n]
        onion_spectrum[ layer[n] ] += 1

    onion_spect = {}

    for o in range(1, len(onion_spectrum)):
        if odlayer2kshell[o-1] < odlayer2kshell[o]:
          onion_spect[ odlayer2kshell[o] ] = []
        onion_spect[ odlayer2kshell[o] ].append(onion_spectrum[o] / graph.number_of_nodes() )

    return onion_spect

def get_clustering_spectrum(graph):
    deg = graph.degree()
    clu = nx.clustering(graph)

    d = [deg[n] for n in graph.nodes()]
    c = [clu[n] for n in graph.nodes()]

    spectrum = {k : x / n for x,k,n in zip(
                 np.histogram(d, bins=np.arange(0.5, np.max(d)+1.5, 1), weights=c)[0],
                 np.arange(1, np.max(d)+2, 1),
                 np.histogram(d, bins=np.arange(0.5, np.max(d)+1.5, 1))[0]) if n > 0}
    del spectrum[1]
    return spectrum
