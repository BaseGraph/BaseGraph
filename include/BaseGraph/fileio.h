#ifndef BASE_GRAPH_FILEIO_H
#define BASE_GRAPH_FILEIO_H

#include <map>
#include <string>
#include <iostream>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/vertexlabeled_graph.hpp"


namespace BaseGraph{


void writeEdgeListIdxInTextFile(const DirectedGraph& graph, const std::string& fileName, size_t vertexIndexShift=0);
void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, const std::string& fileName);

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, const std::string& fileName);
void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, const std::string& fileName);

template<typename Label, bool hashable=false> void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);

template<typename Label, bool hashable=false> void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);


DirectedGraph loadDirectedEdgeListIdxFromTextFile(const std::string& fileName);
DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(const std::string& fileName);

UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(const std::string& fileName);
UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(const std::string& fileName);

VertexLabeledUndirectedGraph<std::string, true> loadUndirectedEdgeListFromTextFile(const std::string& fileName);
template<typename Label, bool hashable=false> VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);

VertexLabeledDirectedGraph<std::string, true> loadDirectedEdgeListFromTextFile(const std::string& fileName);
template<typename Label, bool hashable=false> VertexLabeledDirectedGraph<Label, hashable> loadDirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);


template<typename T>
inline void verifyStreamOpened(const T& fileStream, const std::string& fileName) {
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file \""+fileName+"\".");
}

// VertexLabeledDirectedGraph

template<typename Label, bool hashable>
VertexLabeledDirectedGraph<Label, hashable> loadDirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    VertexLabeledDirectedGraph<Label, hashable> returnedGraph;

    Label vertex1, vertex2;
    bool addEdge = false;
    while (fileStream.read((char*) &vertex2, byteSize)){
        returnedGraph.addVertex(vertex2);
        if (addEdge)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}

template<typename Label, bool hashable>
void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    Label vertex;
    while (fileStream.read((char*) &vertex, byteSize))
        graph.addVertex(vertex);
}

template<typename Label, bool hashable>
void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            // +x : promotes "x" to a type printable as a number, regardless of type
            fileStream << +vertices[i] << "   " << +vertices[j] << '\n';
}

template<bool hashable>
inline void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<std::string, hashable>& graph, std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<typename Label, bool hashable>
void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph) {
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i)) {
            fileStream.write((char*) &vertices[i], byteSize);
            fileStream.write((char*) &vertices[j], byteSize);
        }
    }
}

template<typename Label, bool hashable>
void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ofstream fileStream(fileName, std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (auto& vertex: vertices)
        fileStream.write((char*) &vertex, byteSize);
}


// VertexLabeledUndirectedGraph

template<typename Label, bool hashable>
VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    VertexLabeledUndirectedGraph<Label, hashable> returnedGraph;

    Label vertex1, vertex2;
    bool addEdge = false;
    while (fileStream.read((char*) &vertex2, byteSize)){
        returnedGraph.addVertex(vertex2);
        if (addEdge)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}

template<typename Label, bool hashable>
void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    Label vertex;
    while (fileStream.read((char*) &vertex, byteSize))
        graph.addVertex(vertex);
}

template<typename Label, bool hashable>
void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            // +x : promotes "x" to a type printable as a number, regardless of type
            if (i<j) fileStream << +vertices[i] << "   " << +vertices[j] << '\n';
}

template<bool hashable>
inline void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<std::string>& graph, std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            if (i<j) fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<typename Label, bool hashable>
void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph) {
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i)) {
            if (i <= j) { // write edges once
                fileStream.write((char*) &vertices[i], byteSize);
                fileStream.write((char*) &vertices[j], byteSize);
            }
        }
    }
}

template<typename Label, bool hashable>
void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    std::ofstream fileStream(fileName, std::ios::out | std::ios::binary);
    verifyStreamOpened(fileStream, fileName);

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        fileStream.write((char*) &vertices[i], byteSize);
}


} // namespace BaseGraph


#endif
