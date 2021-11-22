#include <string>
#include <vector>
#include <sstream>

#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"
#include "BaseGraph/types.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/directed_multigraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/undirected_multigraph.h"

#include "BaseGraph/fileio.h"
#include "BaseGraph/metrics/directed.h"
#include "BaseGraph/metrics/general.h"
#include "BaseGraph/metrics/undirected.h"
#include "BaseGraph/algorithms/graphpaths.h"
#include "BaseGraph/algorithms/percolation.h"
#include "BaseGraph/algorithms/randomgraphs.h"

#include "vertexlabeled_graphs.hpp"
#include "edgelabeled_graphs.hpp"


namespace py = pybind11;
using namespace BaseGraph;


PYBIND11_MODULE(basegraph, m){
    py::module metrics = m.def_submodule("metrics");
    py::module core = m.def_submodule("core");
    py::module io = m.def_submodule("io");
    py::module random = m.def_submodule("random");


    py::class_<DirectedGraph> (core, "DirectedGraph")
        .def(py::init<size_t>(), py::arg("size"))
        .def("resize",          &DirectedGraph::resize, py::arg("size"))
        .def("get_size",        &DirectedGraph::getSize)
        .def("get_edge_number", &DirectedGraph::getEdgeNumber)
        .def("is_directed",     [](const UndirectedGraph &self) { return true; })

        .def("add_edge_idx",            py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedGraph::addEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"), py::arg("force")=false)
        .def("add_reciprocal_edge_idx", py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedGraph::addReciprocalEdgeIdx),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("remove_edge_idx",         py::overload_cast<VertexIndex, VertexIndex>(&DirectedGraph::removeEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"))
        .def("is_edge_idx",             py::overload_cast<VertexIndex, VertexIndex>(&DirectedGraph::isEdgeIdx, py::const_),
                                          py::arg("source index"), py::arg("destination index"))
        .def("remove_vertex_from_edgelist_idx", &DirectedGraph::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_multiedges",       &DirectedGraph::removeMultiedges)
        .def("remove_self_loops",       &DirectedGraph::removeSelfLoops)
        .def("clear_edges",             &DirectedGraph::clearEdges)

        .def("get_out_edges_of_idx",  &DirectedGraph::getOutEdgesOfIdx, py::arg("vertex index"))
        .def("get_in_edges",          &DirectedGraph::getInEdges)
        .def("get_adjacency_matrix",  &DirectedGraph::getAdjacencyMatrix)
        .def("get_in_degree_of_idx",  &DirectedGraph::getInDegreeOfIdx, py::arg("vertex index"))
        .def("get_in_degrees",        &DirectedGraph::getInDegrees)
        .def("get_out_degree_of_idx", &DirectedGraph::getOutDegreeOfIdx, py::arg("vertex index"))
        .def("get_out_degrees",       &DirectedGraph::getOutDegrees)

        .def("get_deep_copy",        [](const DirectedGraph& self) { return DirectedGraph(self); })
        .def("get_undirected_graph", [](const DirectedGraph& self) { return UndirectedGraph(self); })
        .def("get_reversed_graph",   &DirectedGraph::getReversedGraph)
        .def("get_subgraph_of_idx",  [](const DirectedGraph& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOfIdx(vertices.begin(), vertices.end()); },
                                        py::arg("subgraph vertices"))

        .def("__eq__",      [](const DirectedGraph& self, const DirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const DirectedGraph& self, const DirectedGraph& other) {return self != other;}, py::is_operator())
        .def("__getitem__", [](const DirectedGraph self, VertexIndex idx) { return self.getOutEdgesOfIdx(idx); })
        .def("__str__",     [](const DirectedGraph self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const DirectedGraph &self) { return py::make_iterator(self.begin(), self.end()); },
                            py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     [](const DirectedGraph self)  { return self.getSize(); });


    py::class_<UndirectedGraph> (core, "UndirectedGraph")
        .def(py::init<size_t>(), py::arg("size"))
        .def("resize",          &UndirectedGraph::resize, py::arg("size"))
        .def("get_size",        &UndirectedGraph::getSize)
        .def("get_edge_number", &UndirectedGraph::getEdgeNumber)
        .def("is_directed",     [](const UndirectedGraph &self) { return false; })

        .def("add_edge_idx",      py::overload_cast<VertexIndex, VertexIndex, bool> (&UndirectedGraph::addEdgeIdx),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("is_edge_idx",       py::overload_cast<VertexIndex, VertexIndex>(&UndirectedGraph::isEdgeIdx, py::const_),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_edge_idx",   py::overload_cast<VertexIndex, VertexIndex>(&UndirectedGraph::removeEdgeIdx),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_vertex_from_edgelist_idx", &UndirectedGraph::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_multiedges", &UndirectedGraph::removeMultiedges)
        .def("remove_self_loops", &UndirectedGraph::removeSelfLoops)
        .def("clear_edges",       &UndirectedGraph::clearEdges)

        .def("get_neighbours_of_idx", &UndirectedGraph::getNeighboursOfIdx, py::arg("vertex index"))
        .def("get_out_edges_of_idx",  &UndirectedGraph::getNeighboursOfIdx, py::arg("vertex index"))
        .def("get_adjacency_matrix",  &UndirectedGraph::getAdjacencyMatrix)
        .def("get_degree_of_idx",     &UndirectedGraph::getDegreeOfIdx, py::arg("vertex index"), py::arg("with self-loops")=true)
        .def("get_degrees",           &UndirectedGraph::getDegrees, py::arg("with self-loops")=true)

        .def("get_directed_graph",  &UndirectedGraph::getDirectedGraph)
        .def("get_deep_copy",       [](const UndirectedGraph& self) {return UndirectedGraph(self);})
        .def("get_subgraph_of_idx", [](const UndirectedGraph& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOfIdx(vertices.begin(), vertices.end()); },
                                        py::arg("subgraph vertices"))

        .def("__eq__",      [](const UndirectedGraph& self, const UndirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const UndirectedGraph& self, const UndirectedGraph& other) {return self != other;}, py::is_operator())
        .def("__getitem__", [](const UndirectedGraph self, VertexIndex idx) { return self.getOutEdgesOfIdx(idx); })
        .def("__str__",     [](const UndirectedGraph self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const UndirectedGraph &self) { return py::make_iterator(self.begin(), self.end()); },
                              py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     [](const UndirectedGraph self)  { return self.getSize(); });

    declareEdgeLabeledDirectedGraph<EdgeMultiplicity>(core, "UnsignedInt");
    declareEdgeLabeledUndirectedGraph<EdgeMultiplicity>(core, "UnsignedInt");

    py::class_<DirectedMultigraph, EdgeLabeledDirectedGraph<EdgeMultiplicity>> (core, "DirectedMultigraph")
        .def(py::init<size_t>(), py::arg("size"))

        .def("add_edge_idx",              py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedMultigraph::addEdgeIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("force")=false)
        .def("remove_edge_idx",           py::overload_cast<VertexIndex, VertexIndex>(&DirectedMultigraph::removeEdgeIdx),
                                            py::arg("source"), py::arg("destination"))
        .def("add_multiedge_idx",         py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(&DirectedMultigraph::addMultiedgeIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"), py::arg("force")=false)
        .def("remove_multiedge_idx",      py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&DirectedMultigraph::removeMultiedgeIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"))

        .def("get_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex>(&DirectedMultigraph::getEdgeMultiplicityIdx, py::const_),
                                            py::arg("source"), py::arg("destination"))
        .def("set_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&DirectedMultigraph::setEdgeMultiplicityIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"));

    py::class_<UndirectedMultigraph, EdgeLabeledUndirectedGraph<EdgeMultiplicity>> (core, "UndirectedMultigraph")
        .def(py::init<size_t>(), py::arg("size"))

        .def("add_edge_idx",              py::overload_cast<VertexIndex, VertexIndex, bool>(&UndirectedMultigraph::addEdgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("force")=false)
        .def("remove_edge_idx",           py::overload_cast<VertexIndex, VertexIndex>(&UndirectedMultigraph::removeEdgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"))
        .def("add_multiedge_idx",         py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(&UndirectedMultigraph::addMultiedgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"), py::arg("force")=false)
        .def("remove_multiedge_idx",      py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&UndirectedMultigraph::removeMultiedgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"))

        .def("get_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex>(&UndirectedMultigraph::getEdgeMultiplicityIdx, py::const_),
                                            py::arg("vertex1"), py::arg("vertex2"))
        .def("set_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&UndirectedMultigraph::setEdgeMultiplicityIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"));



    declareVertexLabeledDirectedGraph  <std::string, true> (core, "Str");
    declareVertexLabeledUndirectedGraph<std::string, true> (core, "Str");


    // Read/write graph files
    io.def("write_edgelist_idx_in_text_file",   py::overload_cast<const DirectedGraph&, const std::string&, size_t>(&writeEdgeListIdxInTextFile),
            py::arg("directed graph"), py::arg("file name"), py::arg("vertex index shift")=0);
    io.def("write_edgelist_idx_in_text_file",   py::overload_cast<const UndirectedGraph&, const std::string&>(&writeEdgeListIdxInTextFile),
            py::arg("undirected graph"), py::arg("file name"));
    io.def("write_edgelist_idx_in_binary_file", py::overload_cast<const DirectedGraph&, const std::string&>(&writeEdgeListIdxInBinaryFile),
            py::arg("directed graph"), py::arg("file name"));
    io.def("write_edgelist_idx_in_binary_file", py::overload_cast<const UndirectedGraph&, const std::string&>(&writeEdgeListIdxInBinaryFile),
            py::arg("undirected graph"), py::arg("file name"));

    io.def("load_directed_edgelist_idx_from_text_file",   py::overload_cast<const std::string&>(&loadDirectedEdgeListIdxFromTextFile), py::arg("file name"));
    io.def("load_undirected_edgelist_idx_from_text_file", py::overload_cast<const std::string&>(&loadUndirectedEdgeListIdxFromTextFile), py::arg("file name"));
    io.def("load_directed_edgelist_from_text_file",       py::overload_cast<const std::string&>(&loadDirectedEdgeListFromTextFile), py::arg("file name"));
    io.def("load_undirected_edgelist_from_text_file",     py::overload_cast<const std::string&>(&loadUndirectedEdgeListFromTextFile), py::arg("file name"));

    io.def("load_directed_edgelist_idx_from_binary_file",   py::overload_cast<const std::string&>(&loadDirectedEdgeListIdxFromBinaryFile), py::arg("file name"));
    io.def("load_undirected_edgelist_idx_from_binary_file", py::overload_cast<const std::string&>(&loadUndirectedEdgeListIdxFromBinaryFile), py::arg("file name"));


    // General metrics
    metrics.def("get_closeness_centralities",   py::overload_cast<const DirectedGraph&> (&getClosenessCentralities<DirectedGraph>));
    metrics.def("get_closeness_centralities",   py::overload_cast<const UndirectedGraph&> (&getClosenessCentralities<UndirectedGraph>));
    metrics.def("get_harmonic_centralities",    py::overload_cast<const DirectedGraph&> (&getHarmonicCentralities<DirectedGraph>));
    metrics.def("get_harmonic_centralities",    py::overload_cast<const UndirectedGraph&> (&getHarmonicCentralities<UndirectedGraph>));
    metrics.def("get_betweenness_centralities", py::overload_cast<const DirectedGraph&, bool> (&getBetweennessCentralities<DirectedGraph>));
    metrics.def("get_betweenness_centralities", py::overload_cast<const UndirectedGraph&, bool> (&getBetweennessCentralities<UndirectedGraph>));

/**/metrics.def("get_diameters",                       py::overload_cast<const DirectedGraph&> (&getDiameters<DirectedGraph>));
    metrics.def("get_diameters",                       py::overload_cast<const UndirectedGraph&> (&getDiameters<UndirectedGraph>));
    metrics.def("get_shortest_path_averages",          py::overload_cast<const DirectedGraph&> (&getShortestPathAverages<DirectedGraph>));
    metrics.def("get_shortest_path_averages",          py::overload_cast<const UndirectedGraph&> (&getShortestPathAverages<UndirectedGraph>));
    metrics.def("get_shortest_path_harmonic_averages", py::overload_cast<const DirectedGraph&> (&getShortestPathHarmonicAverages<DirectedGraph>));
    metrics.def("get_shortest_path_harmonic_averages", py::overload_cast<const UndirectedGraph&> (&getShortestPathHarmonicAverages<UndirectedGraph>));

/**/metrics.def("get_shortest_paths_distribution", py::overload_cast<const DirectedGraph&> (&getShortestPathsDistribution<DirectedGraph>));
/**/metrics.def("get_shortest_paths_distribution", py::overload_cast<const UndirectedGraph&> (&getShortestPathsDistribution<UndirectedGraph>));
    metrics.def("find_connected_components",       py::overload_cast<const DirectedGraph&> (&findConnectedComponents<DirectedGraph>));
    metrics.def("find_connected_components",       py::overload_cast<const UndirectedGraph&> (&findConnectedComponents<UndirectedGraph>));

    // Undirected metrics
    metrics.def("get_degree_correlation",            py::overload_cast<const UndirectedGraph&>(&getDegreeCorrelation));
/**/metrics.def("find_all_triangles",                &findAllTriangles);
    metrics.def("count_triangles_around_vertex_idx", &countTrianglesAroundVertexIdx);
    metrics.def("count_triangles",                   &countTriangles);

    metrics.def("get_local_clustering_coefficients", py::overload_cast<const UndirectedGraph&> (&getLocalClusteringCoefficients));
    metrics.def("get_global_clustering_coefficient", py::overload_cast<const UndirectedGraph&> (&getGlobalClusteringCoefficient));
    metrics.def("get_clustering_spectrum",           &getClusteringSpectrum);
/**/metrics.def("get_redundancy",                    &getRedundancy);

    metrics.def("get_kshells_and_onion_layers",      &getKShellsAndOnionLayers);
    metrics.def("get_kshells",                       &getKShells);
    metrics.def("get_onion_layers",                  &getOnionLayers);
    metrics.def("get_onion_spectrum",                py::overload_cast<const UndirectedGraph&> (&getOnionSpectrum));
    metrics.def("get_kcore",                         py::overload_cast<const UndirectedGraph&, size_t> (&getKCore));
/**/metrics.def("get_neighbourhood_degrees_of_vertex_idx", &getNeighbourhoodDegreesOfVertexIdx);
/**/metrics.def("get_neighbourhood_degree_spectrum", &getNeighbourDegreeSpectrum);

    metrics.def("get_modularity", &getModularity);

    // Directed metrics
    metrics.def("get_density",                       &getDensity);
/**/metrics.def("find_all_directed_triangles",       py::overload_cast<const DirectedGraph&> (&findAllDirectedTriangles));
/**/metrics.def("get_triangle_spectrum",             &getTriangleSpectrum);
    metrics.def("get_undirected_local_clustering_coefficients", py::overload_cast<const DirectedGraph&> (&getUndirectedLocalClusteringCoefficients));
    metrics.def("get_undirected_global_clustering_coefficient", py::overload_cast<const DirectedGraph&> (&getUndirectedGlobalClusteringCoefficient));

    metrics.def("get_reciprocity",           &getReciprocity);
/**/metrics.def("get_reciprocal_degrees",    &getReciprocalDegrees);
/**/metrics.def("get_jaccard_reciprocities", py::overload_cast<const DirectedGraph&> (&getJaccardReciprocities));
/**/metrics.def("get_reciprocity_ratios",    py::overload_cast<const DirectedGraph&> (&getReciprocityRatios));
/**/metrics.def("get_out_degree_histogram",  &getOutDegreeHistogram);
/**/metrics.def("get_in_degree_histogram",   py::overload_cast<const DirectedGraph&> (&getInDegreeHistogram));

    // Path algorithms
    metrics.def("find_shortest_path_lengths_from_vertex_idx", py::overload_cast<const DirectedGraph&, VertexIndex>(&findShortestPathLengthsFromVertexIdx<DirectedGraph>));
    metrics.def("find_shortest_path_lengths_from_vertex_idx", py::overload_cast<const UndirectedGraph&, VertexIndex>(&findShortestPathLengthsFromVertexIdx<UndirectedGraph>));
    metrics.def("find_geodesics_idx",                 py::overload_cast<const DirectedGraph&, VertexIndex, VertexIndex> (&findGeodesicsIdx<DirectedGraph>));
    metrics.def("find_geodesics_idx",                 py::overload_cast<const UndirectedGraph&, VertexIndex, VertexIndex> (&findGeodesicsIdx<UndirectedGraph>));
    metrics.def("find_all_geodesics_idx",             py::overload_cast<const DirectedGraph&, VertexIndex, VertexIndex> (&findAllGeodesicsIdx<DirectedGraph>));
    metrics.def("find_all_geodesics_idx",             py::overload_cast<const UndirectedGraph&, VertexIndex, VertexIndex> (&findAllGeodesicsIdx<UndirectedGraph>));
    metrics.def("find_geodesics_from_vertex_idx",     py::overload_cast<const DirectedGraph&, VertexIndex> (&findGeodesicsFromVertexIdx<DirectedGraph>));
    metrics.def("find_geodesics_from_vertex_idx",     py::overload_cast<const UndirectedGraph&, VertexIndex> (&findGeodesicsFromVertexIdx<UndirectedGraph>));
    metrics.def("find_all_geodesics_from_vertex_idx", py::overload_cast<const DirectedGraph&, VertexIndex> (&findAllGeodesicsFromVertexIdx<DirectedGraph>));
    metrics.def("find_all_geodesics_from_vertex_idx", py::overload_cast<const UndirectedGraph&, VertexIndex> (&findAllGeodesicsFromVertexIdx<UndirectedGraph>));

    // Random graphs
    random.def("seed", &seed);
    random.def("generate_gilbert_random_graph",          &generateGilbertRandomGraph);
    random.def("generate_erdos_renyi_random_graph",      &generateErdosRenyiRandomGraph);
    random.def("generate_small_world_random_graph",      &generateSmallWorldRandomGraph);
    random.def("generate_graph_with_degree_distribution_stub_matching", &generateGraphWithDegreeDistributionStubMatching);
    random.def("get_edge_list_of_graph",                 &getEdgeVectorOfGraph);
    random.def("shuffle_graph_with_configuration_model", py::overload_cast<UndirectedGraph&, size_t> (&shuffleGraphWithConfigurationModel));
    random.def("shuffle_graph_with_configuration_model", py::overload_cast<UndirectedGraph&, std::vector<std::pair<VertexIndex, VertexIndex>>&, size_t> (&shuffleGraphWithConfigurationModel));
}
