#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/metrics/directed.h"
#include "BaseGraph/metrics/general.h"
#include "BaseGraph/metrics/undirected.h"
#include "BaseGraph/algorithms/graphpaths.h"


namespace py = pybind11;
using namespace BaseGraph;


// Metrics that aren't validatted with Networkx are tagged with /**/
void defineMetrics(py::module &m) {

    // General metrics
    m.def("get_closeness_centralities",   py::overload_cast<const DirectedGraph&> (&getClosenessCentralities<DirectedGraph>));
    m.def("get_closeness_centralities",   py::overload_cast<const UndirectedGraph&> (&getClosenessCentralities<UndirectedGraph>));
    m.def("get_harmonic_centralities",    py::overload_cast<const DirectedGraph&> (&getHarmonicCentralities<DirectedGraph>));
    m.def("get_harmonic_centralities",    py::overload_cast<const UndirectedGraph&> (&getHarmonicCentralities<UndirectedGraph>));
    m.def("get_betweenness_centralities", py::overload_cast<const DirectedGraph&, bool> (&getBetweennessCentralities<DirectedGraph>));
    m.def("get_betweenness_centralities", py::overload_cast<const UndirectedGraph&, bool> (&getBetweennessCentralities<UndirectedGraph>));

/**/m.def("get_diameters",                       py::overload_cast<const DirectedGraph&> (&getDiameters<DirectedGraph>));
    m.def("get_diameters",                       py::overload_cast<const UndirectedGraph&> (&getDiameters<UndirectedGraph>));
    m.def("get_shortest_path_averages",          py::overload_cast<const DirectedGraph&> (&getShortestPathAverages<DirectedGraph>));
    m.def("get_shortest_path_averages",          py::overload_cast<const UndirectedGraph&> (&getShortestPathAverages<UndirectedGraph>));
    m.def("get_shortest_path_harmonic_averages", py::overload_cast<const DirectedGraph&> (&getShortestPathHarmonicAverages<DirectedGraph>));
    m.def("get_shortest_path_harmonic_averages", py::overload_cast<const UndirectedGraph&> (&getShortestPathHarmonicAverages<UndirectedGraph>));

/**/m.def("get_shortest_paths_distribution", py::overload_cast<const DirectedGraph&> (&getShortestPathsDistribution<DirectedGraph>));
/**/m.def("get_shortest_paths_distribution", py::overload_cast<const UndirectedGraph&> (&getShortestPathsDistribution<UndirectedGraph>));
    m.def("find_connected_components",       py::overload_cast<const DirectedGraph&> (&findConnectedComponents<DirectedGraph>));
    m.def("find_connected_components",       py::overload_cast<const UndirectedGraph&> (&findConnectedComponents<UndirectedGraph>));


    // Undirected metrics
    m.def("get_degree_correlation",            py::overload_cast<const UndirectedGraph&>(&getDegreeCorrelation));
/**/m.def("find_all_triangles",                &findAllTriangles);
    m.def("count_triangles_around_vertex_idx", &countTrianglesAroundVertexIdx);
    m.def("count_triangles",                   &countTriangles);

    m.def("get_local_clustering_coefficients", py::overload_cast<const UndirectedGraph&> (&getLocalClusteringCoefficients));
    m.def("get_global_clustering_coefficient", py::overload_cast<const UndirectedGraph&> (&getGlobalClusteringCoefficient));
    m.def("get_clustering_spectrum",           &getClusteringSpectrum);
/**/m.def("get_redundancy",                    &getRedundancy);

    m.def("get_kshells_and_onion_layers",      &getKShellsAndOnionLayers);
    m.def("get_kshells",                       &getKShells);
    m.def("get_onion_layers",                  &getOnionLayers);
    m.def("get_onion_spectrum",                py::overload_cast<const UndirectedGraph&> (&getOnionSpectrum));
    m.def("get_kcore",                         py::overload_cast<const UndirectedGraph&, size_t> (&getKCore));
/**/m.def("get_neighbourhood_degrees_of_vertex_idx", &getNeighbourhoodDegreesOfVertexIdx);
/**/m.def("get_neighbourhood_degree_spectrum", &getNeighbourDegreeSpectrum);

    m.def("get_modularity", &getModularity);


    // Directed metrics
    m.def("get_density",                       &getDensity);
/**/m.def("find_all_directed_triangles",       py::overload_cast<const DirectedGraph&> (&findAllDirectedTriangles));
/**/m.def("get_triangle_spectrum",             &getTriangleSpectrum);
    m.def("get_undirected_local_clustering_coefficients", py::overload_cast<const DirectedGraph&> (&getUndirectedLocalClusteringCoefficients));
    m.def("get_undirected_global_clustering_coefficient", py::overload_cast<const DirectedGraph&> (&getUndirectedGlobalClusteringCoefficient));

    m.def("get_reciprocity",           &getReciprocity);
/**/m.def("get_reciprocal_degrees",    &getReciprocalDegrees);
/**/m.def("get_jaccard_reciprocities", py::overload_cast<const DirectedGraph&> (&getJaccardReciprocities));
/**/m.def("get_reciprocity_ratios",    py::overload_cast<const DirectedGraph&> (&getReciprocityRatios));
/**/m.def("get_out_degree_histogram",  &getOutDegreeHistogram);
/**/m.def("get_in_degree_histogram",   py::overload_cast<const DirectedGraph&> (&getInDegreeHistogram));


    // Path algorithms
    m.def("find_shortest_path_lengths_from_vertex_idx", py::overload_cast<const DirectedGraph&, VertexIndex>(&findShortestPathLengthsFromVertexIdx<DirectedGraph>));
    m.def("find_shortest_path_lengths_from_vertex_idx", py::overload_cast<const UndirectedGraph&, VertexIndex>(&findShortestPathLengthsFromVertexIdx<UndirectedGraph>));
    m.def("find_geodesics_idx",                 py::overload_cast<const DirectedGraph&, VertexIndex, VertexIndex> (&findGeodesicsIdx<DirectedGraph>));
    m.def("find_geodesics_idx",                 py::overload_cast<const UndirectedGraph&, VertexIndex, VertexIndex> (&findGeodesicsIdx<UndirectedGraph>));
    m.def("find_all_geodesics_idx",             py::overload_cast<const DirectedGraph&, VertexIndex, VertexIndex> (&findAllGeodesicsIdx<DirectedGraph>));
    m.def("find_all_geodesics_idx",             py::overload_cast<const UndirectedGraph&, VertexIndex, VertexIndex> (&findAllGeodesicsIdx<UndirectedGraph>));
    m.def("find_geodesics_from_vertex_idx",     py::overload_cast<const DirectedGraph&, VertexIndex> (&findGeodesicsFromVertexIdx<DirectedGraph>));
    m.def("find_geodesics_from_vertex_idx",     py::overload_cast<const UndirectedGraph&, VertexIndex> (&findGeodesicsFromVertexIdx<UndirectedGraph>));
    m.def("find_all_geodesics_from_vertex_idx", py::overload_cast<const DirectedGraph&, VertexIndex> (&findAllGeodesicsFromVertexIdx<DirectedGraph>));
    m.def("find_all_geodesics_from_vertex_idx", py::overload_cast<const UndirectedGraph&, VertexIndex> (&findAllGeodesicsFromVertexIdx<UndirectedGraph>));

}
