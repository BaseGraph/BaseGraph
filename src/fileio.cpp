#include "BaseGraph/fileio.h"

#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>


using namespace std;


namespace BaseGraph{


// VertexLabeledDirectedGraph

VertexLabeledDirectedGraph<std::string, true> loadDirectedEdgeListFromTextFile(const std::string& fileName){
    std::ifstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    VertexLabeledDirectedGraph<std::string, true> returnedGraph;

    std::stringstream currentLine;
    std::string fullLine, name1_str, name2_str;

    while ( std::getline(fileStream, fullLine) ){
        fileStream >> std::ws;
        currentLine.str(fullLine);
        currentLine >> std::ws;
        currentLine >> name1_str >> std::ws;

        // Skips lines of comment.
        if (name1_str == "#") {
            currentLine.clear();
            continue;
        }
        currentLine >> name2_str >> std::ws;
        currentLine.clear();

        returnedGraph.addVertex(name1_str);
        returnedGraph.addVertex(name2_str);
        returnedGraph.addEdge(name1_str, name2_str);
    }
    return returnedGraph;
}


// VertexLabeledUndirectedGraph

VertexLabeledUndirectedGraph<std::string, true> loadUndirectedEdgeListFromTextFile(const std::string& fileName){
    std::ifstream fileStream(fileName.c_str());
    verifyStreamOpened(fileStream, fileName);

    VertexLabeledUndirectedGraph<std::string, true> returnedGraph;

    std::stringstream currentLine;
    std::string fullLine, name1_str, name2_str;

    while ( std::getline(fileStream, fullLine) ){
        fileStream >> std::ws;
        currentLine.str(fullLine);
        currentLine >> std::ws;
        currentLine >> name1_str >> std::ws;

        // Skips lines of comment.
        if (name1_str == "#") {
            currentLine.clear();
            continue;
        }
        currentLine >> name2_str >> std::ws;
        currentLine.clear();

        returnedGraph.addVertex(name1_str);
        returnedGraph.addVertex(name2_str);
        returnedGraph.addEdge(name1_str, name2_str);
    }
    return returnedGraph;
}

// DirectedGraph

void writeEdgeListIdxInTextFile(const DirectedGraph& graph, const string& fileName, size_t vertexIndexShift) {
    ofstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (const VertexIndex i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            fileStream << i + vertexIndexShift << " " << j + vertexIndexShift << '\n';
}

void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName, ios::out|ios::binary);
    verifyStreamOpened(fileStream, fileName);

    size_t byteSize = sizeof(VertexIndex);

    for (const VertexIndex& i: graph){
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i)) {
            fileStream.write((char*) &i, byteSize);
            fileStream.write((char*) &j, byteSize);
        }
    }
}

DirectedGraph loadDirectedEdgeListIdxFromTextFile(const string& fileName){
    ifstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    DirectedGraph returnedGraph(0);

    stringstream currentLine;
    string fullLine, strVertexIdx, strVertex2Idx;
    VertexIndex vertexIdx, vertex2Idx;

    while ( getline(fileStream, fullLine) ){
        fileStream >> std::ws;
        currentLine.str(fullLine);
        currentLine >> std::ws;
        currentLine >> strVertexIdx >> std::ws;

        // Skips lines of comment.
        if (strVertexIdx == "#") {
            currentLine.clear();
            continue;
        }

        currentLine >> strVertex2Idx >> std::ws;
        currentLine.clear();

        vertexIdx = stoi(strVertexIdx);
        vertex2Idx = stoi(strVertex2Idx);
        if (vertexIdx >= returnedGraph.getSize()) returnedGraph.resize(vertexIdx + 1);
        if (vertex2Idx >= returnedGraph.getSize()) returnedGraph.resize(vertex2Idx + 1);
        returnedGraph.addEdgeIdx(vertexIdx, vertex2Idx);
    }
    return returnedGraph;
}

DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(const string& fileName){
    ifstream fileStream(fileName, ios::out|ios::binary);
    verifyStreamOpened(fileStream, fileName);
    size_t byteSize = sizeof(VertexIndex);

    DirectedGraph returnedGraph(0);

    bool addEdge = false;
    VertexIndex vertex1, vertex2;
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 >= returnedGraph.getSize()) returnedGraph.resize(vertex2 + 1);
        if (addEdge)
            returnedGraph.addEdgeIdx(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}


// UndirectedGraph

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            if (i<j) fileStream << i << "   " << j << '\n'; // i < *j to write edges only once
}

void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName, ios::out|ios::binary);
    verifyStreamOpened(fileStream, fileName);

    size_t byteSize = sizeof(VertexIndex);

    for (const VertexIndex& i: graph) {
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i)) {
            if (i <= j) { // write edges once
                fileStream.write((char*) &i, byteSize);
                fileStream.write((char*) &j, byteSize);
            }
        }
    }
}

UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(const string& fileName){
    ifstream fileStream(fileName);
    verifyStreamOpened(fileStream, fileName);

    UndirectedGraph returnedGraph(0);

    stringstream currentLine;
    string fullLine, strVertexIdx, strVertex2Idx;
    VertexIndex vertexIdx, vertex2Idx;

    while ( getline(fileStream, fullLine) ){
        fileStream >> std::ws;
        currentLine.str(fullLine);
        currentLine >> std::ws;
        currentLine >> strVertexIdx >> std::ws;

        // Skips lines of comment.
        if (strVertexIdx == "#") {
            currentLine.clear();
            continue;
        }
        currentLine >> strVertex2Idx >> std::ws;
        currentLine.clear();

        vertexIdx = stoi(strVertexIdx);
        vertex2Idx = stoi(strVertex2Idx);
        if (vertexIdx >= returnedGraph.getSize()) returnedGraph.resize(vertexIdx+1);
        if (vertex2Idx >= returnedGraph.getSize()) returnedGraph.resize(vertex2Idx+1);
        returnedGraph.addEdgeIdx(vertexIdx, vertex2Idx);
    }
    return returnedGraph;
}

UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(const string& fileName){
    ifstream fileStream(fileName, ios::out|ios::binary);
    verifyStreamOpened(fileStream, fileName);
    size_t byteSize = sizeof(VertexIndex);

    UndirectedGraph returnedGraph(0);

    bool addEdge = false;
    VertexIndex vertex1, vertex2;
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 >= returnedGraph.getSize()) returnedGraph.resize(vertex2+1);
        if (addEdge)
            returnedGraph.addEdgeIdx(vertex1, vertex2);
        vertex1 = vertex2;
        addEdge = !addEdge;
    }
    return returnedGraph;
}

} // namespace BaseGraph
