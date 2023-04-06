#ifndef BASE_GRAPH_FILEIO_H
#define BASE_GRAPH_FILEIO_H

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirectedgraph.h"

namespace BaseGraph {
namespace io {

class VertexCountMapper {
    size_t i = 0;
    std::unordered_map<std::string, VertexIndex> labels;

  public:
    VertexIndex operator()(const std::string &s) {
        if (labels.count(s) == 0)
            labels[s] = i++;

        return labels.at(s);
    }
};

void writeTextEdgeList(const DirectedGraph &graph, const std::string &fileName,
                       size_t vertexIndexShift = 0);
void writeBinaryEdgeList(const DirectedGraph &graph,
                         const std::string &fileName);
template <typename EdgeLabel>
void writeBinaryEdgeList(const EdgeLabeledDirectedGraph<EdgeLabel> &graph,
                         const std::string &fileName);

void writeTextEdgeList(const UndirectedGraph &graph,
                       const std::string &fileName);
void writeBinaryEdgeList(const UndirectedGraph &graph,
                         const std::string &fileName);
template <typename EdgeLabel>
void writeBinaryEdgeList(const EdgeLabeledUndirectedGraph<EdgeLabel> &graph,
                         const std::string &fileName);

std::pair<DirectedGraph, std::vector<std::string>> loadDirectedTextEdgeList(
    const std::string &fileName,
    std::function<VertexIndex(const std::string &)> getVertex =
        [](const std::string &s) { return stoi(s); });
DirectedGraph loadDirectedBinaryEdgeList(const std::string &fileName);
template <typename EdgeLabel>
EdgeLabeledDirectedGraph<EdgeLabel>
loadLabeledDirectedBinaryEdgeList(const std::string &fileName);

std::pair<UndirectedGraph, std::vector<std::string>> loadUndirectedTextEdgeList(
    const std::string &fileName,
    std::function<VertexIndex(const std::string &)> getVertex =
        [](const std::string &s) { return stoi(s); });
UndirectedGraph loadUndirectedBinaryEdgeList(const std::string &fileName);
template <typename EdgeLabel>
EdgeLabeledUndirectedGraph<EdgeLabel>
loadLabeledUndirectedBinaryEdgeList(const std::string &fileName);

template <typename T>
inline void verifyStreamOpened(const T &fileStream,
                               const std::string &fileName) {
    if (!fileStream.is_open())
        throw std::runtime_error("Could not open file \"" + fileName + "\".");
}

// From https://mklimenko.github.io/english/2018/08/22/robust-endian-swap/
template <typename T> void swapBytes(T &val) {
    union U {
        T val;
        std::array<std::uint8_t, sizeof(T)> raw;
    } src, dst;

    src.val = val;
    std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
    val = dst.val;
}

static bool _isSystemBigEndian() {
    union {
        uint32_t i32;
        uint8_t i8[4];
    } integer = {0x01020304};
    return integer.i8[0] == 1;
}
const bool SYSTEM_IS_BIG_ENDIAN = _isSystemBigEndian();

template <typename T>
void writeBinaryValue(std::ofstream &fileStream, T value) {
    if (SYSTEM_IS_BIG_ENDIAN)
        swapBytes(value);
    fileStream.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

template <typename T>
std::ifstream &readBinaryValue(std::ifstream &fileStream, T &value) {
    fileStream.read(reinterpret_cast<char *>(&value), sizeof(T));
    if (SYSTEM_IS_BIG_ENDIAN && fileStream)
        swapBytes(value);
    return fileStream;
}

// EdgeLabeled Graphs

template <typename EdgeLabel>
void writeBinaryEdgeList(const EdgeLabeledDirectedGraph<EdgeLabel> &graph,
                         const std::string &fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value,
                  "No implementation of string to write binary file");

    std::ofstream fileStream(fileName.c_str(),
                             std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    for (auto vertex : graph)
        for (auto neighbour : graph.getOutEdgesOf(vertex)) {
            writeBinaryValue(fileStream, vertex);
            writeBinaryValue(fileStream, neighbour);
            writeBinaryValue(fileStream,
                             graph.getEdgeLabelOf(vertex, neighbour));
        }
}

template <typename EdgeLabel>
void writeBinaryEdgeList(const EdgeLabeledUndirectedGraph<EdgeLabel> &graph,
                         const std::string &fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value,
                  "No implementation of string to write binary file");

    std::ofstream fileStream(fileName.c_str(),
                             std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    for (auto vertex : graph)
        for (auto neighbour : graph.getOutEdgesOf(vertex))
            if (vertex <= neighbour) {
                writeBinaryValue(fileStream, vertex);
                writeBinaryValue(fileStream, neighbour);
                writeBinaryValue(fileStream,
                                 graph.getEdgeLabelOf(vertex, neighbour));
            }
}

template <typename EdgeLabel>
EdgeLabeledDirectedGraph<EdgeLabel>
loadLabeledDirectedBinaryEdgeList(const std::string &fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value,
                  "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    EdgeLabeledDirectedGraph<EdgeLabel> returnedGraph(0);

    VertexIndex vertex1, vertex2;
    EdgeLabel label;
    while (readBinaryValue(fileStream, vertex1)) {
        readBinaryValue(fileStream, vertex2);
        readBinaryValue(fileStream, label);

        if (vertex1 >= returnedGraph.getSize())
            returnedGraph.resize(vertex1 + 1);
        if (vertex2 >= returnedGraph.getSize())
            returnedGraph.resize(vertex2 + 1);

        returnedGraph.addEdge(vertex1, vertex2, label);
    }
    return returnedGraph;
}

template <typename EdgeLabel>
EdgeLabeledUndirectedGraph<EdgeLabel>
loadLabeledUndirectedBinaryEdgeList(const std::string &fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value,
                  "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    EdgeLabeledUndirectedGraph<EdgeLabel> returnedGraph(0);

    VertexIndex vertex1, vertex2;
    EdgeLabel label;
    while (readBinaryValue(fileStream, vertex1)) {
        readBinaryValue(fileStream, vertex2);
        readBinaryValue(fileStream, label);

        if (vertex1 >= returnedGraph.getSize())
            returnedGraph.resize(vertex1 + 1);
        if (vertex2 >= returnedGraph.getSize())
            returnedGraph.resize(vertex2 + 1);

        returnedGraph.addEdge(vertex1, vertex2, label);
    }
    return returnedGraph;
}

} // namespace io
} // namespace BaseGraph

#endif
