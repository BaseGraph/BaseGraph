#include "BaseGraph/fileio.h"

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
    std::function<VertexIndex(const std::string &)> getVertex) {
    ifstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    DirectedGraph returnedGraph(0);
    std::vector<std::string> vertexLabels;

    stringstream currentLine;
    string fullLine, strVertex, strVertex2;
    VertexIndex vertex, vertex2;

    auto resize = [&returnedGraph, &vertexLabels](size_t newSize) {
        if (newSize >= returnedGraph.getSize()) {
            returnedGraph.resize(newSize + 1);
            vertexLabels.resize(newSize + 1, "");
        }
    };

    while (getline(fileStream, fullLine)) {
        fileStream >> std::ws;
        currentLine.str(fullLine);
        currentLine >> std::ws;
        currentLine >> strVertex >> std::ws;

        // Skips lines of comment.
        if (strVertex == "#") {
            currentLine.clear();
            continue;
        }

        currentLine >> strVertex2 >> std::ws;
        currentLine.clear();

        vertex = getVertex(strVertex);
        vertex2 = getVertex(strVertex2);
        resize(vertex);
        resize(vertex2);
        vertexLabels[vertex] = strVertex;
        vertexLabels[vertex2] = strVertex2;

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
    const string &fileName,
    std::function<VertexIndex(const std::string &)> getVertex) {
    ifstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    UndirectedGraph returnedGraph(0);
    std::vector<std::string> vertexLabels;

    stringstream currentLine;
    string fullLine, strVertex, strVertex2;
    VertexIndex vertex, vertex2;

    auto resize = [&returnedGraph, &vertexLabels](size_t newSize) {
        if (newSize >= returnedGraph.getSize()) {
            returnedGraph.resize(newSize + 1);
            vertexLabels.resize(newSize + 1, "");
        }
    };

    while (getline(fileStream, fullLine)) {
        fileStream >> std::ws;
        currentLine.str(fullLine);
        currentLine >> std::ws;
        currentLine >> strVertex >> std::ws;

        // Skips lines of comment.
        if (strVertex == "#") {
            currentLine.clear();
            continue;
        }
        currentLine >> strVertex2 >> std::ws;
        currentLine.clear();

        vertex = getVertex(strVertex);
        vertex2 = getVertex(strVertex2);
        resize(vertex);
        resize(vertex2);
        vertexLabels[vertex] = strVertex;
        vertexLabels[vertex2] = strVertex2;
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

} // namespace io
} // namespace BaseGraph
