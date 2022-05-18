#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/algorithms/randomgraphs.h"
#include "BaseGraph/algorithms/percolation.h"


namespace py = pybind11;
using namespace BaseGraph;


void defineRandomTools(py::module &m) {
    m.def("seed", &seed);

    m.def("generate_gilbert_random_graph",          &generateGilbertRandomGraph);
    m.def("generate_erdos_renyi_random_graph",      &generateErdosRenyiRandomGraph);
    m.def("generate_small_world_random_graph",      &generateSmallWorldRandomGraph);
    m.def("generate_graph_with_degree_distribution_stub_matching", &generateGraphWithDegreeDistributionStubMatching);
    m.def("get_edge_list_of_graph",                 &getEdgeVectorOfGraph);
    m.def("shuffle_graph_with_configuration_model", py::overload_cast<UndirectedGraph&, size_t> (&shuffleGraphWithConfigurationModel));
    m.def("shuffle_graph_with_configuration_model", py::overload_cast<UndirectedGraph&, std::vector<std::pair<VertexIndex, VertexIndex>>&, size_t> (&shuffleGraphWithConfigurationModel));
}
