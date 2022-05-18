#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/fileio.h"


namespace py = pybind11;
using namespace BaseGraph;


void defineIOTools(py::module &m) {
    m.def("write_text_edgelist_idx",   py::overload_cast<const DirectedGraph&, const std::string&, size_t>(&writeTextEdgeListIdx),
            py::arg("directed graph"), py::arg("file name"), py::arg("vertex index shift")=0);
    m.def("write_text_edgelist_idx",   py::overload_cast<const UndirectedGraph&, const std::string&>(&writeTextEdgeListIdx),
            py::arg("undirected graph"), py::arg("file name"));
    m.def("write_binary_edgelist_idx", [&](const DirectedGraph& graph, const std::string& fileName) { writeBinaryEdgeListIdx(graph, fileName); },
            py::arg("directed graph"), py::arg("file name"));
    m.def("write_binary_edgelist_idx", [&](const UndirectedGraph& graph, const std::string& fileName) { writeBinaryEdgeListIdx(graph, fileName); },
            py::arg("undirected graph"), py::arg("file name"));

    m.def("load_directed_text_edgelist_idx",   &loadDirectedTextEdgeListIdx, py::arg("file name"));
    m.def("load_undirected_text_edgelist_idx", &loadUndirectedTextEdgeListIdx, py::arg("file name"));
    m.def("load_directed_text_edgelist",   &loadDirectedTextEdgeList, py::arg("file name"));
    m.def("load_undirected_text_edgelist", &loadUndirectedTextEdgeList, py::arg("file name"));

    m.def("load_directed_binary_edgelist_idx",   &loadDirectedBinaryEdgeListIdx, py::arg("file name"));
    m.def("load_undirected_binary_edgelist_idx", &loadUndirectedBinaryEdgeListIdx, py::arg("file name"));
}
