#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/fileio.h"


namespace py = pybind11;
using namespace BaseGraph;


void defineIOTools(py::module &m) {
    m.def("write_text_edgelist",   py::overload_cast<const DirectedGraph&, const std::string&, size_t>(&io::writeTextEdgeList),
            py::arg("directed graph"), py::arg("file name"), py::arg("vertex index shift")=0);
    m.def("write_text_edgelist",   py::overload_cast<const UndirectedGraph&, const std::string&>(&io::writeTextEdgeList),
            py::arg("undirected graph"), py::arg("file name"));
    m.def("write_binary_edgelist", [&](const DirectedGraph& graph, const std::string& fileName) { io::writeBinaryEdgeList(graph, fileName); },
            py::arg("directed graph"), py::arg("file name"));
    m.def("write_binary_edgelist", [&](const UndirectedGraph& graph, const std::string& fileName) { io::writeBinaryEdgeList(graph, fileName); },
            py::arg("undirected graph"), py::arg("file name"));


    m.def("load_directed_text_edgelist", [](const std::string& fileName) {
                return io::loadDirectedTextEdgeList(fileName, io::VertexCountMapper());
            },
            py::arg("file name"));
    m.def("load_undirected_text_edgelist",  [](const std::string& fileName) {
                return io::loadUndirectedTextEdgeList(fileName, io::VertexCountMapper());
            },
            py::arg("file name"));

    m.def("load_directed_text_edgelist_index", [](const std::string& fileName) {
                return io::loadDirectedTextEdgeList(fileName).first;
            },
            py::arg("file name"));
    m.def("load_undirected_text_edgelist_index",  [](const std::string& fileName) {
                return io::loadUndirectedTextEdgeList(fileName).first;
            },
            py::arg("file name"));


    // Would ideally bind the functions for more flexibility
    //m.def("vertex_counter", &io::vertexCounter);
    //m.def("load_directed_text_edgelist",   &io::loadDirectedTextEdgeList,
            //py::arg("file name"), py::arg("get vertex function")=[](const std::string& s) { return std::stoi(s); });
    //m.def("load_undirected_text_edgelist", &io::loadUndirectedTextEdgeList,
            //py::arg("file name"), py::arg("get vertex function")=[](const std::string& s) { return std::stoi(s); });

    m.def("load_directed_binary_edgelist",   &io::loadDirectedBinaryEdgeList, py::arg("file name"));
    m.def("load_undirected_binary_edgelist", &io::loadUndirectedBinaryEdgeList, py::arg("file name"));
}
