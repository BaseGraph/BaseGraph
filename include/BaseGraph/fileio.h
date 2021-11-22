#ifndef BASE_GRAPH_FILEIO_H
#define BASE_GRAPH_FILEIO_H

#include <map>
#include <string>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/vertexlabeled_graph.hpp"


namespace BaseGraph{


void writeEdgeListIdxInTextFile(const DirectedGraph& graph, const std::string& fileName, size_t vertexIndexShift=0);
void writeEdgeListIdxInTextFile(const DirectedGraph& graph, std::ofstream& fileStream, size_t vertexIndexShift=0);
void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, const std::string& fileName);
void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, std::ofstream& fileStream);

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, const std::string& fileName);
void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, std::ofstream& fileStream);
void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, const std::string& fileName);
void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, std::ofstream& fileStream);

template<typename Label, bool hashable=false> void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ofstream& fileStream);
template<typename Label, bool hashable=false> void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize=0);

template<typename Label, bool hashable=false> void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName);
template<typename Label, bool hashable=false> void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, std::ofstream& fileStream);
template<typename Label, bool hashable=false> void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize=0);


DirectedGraph loadDirectedEdgeListIdxFromTextFile(const std::string& fileName);
DirectedGraph loadDirectedEdgeListIdxFromTextFile(std::ifstream& fileStream);
DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(const std::string& fileName);
DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(std::ifstream& fileStream);

UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(const std::string& fileName);
UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(std::ifstream& fileStream);
UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(const std::string& fileName);
UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(std::ifstream& fileStream);

VertexLabeledUndirectedGraph<std::string, true> loadUndirectedEdgeListFromTextFile(const std::string& fileName);
VertexLabeledUndirectedGraph<std::string, true> loadUndirectedEdgeListFromTextFile(std::ifstream& fileStream);
template<typename Label, bool hashable=false> VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize=0);
template<typename Label, bool hashable=false> void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ifstream& fileStream, size_t byteSize=0);

VertexLabeledDirectedGraph<std::string, true> loadDirectedEdgeListFromTextFile(const std::string& fileName);
VertexLabeledDirectedGraph<std::string, true> loadDirectedEdgeListFromTextFile(std::ifstream& fileStream);
template<typename Label, bool hashable=false> VertexLabeledDirectedGraph<Label, hashable> loadDirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> VertexLabeledDirectedGraph<Label, hashable> loadDirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize=0);
template<typename Label, bool hashable=false> void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize=0);
template<typename Label, bool hashable=false> void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<Label, hashable>& graph, std::ifstream& fileStream, size_t byteSize=0);




// VertexLabeledDirectedGraph

template<typename Label, bool hashable>
VertexLabeledDirectedGraph<Label, hashable> loadDirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize){
    VertexLabeledDirectedGraph<Label, hashable> returnedGraph;

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    returnedGraph = loadDirectedEdgeListFromBinaryFile<Label, hashable>(fileStream, byteSize);
    fileStream.close();

    return returnedGraph;
}

template<typename Label, bool hashable>
VertexLabeledDirectedGraph<Label, hashable> loadDirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");
    VertexLabeledDirectedGraph<Label, hashable> returnedGraph;
    if (byteSize == 0) byteSize = sizeof(Label);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    Label vertex1, vertex2;
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

template<typename Label, bool hashable>
void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    addVerticesFromBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename Label, bool hashable>
void addVerticesFromBinaryFile(VertexLabeledDirectedGraph<Label, hashable>& graph, std::ifstream& fileStream, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");

    if (byteSize == 0) byteSize = sizeof(Label);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    Label vertex;
    while (fileStream.read((char*) &vertex, byteSize)){
        graph.addVertex(vertex);
    }
}

template<typename Label, bool hashable>
void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    writeEdgeListInTextFile(graph, fileStream);
    fileStream.close();
}

template<typename Label, bool hashable>
void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<bool hashable>
inline void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<unsigned char, hashable>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<bool hashable>
inline void writeEdgeListInTextFile(const VertexLabeledDirectedGraph<signed char, hashable>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<typename Label, bool hashable>
void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    writeEdgeListInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename Label, bool hashable>
void writeEdgeListInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");

    if (byteSize == 0) byteSize = sizeof(Label);

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

template<typename Label, bool hashable>
void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName, std::ios::binary);
    writeVerticesInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename Label, bool hashable>
void writeVerticesInBinaryFile(const VertexLabeledDirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    auto& vertices = graph.getVertices();
    for (auto& vertex: vertices)
        fileStream.write((char*) &vertex, byteSize);
}


// VertexLabeledUndirectedGraph

template<typename Label, bool hashable>
VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize){
    VertexLabeledUndirectedGraph<Label, hashable> returnedGraph;

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    returnedGraph = loadUndirectedEdgeListFromBinaryFile<Label>(fileStream, byteSize);
    fileStream.close();

    return returnedGraph;
}

template<typename Label, bool hashable>
VertexLabeledUndirectedGraph<Label, hashable> loadUndirectedEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");
    VertexLabeledUndirectedGraph<Label, hashable> returnedGraph;
    if (byteSize == 0) byteSize = sizeof(Label);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    Label vertex1, vertex2;
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

template<typename Label, bool hashable>
void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    addVerticesFromBinaryFile<Label>(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename Label, bool hashable>
void addVerticesFromBinaryFile(VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ifstream& fileStream, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to read binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    Label vertex;
    while (fileStream.read((char*) &vertex, byteSize)){
        graph.addVertex(vertex);
    }
}

template<typename Label, bool hashable>
void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName){
    std::ofstream fileStream(fileName.c_str());
    writeEdgeListInTextFile(graph, fileStream);
    fileStream.close();
}

template<typename Label, bool hashable>
void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            if (i<j) fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<bool hashable>
inline void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<unsigned char, hashable>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            if (i<j) fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<bool hashable>
inline void writeEdgeListInTextFile(const VertexLabeledUndirectedGraph<signed char, hashable>& graph, std::ofstream& fileStream){
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            if (i<j) fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<typename Label, bool hashable>
void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    writeEdgeListInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename Label, bool hashable>
void writeEdgeListInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize) {
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

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

template<typename Label, bool hashable>
void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, const std::string& fileName, size_t byteSize){
    std::ofstream fileStream(fileName, std::ios::binary);
    writeVerticesInBinaryFile(graph, fileStream, byteSize);
    fileStream.close();
}

template<typename Label, bool hashable>
void writeVerticesInBinaryFile(const VertexLabeledUndirectedGraph<Label, hashable>& graph, std::ofstream& fileStream, size_t byteSize){
    static_assert(!std::is_same<Label, std::string>::value, "No implementation of string to write binary file");
    if (byteSize == 0) byteSize = sizeof(Label);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    auto& vertices = graph.getVertices();
    for (const VertexIndex& i: graph)
        fileStream.write((char*) &vertices[i], byteSize);
}


} // namespace BaseGraph


#endif
