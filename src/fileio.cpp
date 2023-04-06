#include "BaseGraph/fileio.h"
#include "BaseGraph/types.h"

#include <cstdlib>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

namespace BaseGraph {
namespace io {


static inline std::pair<std::string, std::string> findVerticesFromStr(std::string &s, const char *t = " \t\n\r\f\v");

// DirectedGraph

void writeTextEdgeList(const DirectedGraph &graph, const string &fileName,
                       size_t vertexIndexShift) {
    ofstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    for (const VertexIndex i : graph)
        for (const VertexIndex &j : graph.getOutEdgesOf(i))
            fileStream << i + vertexIndexShift << " " << j + vertexIndexShift
                       << '\n';
}

void writeBinaryEdgeList(const DirectedGraph &graph, const string &fileName) {
    ofstream fileStream(fileName, ios::out | ios::binary);
    verifyStreamOpened(fileStream, fileName);

    size_t byteSize = sizeof(VertexIndex);

    for (const VertexIndex &i : graph) {
        for (const VertexIndex &j : graph.getOutEdgesOf(i)) {
            writeBinaryValue(fileStream, i);
            writeBinaryValue(fileStream, j);
        }
    }
}

std::pair<DirectedGraph, std::vector<std::string>> loadDirectedTextEdgeList(
    const string &fileName,
    VertexIndex vertexNumber,
    std::function<VertexIndex(const std::string &)> getVertex) {

    ifstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    DirectedGraph returnedGraph(0);
    std::vector<std::string> vertexLabels;

    std::function<void(size_t)> resize;
    if (vertexNumber == 0) {
        resize = [&returnedGraph, &vertexLabels](size_t newSize) {
            if (newSize >= returnedGraph.getSize()) {
                returnedGraph.resize(newSize + 1);
                vertexLabels.resize(newSize + 1, "");
            }
        };
    } else {
        returnedGraph.resize(vertexNumber);
        vertexLabels.resize(vertexNumber, "");
        resize = [](size_t) {};
    }
    string fullLine;
    std::pair<std::string, std::string> verticesStr;
    VertexIndex vertex, vertex2;

    while (getline(fileStream, fullLine)) {
        if (fullLine[0] == '#') {
            continue;
        }
        verticesStr = findVerticesFromStr(fullLine);
        vertex = getVertex(verticesStr.first);
        vertex2 = getVertex(verticesStr.second);
        resize(vertex);
        resize(vertex2);
        vertexLabels[vertex] = verticesStr.first;
        vertexLabels[vertex2] = verticesStr.second;
        returnedGraph.addEdge(vertex, vertex2);
    }
    return {returnedGraph, vertexLabels};
}

DirectedGraph loadDirectedBinaryEdgeList(const string &fileName) {
    size_t byteSize = sizeof(VertexIndex);

    ifstream fileStream(fileName, ios::out | ios::binary);
    verifyStreamOpened(fileStream, fileName);

    DirectedGraph returnedGraph(0);

    bool addEdge = false;
    VertexIndex vertex1, vertex2;
    while (readBinaryValue(fileStream, vertex2)) {
        if (vertex2 >= returnedGraph.getSize())
            returnedGraph.resize(vertex2 + 1);
        if (addEdge)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}

// UndirectedGraph

void writeTextEdgeList(const UndirectedGraph &graph, const string &fileName) {
    ofstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    for (const VertexIndex &i : graph)
        for (const VertexIndex &j : graph.getNeighboursOf(i))
            if (i <= j)
                fileStream << i << "   " << j << '\n';
}

void writeBinaryEdgeList(const UndirectedGraph &graph, const string &fileName) {
    ofstream fileStream(fileName, ios::out | ios::binary);
    verifyStreamOpened(fileStream, fileName);

    size_t byteSize = sizeof(VertexIndex);

    for (const VertexIndex &i : graph) {
        for (const VertexIndex &j : graph.getNeighboursOf(i)) {
            if (i <= j) { // write edges once
                writeBinaryValue(fileStream, i);
                writeBinaryValue(fileStream, j);
            }
        }
    }
}

std::pair<UndirectedGraph, std::vector<std::string>> loadUndirectedTextEdgeList(
    const string &fileName, VertexIndex vertexNumber,
    std::function<VertexIndex(const std::string &)> getVertex) {

    ifstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    UndirectedGraph returnedGraph(0);
    std::vector<std::string> vertexLabels;

    std::function<void(size_t)> resize;
    if (vertexNumber == 0) {
        resize = [&returnedGraph, &vertexLabels](size_t newSize) {
            if (newSize >= returnedGraph.getSize()) {
                returnedGraph.resize(newSize + 1);
                vertexLabels.resize(newSize + 1, "");
            }
        };
    } else {
        returnedGraph.resize(vertexNumber);
        vertexLabels.resize(vertexNumber, "");
        resize = [](size_t) {};
    }
    string fullLine;
    std::pair<std::string, std::string> verticesStr;
    VertexIndex vertex, vertex2;

    while (getline(fileStream, fullLine)) {
        if (fullLine[0] == '#') {
            continue;
        }
        verticesStr = findVerticesFromStr(fullLine);
        vertex = getVertex(verticesStr.first);
        vertex2 = getVertex(verticesStr.second);
        resize(vertex);
        resize(vertex2);
        vertexLabels[vertex] = verticesStr.first;
        vertexLabels[vertex2] = verticesStr.second;
        returnedGraph.addEdge(vertex, vertex2);
    }
    return {returnedGraph, vertexLabels};
}

UndirectedGraph loadUndirectedBinaryEdgeList(const string &fileName) {
    size_t byteSize = sizeof(VertexIndex);

    ifstream fileStream(fileName, ios::out | ios::binary);
    verifyStreamOpened(fileStream, fileName);

    UndirectedGraph returnedGraph(0);

    bool addEdge = false;
    VertexIndex vertex1, vertex2;
    while (readBinaryValue(fileStream, vertex2)) {
        if (vertex2 >= returnedGraph.getSize())
            returnedGraph.resize(vertex2 + 1);
        if (addEdge)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}

// Inspired by
// https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
static inline std::pair<std::string, std::string>
findVerticesFromStr(std::string &s, const char *t) {
    const auto v1Start = s.find_first_not_of(t);
    const auto v1End = s.find_last_not_of(t, v1Start)+1;

    const auto v2Start = s.find_first_not_of(t, v1End);
    const auto v2End = s.find_last_not_of(t, v2Start)+1;

    return std::make_pair(s.substr(v1Start, v1End), s.substr(v2Start, v2End));
}

} // namespace io
} // namespace BaseGraph
