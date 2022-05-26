#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/extensions/metrics/directed.h"
#include "BaseGraph/extensions/metrics/general.h"
#include "BaseGraph/extensions/metrics/undirected.h"
#include "BaseGraph/algorithms/graphpaths.h"


namespace py = pybind11;
using namespace BaseGraph;


// Metrics that aren't validated with Networkx are tagged with /**/
PYBIND11_MODULE(_metrics, m) {
    // General metrics
    m.def("get_closeness_centralities",   py::overload_cast<const DirectedGraph&> (&metrics::getClosenessCentralities<DirectedGraph>));
    m.def("get_closeness_centralities",   py::overload_cast<const UndirectedGraph&> (&metrics::getClosenessCentralities<UndirectedGraph>));
    m.def("get_harmonic_centralities",    py::overload_cast<const DirectedGraph&> (&metrics::getHarmonicCentralities<DirectedGraph>));
    m.def("get_harmonic_centralities",    py::overload_cast<const UndirectedGraph&> (&metrics::getHarmonicCentralities<UndirectedGraph>));
    m.def("get_betweenness_centralities", py::overload_cast<const DirectedGraph&, bool> (&metrics::getBetweennessCentralities<DirectedGraph>));
    m.def("get_betweenness_centralities", py::overload_cast<const UndirectedGraph&, bool> (&metrics::getBetweennessCentralities<UndirectedGraph>));

    m.def("get_shortest_path_lengths_from_vertex_idx", py::overload_cast<const DirectedGraph&, VertexIndex>(&metrics::getShortestPathLengthsFromVertexIdx<DirectedGraph>));
    m.def("get_shortest_path_lengths_from_vertex_idx", py::overload_cast<const UndirectedGraph&, VertexIndex>(&metrics::getShortestPathLengthsFromVertexIdx<UndirectedGraph>));
/**/m.def("get_diameters",                       py::overload_cast<const DirectedGraph&> (&metrics::getDiameters<DirectedGraph>));
/**/m.def("get_diameters",                       py::overload_cast<const UndirectedGraph&> (&metrics::getDiameters<UndirectedGraph>));
    m.def("get_shortest_path_averages",          py::overload_cast<const DirectedGraph&> (&metrics::getShortestPathAverages<DirectedGraph>));
    m.def("get_shortest_path_averages",          py::overload_cast<const UndirectedGraph&> (&metrics::getShortestPathAverages<UndirectedGraph>));
    m.def("get_shortest_path_harmonic_averages", py::overload_cast<const DirectedGraph&> (&metrics::getShortestPathHarmonicAverages<DirectedGraph>));
    m.def("get_shortest_path_harmonic_averages", py::overload_cast<const UndirectedGraph&> (&metrics::getShortestPathHarmonicAverages<UndirectedGraph>));

/**/m.def("get_shortest_paths_distribution", py::overload_cast<const DirectedGraph&> (&metrics::getShortestPathsDistribution<DirectedGraph>));
/**/m.def("get_shortest_paths_distribution", py::overload_cast<const UndirectedGraph&> (&metrics::getShortestPathsDistribution<UndirectedGraph>));
/**/m.def("find_connected_components",       py::overload_cast<const DirectedGraph&> (&metrics::findConnectedComponents<DirectedGraph>));
    m.def("find_connected_components",       py::overload_cast<const UndirectedGraph&> (&metrics::findConnectedComponents<UndirectedGraph>));


    // Undirected metrics
    m.def("get_degree_correlation",            py::overload_cast<const UndirectedGraph&>(&metrics::getDegreeCorrelation));
    m.def("find_all_triangles",                &metrics::findAllTriangles);
    m.def("count_triangles_around_vertex_idx", &metrics::countTrianglesAroundVertexIdx);
    m.def("count_triangles",                   &metrics::countTriangles);

    m.def("get_local_clustering_coefficients", py::overload_cast<const UndirectedGraph&> (&metrics::getLocalClusteringCoefficients));
    m.def("get_global_clustering_coefficient", py::overload_cast<const UndirectedGraph&> (&metrics::getGlobalClusteringCoefficient));
    m.def("get_clustering_spectrum",           &metrics::getClusteringSpectrum);
    m.def("get_redundancy",                    &metrics::getRedundancy);

    m.def("get_kshells_and_onion_layers",      &metrics::getKShellsAndOnionLayers);
    m.def("get_kshells",                       &metrics::getKShells);
    m.def("get_onion_layers",                  &metrics::getOnionLayers);
    m.def("get_onion_spectrum",                py::overload_cast<const UndirectedGraph&> (&metrics::getOnionSpectrum));
/**/m.def("get_kcore",                         py::overload_cast<const UndirectedGraph&, size_t> (&metrics::getKCore));
    m.def("get_neighbourhood_degrees_of_vertex_idx", &metrics::getNeighbourhoodDegreesOfVertexIdx);
    m.def("get_neighbourhood_degree_spectrum", &metrics::getNeighbourDegreeSpectrum);

    m.def("get_modularity", &metrics::getModularity);


    // Directed metrics
    m.def("get_density",                       &metrics::getDensity);
/**/m.def("find_all_directed_triangles",       py::overload_cast<const DirectedGraph&> (&metrics::findAllDirectedTriangles));
/**/m.def("get_triangle_spectrum",             &metrics::getTriangleSpectrum);
    m.def("get_undirected_local_clustering_coefficients", py::overload_cast<const DirectedGraph&> (&metrics::getUndirectedLocalClusteringCoefficients));
    m.def("get_undirected_global_clustering_coefficient", py::overload_cast<const DirectedGraph&> (&metrics::getUndirectedGlobalClusteringCoefficient));

    m.def("get_reciprocity",           &metrics::getReciprocity);
/**/m.def("get_reciprocal_degrees",    &metrics::getReciprocalDegrees);
/**/m.def("get_jaccard_reciprocities", py::overload_cast<const DirectedGraph&> (&metrics::getJaccardReciprocities));
/**/m.def("get_reciprocity_ratios",    py::overload_cast<const DirectedGraph&> (&metrics::getReciprocityRatios));
/**/m.def("get_out_degree_histogram",  &metrics::getOutDegreeHistogram);
/**/m.def("get_in_degree_histogram",   py::overload_cast<const DirectedGraph&> (&metrics::getInDegreeHistogram));


    // Path algorithms
    m.def("find_geodesics_idx",                 py::overload_cast<const DirectedGraph&, VertexIndex, VertexIndex> (&algorithms::findGeodesicsIdx<DirectedGraph>));
    m.def("find_geodesics_idx",                 py::overload_cast<const UndirectedGraph&, VertexIndex, VertexIndex> (&algorithms::findGeodesicsIdx<UndirectedGraph>));
    m.def("find_all_geodesics_idx",             py::overload_cast<const DirectedGraph&, VertexIndex, VertexIndex> (&algorithms::findAllGeodesicsIdx<DirectedGraph>));
    m.def("find_all_geodesics_idx",             py::overload_cast<const UndirectedGraph&, VertexIndex, VertexIndex> (&algorithms::findAllGeodesicsIdx<UndirectedGraph>));
    m.def("find_geodesics_from_vertex_idx",     py::overload_cast<const DirectedGraph&, VertexIndex> (&algorithms::findGeodesicsFromVertexIdx<DirectedGraph>));
    m.def("find_geodesics_from_vertex_idx",     py::overload_cast<const UndirectedGraph&, VertexIndex> (&algorithms::findGeodesicsFromVertexIdx<UndirectedGraph>));
    m.def("find_all_geodesics_from_vertex_idx", py::overload_cast<const DirectedGraph&, VertexIndex> (&algorithms::findAllGeodesicsFromVertexIdx<DirectedGraph>));
    m.def("find_all_geodesics_from_vertex_idx", py::overload_cast<const UndirectedGraph&, VertexIndex> (&algorithms::findAllGeodesicsFromVertexIdx<UndirectedGraph>));

}
