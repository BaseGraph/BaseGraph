#ifndef BASE_GRAPH_FILEIO_H
#define BASE_GRAPH_FILEIO_H

#include <map>
#include <string>
#include <iostream>
#include <array>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/vertexlabeled_graph.hpp"
#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


namespace BaseGraph{ namespace io {


void writeTextEdgeListIdx(const DirectedGraph& graph, const std::string& fileName, size_t vertexIndexShift=0);
void writeBinaryEdgeListIdx(const DirectedGraph& graph, const std::string& fileName);
template<typename EdgeLabel>void writeBinaryEdgeListIdx(const EdgeLabeledDirectedGraph<EdgeLabel>& graph, const std::string& fileName);

void writeTextEdgeListIdx(const UndirectedGraph& graph, const std::string& fileName);
void writeBinaryEdgeListIdx(const UndirectedGraph& graph, const std::string& fileName);
template<typename EdgeLabel>void writeBinaryEdgeListIdx(const EdgeLabeledUndirectedGraph<EdgeLabel>& graph, const std::string& fileName);

template<typename Label, bool hashable=false> void writeTextEdgeList(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeBinaryEdgeList(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeVerticesToBinary(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName);

template<typename Label, bool hashable=false> void writeTextEdgeList(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeBinaryEdgeList(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeVerticesToBinary(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName);


DirectedGraph loadDirectedTextEdgeListIdx(const std::string& fileName);
DirectedGraph loadDirectedBinaryEdgeListIdx(const std::string& fileName);
template<typename EdgeLabel> EdgeLabeledDirectedGraph<EdgeLabel> loadLabeledDirectedBinaryEdgeListIdx(const std::string& fileName);

UndirectedGraph loadUndirectedTextEdgeListIdx(const std::string& fileName);
UndirectedGraph loadUndirectedBinaryEdgeListIdx(const std::string& fileName);
template<typename EdgeLabel> EdgeLabeledUndirectedGraph<EdgeLabel> loadLabeledUndirectedBinaryEdgeListIdx(const std::string& fileName);

VertexLabeledUndirectedGraph<std::string, true> loadUndirectedTextEdgeList(const std::string& fileName);
template<typename Label, bool hashable=false> VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedBinaryEdgeList(const std::string& fileName);
template<typename Label, bool hashable=false> void addVerticesFromBinary(VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName);

VertexLabeledDirectedGraph<std::string, true> loadDirectedTextEdgeList(const std::string& fileName);
template<typename Label, bool hashable=false> VertexLabeledDirectedGraph<Label, hashable> loadDirectedBinaryEdgeList(const std::string& fileName);
template<typename Label, bool hashable=false> void addVerticesFromBinary(VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName);


template<typename T>
inline void verifyStreamOpened(const T& fileStream, const std::string& fileName) {
    if (!fileStream.is_open())
        throw std::runtime_error("Could not open file \""+fileName+"\".");
}

// From https://mklimenko.github.io/english/2018/08/22/robust-endian-swap/
template<typename T>
void swapBytes(T& val) {
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

template<typename T>
void writeBinaryValue(std::ofstream& fileStream, T value) {
    if (SYSTEM_IS_BIG_ENDIAN)
        swapBytes(value);
    fileStream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template<typename T>
std::ifstream& readBinaryValue(std::ifstream& fileStream, T& value) {
    fileStream.read(reinterpret_cast<char*>(&value), sizeof(T));
    if (SYSTEM_IS_BIG_ENDIAN && fileStream)
        swapBytes(value);
    return fileStream;
}


// EdgeLabeled Graphs

template<typename EdgeLabel>
void writeBinaryEdgeListIdx(const EdgeLabeledDirectedGraph<EdgeLabel>& graph, const std::string& fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value, "No implementation of string to write binary file");

    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    for (auto vertex: graph)
        for (auto neighbour: graph.getOutEdgesOfIdx(vertex)) {
            writeBinaryValue(fileStream, vertex);
            writeBinaryValue(fileStream, neighbour);
            writeBinaryValue(fileStream, graph.getEdgeLabelOfIdx(vertex, neighbour));
        }
}

template<typename EdgeLabel>
void writeBinaryEdgeListIdx(const EdgeLabeledUndirectedGraph<EdgeLabel>& graph, const std::string& fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value, "No implementation of string to write binary file");

    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    for (auto vertex: graph)
        for (auto neighbour: graph.getOutEdgesOfIdx(vertex))
            if (vertex <= neighbour) {
                writeBinaryValue(fileStream, vertex);
                writeBinaryValue(fileStream, neighbour);
                writeBinaryValue(fileStream, graph.getEdgeLabelOfIdx(vertex, neighbour));
            }
}

template<typename EdgeLabel>
EdgeLabeledDirectedGraph<EdgeLabel> loadLabeledDirectedBinaryEdgeListIdx(const std::string& fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value, "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    EdgeLabeledDirectedGraph<EdgeLabel> returnedGraph(0);

    VertexIndex vertex1, vertex2;
    EdgeLabel label;
    while (readBinaryValue(fileStream, vertex1)) {
        readBinaryValue(fileStream, vertex2);
        readBinaryValue(fileStream, label);

        if (vertex1 >= returnedGraph.getSize()) returnedGraph.resize(vertex1+1);
        if (vertex2 >= returnedGraph.getSize()) returnedGraph.resize(vertex2+1);

        returnedGraph.addEdgeIdx(vertex1, vertex2, label);
    }
    return returnedGraph;
}

template<typename EdgeLabel>
EdgeLabeledUndirectedGraph<EdgeLabel> loadLabeledUndirectedBinaryEdgeListIdx(const std::string& fileName) {
    static_assert(!std::is_same<EdgeLabel, std::string>::value, "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    EdgeLabeledUndirectedGraph<EdgeLabel> returnedGraph(0);

    VertexIndex vertex1, vertex2;
    EdgeLabel label;
    while (readBinaryValue(fileStream, vertex1)) {
        readBinaryValue(fileStream, vertex2);
        readBinaryValue(fileStream, label);

        if (vertex1 >= returnedGraph.getSize()) returnedGraph.resize(vertex1+1);
        if (vertex2 >= returnedGraph.getSize()) returnedGraph.resize(vertex2+1);

        returnedGraph.addEdgeIdx(vertex1, vertex2, label);
    }
    return returnedGraph;
}


// VertexLabeledDirectedGraph

template<typename Label, bool hashable>
VertexLabeledDirectedGraph<Label, hashable> loadDirectedBinaryEdgeList(const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    VertexLabeledDirectedGraph<Label, hashable> returnedGraph;

    Label vertex1, vertex2;
    bool addEdge = false;
    while (readBinaryValue(fileStream, vertex2)) {
        returnedGraph.addVertex(vertex2);
        if (addEdge)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}

template<typename Label, bool hashable>
void addVerticesFromBinary(VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    Label vertex;
    while (readBinaryValue(fileStream, vertex))
        graph.addVertex(vertex);
}

template<typename Label, bool hashable>
void writeTextEdgeList(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<typename Label, bool hashable>
void writeBinaryEdgeList(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");

    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph) {
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i)) {
            writeBinaryValue(fileStream, vertices[i]);
            writeBinaryValue(fileStream, vertices[j]);
        }
    }
}

template<typename Label, bool hashable>
void writeVerticesToBinary(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");

    std::ofstream fileStream(fileName, std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (auto& vertex: vertices)
        writeBinaryValue(fileStream, vertex);
}


// VertexLabeledUndirectedGraph

template<typename Label, bool hashable>
VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedBinaryEdgeList(const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    VertexLabeledUndirectedGraph<Label, hashable> returnedGraph;

    Label vertex1, vertex2;
    bool addEdge = false;
    while (readBinaryValue(fileStream, vertex2)) {
        returnedGraph.addVertex(vertex2);
        if (addEdge)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}

template<typename Label, bool hashable>
void addVerticesFromBinary(VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");

    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    Label vertex;
    while (readBinaryValue(fileStream, vertex))
        graph.addVertex(vertex);
}

template<typename Label, bool hashable>
void writeTextEdgeList(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            if (i<=j) fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<typename Label, bool hashable>
void writeBinaryEdgeList(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");

    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph) {
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i)) {
            if (i <= j) { // write edges once
                writeBinaryValue(fileStream, vertices[i]);
                writeBinaryValue(fileStream, vertices[j]);
            }
        }
    }
}

template<typename Label, bool hashable>
void writeVerticesToBinary(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");

    std::ofstream fileStream(fileName, std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        writeBinaryValue(fileStream, vertices[i]);
}


}} // namespace BaseGraph::io


#endif
