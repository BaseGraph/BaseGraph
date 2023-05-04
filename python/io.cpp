#include <functional>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/fileio.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

namespace py = pybind11;
using namespace BaseGraph;

template <typename EdgeLabel> std::string toString(const EdgeLabel &label) {
    return std::to_string(label);
}
template <> std::string toString(const NoLabel &) { return std::string(); }

template <typename EdgeLabel> EdgeLabel fromString(const std::string &str) {
    return EdgeLabel();
}

template <template <class...> class Graph, typename EdgeLabel>
void defineTextFileIO(py::module &m, const std::string &typestr) {
    using Class = Graph<EdgeLabel>;
    m.def(
        "write_text_edgelist",
        [](const Graph<EdgeLabel> &graph, const std::string &fileName) {
            io::writeTextEdgeList<Graph, EdgeLabel>(graph, fileName,
                                                    toString<EdgeLabel>);
        },
        py::arg("graph"), py::arg("file name"));
    m.def(
        std::string("load_" + typestr + "_text_edgelist_indexed").c_str(),
        [](const std::string &fileName) {
            return io::loadTextEdgeList<Graph, EdgeLabel>(
                fileName, fromString<EdgeLabel>);
        },
        py::arg("file name"));
    m.def(
        std::string("load_" + typestr + "_text_edgelist_labeled").c_str(),
        [](const std::string &fileName) {
            return io::loadTextVertexLabeledEdgeList<Graph, EdgeLabel>(
                fileName, fromString<EdgeLabel>);
        },
        py::arg("file name"));
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
    defineTextFileIO<LabeledDirectedGraph, EdgeLabel>(
        m, std::string("directed") + typestr);
    defineTextFileIO<LabeledUndirectedGraph, EdgeLabel>(
        m, std::string("undirected") + typestr);
    defineBinaryFileIO<LabeledDirectedGraph, EdgeLabel>(
        m, std::string("directed") + typestr);
    defineBinaryFileIO<LabeledUndirectedGraph, EdgeLabel>(
        m, std::string("undirected") + typestr);
}

void defineIOTools(py::module &m) {
    defineFileIODirectedUndirected<size_t>(m, "_uint");
    defineFileIODirectedUndirected<NoLabel>(m, "");
}
