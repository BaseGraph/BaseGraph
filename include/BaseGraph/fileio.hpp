#ifndef BASE_GRAPH_FILEIO_HPP
#define BASE_GRAPH_FILEIO_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

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

// From https://mklimenko.github.io/english/2018/08/22/robust-endian-swap/
template <typename T> void swapBytes(T &val);

inline bool _isSystemBigEndian();
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

template <template <class...> class Graph, typename EdgeLabel>
void writeTextEdgeList(const Graph<EdgeLabel> &graph,
                       const std::string &fileName,
                       const std::function<std::string(const EdgeLabel &)>
                           toString = std::to_string);

template <template <class...> class Graph, typename EdgeLabel>
std::pair<Graph<EdgeLabel>, std::vector<std::string>>
loadTextVertexLabeledEdgeList(
    const std::string &fileName,
    const std::function<EdgeLabel(const std::string &)> edgeFromString =
        [](const std::string &s) { return EdgeLabel(); },
    const std::function<VertexIndex(const std::string &)> vertexFromString =
        VertexCountMapper());

template <template <class...> class Graph, typename EdgeLabel>
std::pair<Graph<EdgeLabel>, std::vector<std::string>> loadTextEdgeList(
    const std::string &fileName,
    const std::function<EdgeLabel(const std::string &)> fromString =
        [](const std::string &s) { return EdgeLabel(); }) {
    return loadTextVertexLabeledEdgeList<Graph, EdgeLabel>(
        fileName, fromString, [](const std::string &str) { return stoi(str); });
}

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<!std::is_same<EdgeLabel, NoLabel>::value>::type
writeBinaryEdgeList(const Graph<EdgeLabel> &graph, const std::string &fileName,
                    const std::function<void(std::ofstream &, EdgeLabel)>
                        toBinary = writeBinaryValue<EdgeLabel>);

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<std::is_same<EdgeLabel, NoLabel>::value>::type
writeBinaryEdgeList(const Graph<EdgeLabel> &graph, const std::string &fileName);

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<!std::is_same<EdgeLabel, NoLabel>::value,
                        Graph<EdgeLabel>>::type
loadBinaryEdgeList(
    const std::string &fileName,
    const std::function<std::ifstream &(std::ifstream &, EdgeLabel &)>
        fromBinary = readBinaryValue<EdgeLabel>);

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<std::is_same<EdgeLabel, NoLabel>::value,
                        Graph<EdgeLabel>>::type
loadBinaryEdgeList(const std::string &fileName);

template <typename T>
inline void verifyStreamOpened(const T &fileStream,
                               const std::string &fileName) {
    if (!fileStream.is_open())
        throw std::runtime_error("Could not open file \"" + fileName + "\".");
}

inline std::array<std::string, 3>
findEdgeFromString(std::string &s, const char *t = " \t\n\r\f\v");

template <template <class...> class Graph, typename EdgeLabel>
void writeTextEdgeList(
    const Graph<EdgeLabel> &graph, const std::string &fileName,
    const std::function<std::string(const EdgeLabel &)> toString) {

    std::ofstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1 Vertex2 Label\n";

    for (auto edge : graph.edges())
        fileStream << edge.first << " " << edge.second << " "
                   << toString(graph.getEdgeLabel(edge.first, edge.second))
                   << '\n';
}

template <template <class...> class Graph>
void writeTextEdgeList(
    const Graph<NoLabel> &graph, const std::string &fileName,
    const std::function<std::string(const NoLabel &)> toString =
        [](const NoLabel &) { return ""; }) {

    std::ofstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1 Vertex2 Label\n";

    for (auto edge : graph.edges())
        fileStream << edge.first << " " << edge.second << '\n';
}

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<!std::is_same<EdgeLabel, NoLabel>::value>::type
writeBinaryEdgeList(
    const Graph<EdgeLabel> &graph, const std::string &fileName,
    const std::function<void(std::ofstream &, EdgeLabel)> toBinary) {

    static_assert(!std::is_same<EdgeLabel, std::string>::value,
                  "No implementation of string to write binary file");

    std::ofstream fileStream(fileName.c_str(),
                             std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    for (auto edge : graph.edges()) {
        writeBinaryValue(fileStream, edge.first);
        writeBinaryValue(fileStream, edge.second);
        toBinary(fileStream, graph.getEdgeLabel(edge.first, edge.second));
    }
}

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<std::is_same<EdgeLabel, NoLabel>::value>::type
writeBinaryEdgeList(const Graph<EdgeLabel> &graph,
                    const std::string &fileName) {

    std::ofstream fileStream(fileName.c_str(),
                             std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    for (auto edge : graph.edges()) {
        writeBinaryValue(fileStream, edge.first);
        writeBinaryValue(fileStream, edge.second);
    }
}

template <template <class...> class Graph, typename EdgeLabel>
std::pair<Graph<EdgeLabel>, std::vector<std::string>>
loadTextVertexLabeledEdgeList(
    const std::string &fileName,
    const std::function<EdgeLabel(const std::string &)> edgeFromString,
    const std::function<VertexIndex(const std::string &)> vertexFromString) {

    std::ifstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    Graph<EdgeLabel> returnedGraph(0);
    std::vector<std::string> vertexLabels;

    std::string fullLine;
    while (getline(fileStream, fullLine)) {
        if (fullLine[0] == '#') {
            continue;
        }
        auto edgeString = findEdgeFromString(fullLine);
        auto vertex = vertexFromString(edgeString[0]);
        auto vertex2 = vertexFromString(edgeString[1]);

        auto largestVertex = std::max(vertex, vertex2);
        if (largestVertex >= returnedGraph.getSize()) {
            returnedGraph.resize(largestVertex + 1);
            vertexLabels.resize(largestVertex + 1);
        }
        vertexLabels[vertex] = std::move(edgeString[0]);
        vertexLabels[vertex2] = std::move(edgeString[1]);
        returnedGraph.addEdge(vertex, vertex2, edgeFromString(edgeString[2]),
                              true);
    }
    return {std::move(returnedGraph), std::move(vertexLabels)};
}

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<!std::is_same<EdgeLabel, NoLabel>::value,
                        Graph<EdgeLabel>>::type
loadBinaryEdgeList(
    const std::string &fileName,
    const std::function<std::ifstream &(std::ifstream &, EdgeLabel &)>
        fromBinary) {

    static_assert(!std::is_same<EdgeLabel, std::string>::value,
                  "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    Graph<EdgeLabel> returnedGraph(0);

    VertexIndex vertex1, vertex2;
    EdgeLabel label;
    while (readBinaryValue(fileStream, vertex1)) {
        readBinaryValue(fileStream, vertex2);
        fromBinary(fileStream, label);

        if (vertex1 >= returnedGraph.getSize())
            returnedGraph.resize(vertex1 + 1);
        if (vertex2 >= returnedGraph.getSize())
            returnedGraph.resize(vertex2 + 1);

        returnedGraph.addEdge(vertex1, vertex2, label, true);
    }
    return returnedGraph;
}

template <template <class...> class Graph, typename EdgeLabel>
typename std::enable_if<std::is_same<EdgeLabel, NoLabel>::value,
                        Graph<EdgeLabel>>::type
loadBinaryEdgeList(const std::string &fileName) {
    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    Graph<NoLabel> returnedGraph(0);

    VertexIndex vertex1, vertex2;
    NoLabel label;
    while (readBinaryValue(fileStream, vertex1)) {
        readBinaryValue(fileStream, vertex2);

        if (vertex1 >= returnedGraph.getSize())
            returnedGraph.resize(vertex1 + 1);
        if (vertex2 >= returnedGraph.getSize())
            returnedGraph.resize(vertex2 + 1);
        returnedGraph.addEdge(vertex1, vertex2, true);
    }
    return returnedGraph;
}

inline std::array<std::string, 3> findEdgeFromString(std::string &s,
                                                     const char *t) {
    auto pos1 = s.find_first_not_of(t);
    auto pos2 = s.find_first_of(t, pos1);
    auto pos3 = s.find_first_not_of(t, pos2);
    auto pos4 = s.find_first_of(t, pos3);
    auto pos5 = s.find_first_not_of(t, pos4);
    if (pos5 == s.npos)
        return {s.substr(pos1, pos2 - pos1), s.substr(pos3, pos4 - pos3), ""};
    auto pos6 = s.find_first_of(t, pos5);
    return {s.substr(pos1, pos2 - pos1), s.substr(pos3, pos4 - pos3),
            s.substr(pos5, pos6 - pos5)};
}

template <typename T> void swapBytes(T &val) {
    union U {
        T val;
        std::array<std::uint8_t, sizeof(T)> raw;
    } src, dst;

    src.val = val;
    std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
    val = dst.val;
}

bool _isSystemBigEndian() {
    union {
        uint32_t i32;
        uint8_t i8[4];
    } integer = {0x01020304};
    return integer.i8[0] == 1;
}

} // namespace io
} // namespace BaseGraph

#endif
