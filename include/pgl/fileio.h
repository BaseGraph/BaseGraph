#ifndef PGL_FILEIO_H
#define PGL_FILEIO_H

#include <map>
#include <string>

#include "pgl/directedgraph.h"
#include "pgl/undirectedgraph.h"
#include "pgl/vertexlabeled_directedgraph.hpp"
#include "pgl/vertexlabeled_undirectedgraph.hpp"


namespace PGL{


std::map<std::string, VertexIndex> loadGraphFromEdgelist(const std::string edgelist_filename, DirectedGraph& graph, const bool allow_multiedges = false, const bool allow_selfloops = false);

void writeEdgeListIdxInTextFile(const DirectedGraph& graph, const std::string& fileName, size_t starting_id = 0);
void writeEdgeListIdxInTextFile(const DirectedGraph& graph, std::ofstream& fileStream, size_t starting_id = 0);
void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, const std::string& fileName);
void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, std::ofstream& fileStream);

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, const std::string& fileName);
void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, std::ofstream& fileStream);
void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, const std::string& fileName);
void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, std::ofstream& fileStream);

template<typename T> void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName);
template<typename T> void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<T>& graph, std::ofstream& fileStream);
template<typename T> void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName, size_t byteSize=0);
template<typename T> void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize=0);
template<typename T> void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName, size_t byteSize=0);
template<typename T> void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize=0);

template<typename T> void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<T>& graph, const std::string& fileName);
template<typename T> void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<T>& graph, std::ofstream& fileStream);
template<typename T> void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, const std::string& fileName, size_t byteSize=0);
template<typename T> void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize=0);
template<typename T> void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, const std::string& fileName, size_t byteSize=0);
template<typename T> void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize=0);


DirectedGraph loadDirectedEdgeListIdxFromTextFile(const std::string& fileName);
DirectedGraph loadDirectedEdgeListIdxFromTextFile(std::ifstream& fileStream);
DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(const std::string& fileName);
DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(std::ifstream& fileStream);

UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(const std::string& fileName);
UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(std::ifstream& fileStream);
UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(const std::string& fileName);
UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(std::ifstream& fileStream);

VertexLabeledUndirectedGraph<std::string> loadUndirectedEdgeListFromTextFile(const std::string& fileName);
VertexLabeledUndirectedGraph<std::string> loadUndirectedEdgeListFromTextFile(std::ifstream& fileStream);
template<typename T> VertexLabeledUndirectedGraph<T> loadUndirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize=0);
template<typename T> VertexLabeledUndirectedGraph<T> loadUndirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize=0);
template<typename T> void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName, size_t byteSize=0);
template<typename T> void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<T>& graph, std::ifstream& fileStream, size_t byteSize=0);

VertexLabeledDirectedGraph<std::string> loadDirectedEdgeListFromTextFile(const std::string& fileName);
VertexLabeledDirectedGraph<std::string> loadDirectedEdgeListFromTextFile(std::ifstream& fileStream);
template<typename T> VertexLabeledDirectedGraph<T> loadDirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize=0);
template<typename T> VertexLabeledDirectedGraph<T> loadDirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize=0);
template<typename T> void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<T>& graph, const std::string& fileName, size_t byteSize=0);
template<typename T> void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<T>& graph, std::ifstream& fileStream, size_t byteSize=0);




// VertexLabeledDirectedGraph

template<typename T>
VertexLabeledDirectedGraph<T> loadDirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize){
    VertexLabeledDirectedGraph<T> returnedGraph;

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    returnedGraph = loadDirectedEdgeListFromBinaryFile<T>(fileStream, byteSize);
    fileStream.close();

    return returnedGraph;
}

template<typename T>
VertexLabeledDirectedGraph<T> loadDirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize){
    VertexLabeledDirectedGraph<T> returnedGraph;
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    T vertex1, vertex2;
    VertexIndex i = 0;
    while (fileStream.read((char*) &vertex2, byteSize)){
        returnedGraph.addVertex(vertex2);
        if (i % 2 == 1)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        ++i;
    }
    return returnedGraph;
}

template<>
inline VertexLabeledDirectedGraph<std::string> loadDirectedEdgeListFromBinaryFile(std::ifstream& fileName, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<T>& graph, const std::string& fileName, size_t byteSize){
    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    addVerticesFromBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<T>& graph, std::ifstream& fileStream, size_t byteSize){
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    T vertex;
    while (fileStream.read((char*) &vertex, byteSize)){
        graph.addVertex(vertex);
    }
}

template<>
inline void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<std::string>& graph, std::ifstream& fileStream, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<T>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    writeEdgeListInTextFile(graph, fileStream);
    fileStream.close();
}

template<typename T>
void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<T>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<>
inline void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<unsigned char>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<>
inline void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<signed char>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<typename T>
void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    writeEdgeListInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize){
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph) {
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i)) {
            fileStream.write((char*) &vertices[i], byteSize);
            fileStream.write((char*) &vertices[j], byteSize);
        }
    }
}

template<>
inline void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<std::string>& graph, std::ofstream& fileName, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName, std::ios::binary);
    writeVerticesInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize){
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    auto& vertices = graph.getVertices();
    for (auto& vertex: vertices)
        fileStream.write((char*) &vertex, byteSize);
}

template<>
inline void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<std::string>& graph, std::ofstream& fileStream, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}


// VertexLabeledUndirectedGraph

template<typename T>
VertexLabeledUndirectedGraph<T> loadUndirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize){
    VertexLabeledUndirectedGraph<T> returnedGraph;

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    returnedGraph = loadUndirectedEdgeListFromBinaryFile<T>(fileStream, byteSize);
    fileStream.close();

    return returnedGraph;
}

template<typename T>
VertexLabeledUndirectedGraph<T> loadUndirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize){
    VertexLabeledUndirectedGraph<T> returnedGraph;
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    T vertex1, vertex2;
    VertexIndex i = 0;
    while (fileStream.read((char*) &vertex2, byteSize)){
        returnedGraph.addVertex(vertex2);
        if (i % 2 == 1)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        ++i;
    }
    return returnedGraph;
}

template<>
inline VertexLabeledUndirectedGraph<std::string> loadUndirectedEdgeListFromBinaryFile(std::ifstream& fileName, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName, size_t byteSize){
    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    addVerticesFromBinaryFile<T>(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<T>& graph, std::ifstream& fileStream, size_t byteSize){
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    T vertex;
    while (fileStream.read((char*) &vertex, byteSize)){
        graph.addVertex(vertex);
    }
}

template<>
inline void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<std::string>& graph, std::ifstream& fileStream, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    writeEdgeListInTextFile(graph, fileStream);
    fileStream.close();
}

template<typename T>
void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<T>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            if (i<j) fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<>
inline void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<unsigned char>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            if (i<j) fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<>
inline void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<signed char>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            if (i<j) fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<typename T>
void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    writeEdgeListInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize) {
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

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

template<>
inline void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<std::string>& graph, std::ofstream& fileName, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName, std::ios::binary);
    writeVerticesInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<T>& graph, std::ofstream& fileStream, size_t byteSize){
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        fileStream.write((char*) &vertices[i], byteSize);
}

template<>
inline void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<std::string>& graph, std::ofstream& fileStream, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}


} // namespace PGL


#endif
