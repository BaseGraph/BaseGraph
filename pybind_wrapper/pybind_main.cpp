#include <string>
#include <vector>
#include <sstream>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "pgl/directedgraph.h"
#include "pgl/undirectedgraph.h"
#include "pgl/vertexlabeled_directedgraph.hpp"
#include "pgl/vertexlabeled_undirectedgraph.hpp"

#include "pgl/fileio.h"
#include "pgl/metrics/directed.h"
#include "pgl/metrics/general.h"
#include "pgl/metrics/undirected.h"
#include "pgl/algorithms/graphpaths.h"
#include "pgl/algorithms/percolation.h"
#include "pgl/algorithms/randomgraphs.h"

#include "labeled_graphs.hpp"

namespace py = pybind11;
using namespace PGL;


PYBIND11_MODULE(pgl, m){
    py::class_<DirectedGraph> (m, "DirectedGraph")
        .def(py::init<>())
        .def("get_size", &DirectedGraph::getSize)
        .def("resize", &DirectedGraph::resize, py::arg("size"))
        .def("deep_copy", [](const DirectedGraph& self) {return DirectedGraph(self);})

        .def("add_edge_idx", &DirectedGraph::addEdgeIdx, py::arg("source index"), py::arg("destination index"), py::arg("force")=false)
        .def("add_reciprocal_edge_idx", &DirectedGraph::addEdgeIdx, py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("remove_edge_idx", &DirectedGraph::removeEdgeIdx, py::arg("source index"), py::arg("destination index"))
        .def("is_edge_idx", &DirectedGraph::isEdgeIdx, py::arg("source index"), py::arg("destination index"))
        .def("remove_vertex_from_edgelist_idx", &DirectedGraph::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_multiedges", &DirectedGraph::removeMultiedges)
        .def("remove_self_loops", &DirectedGraph::removeSelfLoops)

        .def("get_edge_number", &DirectedGraph::getEdgeNumber)
        .def("get_out_edges_of_idx", &DirectedGraph::getOutEdgesOfIdx, py::arg("vertex index"))
        .def("get_in_edges_of_vertices", &DirectedGraph::getInEdgesOfVertices)
        .def("get_adjacency_matrix", &DirectedGraph::getAdjacencyMatrix)
        .def("get_in_degree_idx", &DirectedGraph::getInDegreeIdx, py::arg("vertex index"))
        .def("get_in_degrees", &DirectedGraph::getInDegrees)
        .def("get_out_degree_idx", &DirectedGraph::getOutDegreeIdx, py::arg("vertex index"))
        .def("get_out_degrees", &DirectedGraph::getOutDegrees)

        .def("__eq__", [](const DirectedGraph& self, const DirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__", [](const DirectedGraph& self, const DirectedGraph& other) {return self != other;}, py::is_operator())
        .def("__str__", [](const DirectedGraph self) { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__getitem__", [](const DirectedGraph self, size_t idx) { return self.getOutEdgesOfIdx(idx); })
        .def("__len__", [](const DirectedGraph self) { return self.getSize(); });

    py::class_<UndirectedGraph, DirectedGraph> (m, "UndirectedGraph").
        def(py::init<>())

        .def("add_edge_idx", &UndirectedGraph::addEdgeIdx, py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("remove_edge_idx", &UndirectedGraph::removeEdgeIdx, py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_vertex_from_edgelist_idx", &UndirectedGraph::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_multiedges", &UndirectedGraph::removeMultiedges)
        .def("get_neighbours_of_idx", &UndirectedGraph::getNeighboursOfIdx, py::arg("vertex index"))
        .def("get_degree_idx", &UndirectedGraph::getDegreeIdx, py::arg("vertex index"))
        .def("get_degrees", &UndirectedGraph::getDegrees)

        .def("__eq__", [](const UndirectedGraph& self, const UndirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__", [](const UndirectedGraph& self, const UndirectedGraph& other) {return self != other;}, py::is_operator());


    declare_directedgraph<std::string>(m, "Str");
    declare_undirectedgraph<std::string>(m, "Str");
    //declare_undirectedgraph<int>(m, "Int");

    // Read/write graph files
    m.def("write_edgelist_idx_in_text_file", py::overload_cast<const DirectedGraph&, const std::string&, size_t>(&writeEdgeListIdxInTextFile));
    m.def("write_edgelist_idx_in_text_file", py::overload_cast<const DirectedGraph&, std::ofstream&, size_t>(&writeEdgeListIdxInTextFile));
    m.def("write_edgelist_idx_in_binary_file", py::overload_cast<const DirectedGraph&, const std::string&>(&writeEdgeListIdxInBinaryFile));
    m.def("write_edgelist_idx_in_binary_file", py::overload_cast<const DirectedGraph&, std::ofstream&>(&writeEdgeListIdxInBinaryFile));

    m.def("write_edgelist_idx_in_text_file", py::overload_cast<const UndirectedGraph&, const std::string&>(&writeEdgeListIdxInTextFile));
    m.def("write_edgelist_idx_in_text_file", py::overload_cast<const UndirectedGraph&, std::ofstream&>(&writeEdgeListIdxInTextFile));
    m.def("write_edgelist_idx_in_binary_file", py::overload_cast<const UndirectedGraph&, const std::string&>(&writeEdgeListIdxInBinaryFile));
    m.def("write_edgelist_idx_in_binary_file", py::overload_cast<const UndirectedGraph&, std::ofstream&>(&writeEdgeListIdxInBinaryFile));

    m.def("load_directed_edgelist_idx_from_text_file", py::overload_cast<const std::string&>(&loadDirectedEdgeListIdxFromTextFile));
    m.def("load_directed_edgelist_idx_from_text_file", py::overload_cast<std::ifstream&>(&loadDirectedEdgeListIdxFromTextFile));
    m.def("load_directed_edgelist_idx_from_binary_file", py::overload_cast<const std::string&>(&loadDirectedEdgeListIdxFromBinaryFile));
    m.def("load_directed_edgelist_idx_from_binary_file", py::overload_cast<std::ifstream&>(&loadDirectedEdgeListIdxFromBinaryFile));

    m.def("load_undirected_edgelist_idx_from_text_file", py::overload_cast<const std::string&>(&loadUndirectedEdgeListIdxFromTextFile));
    m.def("load_undirected_edgelist_idx_from_text_file", py::overload_cast<std::ifstream&>(&loadUndirectedEdgeListIdxFromTextFile));
    m.def("load_undirected_edgelist_idx_from_binary_file", py::overload_cast<const std::string&>(&loadUndirectedEdgeListIdxFromBinaryFile));
    m.def("load_undirected_edgelist_idx_from_binary_file", py::overload_cast<std::ifstream&>(&loadUndirectedEdgeListIdxFromBinaryFile));

    m.def("load_undirected_edgelist_from_text_file", py::overload_cast<const std::string&>(&loadUndirectedEdgeListFromTextFile));
    m.def("load_undirected_edgelist_from_text_file", py::overload_cast<std::ifstream&>(&loadUndirectedEdgeListFromTextFile));
    m.def("load_directed_edgelist_from_text_file", py::overload_cast<const std::string&>(&loadDirectedEdgeListFromTextFile));
    m.def("load_directed_edgelist_from_text_file", py::overload_cast<std::ifstream&>(&loadDirectedEdgeListFromTextFile));


    // General metrics
    m.def("get_closeness_centrality_of_vertex_idx", &getClosenessCentralityOfVertexIdx);
    m.def("get_harmonic_mean_geodesic_of_vertex_idx", &getHarmonicMeanGeodesicOfVertexIdx);
    m.def("get_betweenesses", &getBetweenesses);
    m.def("get_diameters", &getDiameters);
    m.def("get_average_shortest_paths", &getAverageShortestPaths);
    m.def("get_shortest_paths_distribution", &getShortestPathsDistribution);
    m.def("find_connected_components", &findConnectedComponents);

    // Undirected metrics
    m.def("get_degree_correlation", &getDegreeCorrelation);
    m.def("find_all_triangles", &findAllTriangles);
    m.def("count_triangles_around_vertex_idx", &countTrianglesAroundVertexIdx);
    m.def("count_triangles", &countTriangles);

    m.def("get_local_clustering_coefficients", py::overload_cast<const UndirectedGraph&> (&getLocalClusteringCoefficients));
    m.def("get_global_clustering_coefficient", py::overload_cast<const UndirectedGraph&> (&getGlobalClusteringCoefficient));
    m.def("get_clusering_spectrum", &getClusteringSpectrum);
    m.def("get_redundancy", &getRedundancy);

    m.def("get_kshells_and_onion_layers", &getKShellsAndOnionLayers);
    m.def("get_kshells", &getKShells);
    m.def("get_onion_layers", &getOnionLayers);
    m.def("get_onion_spectrum", py::overload_cast<const UndirectedGraph&> (&getOnionSpectrum));
    m.def("get_kcore", py::overload_cast<const UndirectedGraph&, size_t> (&getKCore));
    m.def("get_neighbourhood_degrees_of_vertex_idx", &getNeighbourhoodDegreesOfVertexIdx);
    m.def("get_neighbourhood_degree_spêctrum", &getNeighbourDegreeSpectrum);

    m.def("get_modularity", &getModularity);

    // Directed metrics
    m.def("get_density", &getDensity);
    m.def("find_all_directed_triangles", py::overload_cast<const DirectedGraph&> (&findAllDirectedTriangles));
    m.def("get_triangle_spectrum", &getTriangleSpectrum);
    m.def("get_undirected_local_clustering_coefficients", py::overload_cast<const DirectedGraph&> (&getUndirectedLocalClusteringCoefficients));
    m.def("get_undirected_global_clustering_coefficient", py::overload_cast<const DirectedGraph&> (&getUndirectedGlobalClusteringCoefficient));

    m.def("get_reciprocal_degrees", &getReciprocalDegrees);
    m.def("get_jaccard_reciprocities", py::overload_cast<const DirectedGraph&> (&getJaccardReciprocities));
    m.def("get_reciprocity_ratios", py::overload_cast<const DirectedGraph&> (&getReciprocityRatios));
    m.def("get_out_degree_histogram", &getOutDegreeHistogram);
    m.def("get_in_degree_histogram", py::overload_cast<const DirectedGraph&> (&getInDegreeHistogram));

    // Path algorithms
    m.def("find_geodesics_of_vertex", &findGeodesicsOfVertex);
    m.def("find_every_geodesics_of_vertex", &findEveryGeodesicsOfVertex);
    m.def("find_path_to_vertex_from_predecessors_idx", py::overload_cast<const DirectedGraph&, size_t,
        const std::pair<std::vector<size_t>, std::vector<size_t> >& > (&findPathToVertexFromPredecessorsIdx));
    m.def("find_multiple_paths_to_vertex_from_predecessors_idx", py::overload_cast<const DirectedGraph&, size_t,
        const std::pair<std::vector<size_t>, std::vector<std::list<size_t> >>& >(&findMultiplePathsToVertexFromPredecessorsIdx) );

    // Random graphs
    //m.def("seed_rng", [&rng](size_t seed) { rng.seed(seed); });
    m.def("generate_erdos_renyi_graph", &generateErdosRenyiGraph);
    m.def("generate_sparse_erdos_renyi_graph", &generateSparseErdosRenyiGraph);
    m.def("generate_graph_with_degree_distribution_stub_matching", &generateGraphWithDegreeDistributionStubMatching);
    m.def("rewire_with_configuration_model", &rewireWithConfigurationModel);
}

