#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/fileio.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

namespace py = pybind11;
using namespace BaseGraph;

template <template <class...> class Graph, typename EdgeLabel>
void defineTextFileIO(py::module &m, const std::string &typestr) {
    using Class = Graph<EdgeLabel>;
    m.def("write_text_edgelist", &io::writeTextEdgeList<Graph, EdgeLabel>,
          py::arg("graph"), py::arg("file name"),
          py::arg("func label->string"));
    m.def(std::string("load_" + typestr + "_text_edgelist").c_str(),
          &io::writeTextEdgeList<Graph, EdgeLabel>, py::arg("file name"),
          py::arg("graph size"), py::arg("func string->label"));
}

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<!std::is_same<EdgeLabel, NoLabel>::value>::type
defineBinaryFileIO(py::module &m, const std::string &typestr) {
    m.def("write_binary_edgelist", &io::writeBinaryEdgeList<Graph, EdgeLabel>,
          py::arg("graph"), py::arg("file name"),
          py::arg("func label->binary"));
    m.def(std::string("load_" + typestr + "_binary_edgelist").c_str(),
          &io::loadBinaryEdgeList<Graph, EdgeLabel>, py::arg("file name"),
          py::arg("func binary->label"));
}

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<std::is_same<EdgeLabel, NoLabel>::value>::type
defineBinaryFileIO(py::module &m, const std::string &typestr) {
    m.def("write_binary_edgelist", &io::writeBinaryEdgeList<Graph, EdgeLabel>,
          py::arg("graph"), py::arg("file name"));
    m.def(std::string("load_" + typestr + "_binary_edgelist").c_str(),
          &io::loadBinaryEdgeList<Graph, EdgeLabel>, py::arg("file name"));
}

template <typename EdgeLabel>
void defineFileIODirectedUndirected(py::module &m, const std::string &typestr) {
    defineTextFileIO<LabeledDirectedGraph, EdgeLabel>(m, std::string("directed")+typestr);
    defineTextFileIO<LabeledUndirectedGraph, EdgeLabel>(m, std::string("undirected")+typestr);
    defineBinaryFileIO<LabeledDirectedGraph, EdgeLabel>(m, std::string("directed")+typestr);
    defineBinaryFileIO<LabeledUndirectedGraph, EdgeLabel>(m, std::string("undirected")+typestr);
}

void defineIOTools(py::module &m) {
    defineFileIODirectedUndirected<size_t>(m, "_uint");
    defineFileIODirectedUndirected<NoLabel>(m, "");
}
