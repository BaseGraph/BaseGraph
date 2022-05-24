#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/fileio.h"


namespace py = pybind11;
using namespace BaseGraph;


void defineIOTools(py::module &m) {
    m.def("write_text_edgelist_idx",   py::overload_cast<const DirectedGraph&, const std::string&, size_t>(&io::writeTextEdgeListIdx),
            py::arg("directed graph"), py::arg("file name"), py::arg("vertex index shift")=0);
    m.def("write_text_edgelist_idx",   py::overload_cast<const UndirectedGraph&, const std::string&>(&io::writeTextEdgeListIdx),
            py::arg("undirected graph"), py::arg("file name"));
    m.def("write_binary_edgelist_idx", [&](const DirectedGraph& graph, const std::string& fileName) { io::writeBinaryEdgeListIdx(graph, fileName); },
            py::arg("directed graph"), py::arg("file name"));
    m.def("write_binary_edgelist_idx", [&](const UndirectedGraph& graph, const std::string& fileName) { io::writeBinaryEdgeListIdx(graph, fileName); },
            py::arg("undirected graph"), py::arg("file name"));

    m.def("load_directed_text_edgelist_idx",   &io::loadDirectedTextEdgeListIdx, py::arg("file name"));
    m.def("load_undirected_text_edgelist_idx", &io::loadUndirectedTextEdgeListIdx, py::arg("file name"));
    m.def("load_directed_text_edgelist",   &io::loadDirectedTextEdgeList, py::arg("file name"));
    m.def("load_undirected_text_edgelist", &io::loadUndirectedTextEdgeList, py::arg("file name"));

    m.def("load_directed_binary_edgelist_idx",   &io::loadDirectedBinaryEdgeListIdx, py::arg("file name"));
    m.def("load_undirected_binary_edgelist_idx", &io::loadUndirectedBinaryEdgeListIdx, py::arg("file name"));
}
