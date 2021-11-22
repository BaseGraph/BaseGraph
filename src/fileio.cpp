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
    VertexLabeledDirectedGraph<std::string, true> returnedGraph;

    std::ifstream fileStream(fileName.c_str());
    returnedGraph = loadDirectedEdgeListFromTextFile(fileStream);
    fileStream.close();

    return returnedGraph;
}

VertexLabeledDirectedGraph<std::string, true> loadDirectedEdgeListFromTextFile(std::ifstream& fileStream){
    VertexLabeledDirectedGraph<std::string, true> returnedGraph;

    std::stringstream currentLine;
    std::string full_line, name1_str, name2_str;

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");
    else {
        while( std::getline(fileStream, full_line)){
            fileStream >> std::ws;
            currentLine.str(full_line);
            currentLine >> std::ws;
            currentLine >> name1_str >> std::ws;

            // Skips a line of comment.
            if(name1_str == "#") {
                currentLine.clear();
                continue;
            }
            currentLine >> name2_str >> std::ws;
            currentLine.clear();

            returnedGraph.addVertex(name1_str);
            returnedGraph.addVertex(name2_str);
            returnedGraph.addEdge(name1_str, name2_str);
        }
    }
    return returnedGraph;
}


// VertexLabeledUndirectedGraph

VertexLabeledUndirectedGraph<std::string, true> loadUndirectedEdgeListFromTextFile(const std::string& fileName){
    VertexLabeledUndirectedGraph<std::string, true> returnedGraph;

    std::ifstream fileStream(fileName.c_str());
    returnedGraph = loadUndirectedEdgeListFromTextFile(fileStream);
    fileStream.close();

    return returnedGraph;
}

VertexLabeledUndirectedGraph<std::string, true> loadUndirectedEdgeListFromTextFile(std::ifstream& fileStream){
    VertexLabeledUndirectedGraph<std::string, true> returnedGraph;

    std::stringstream currentLine;
    std::string full_line, name1_str, name2_str;

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");
    else {
        while( std::getline(fileStream, full_line)){
            fileStream >> std::ws;
            currentLine.str(full_line);
            currentLine >> std::ws;
            currentLine >> name1_str >> std::ws;

            // Skips a line of comment.
            if(name1_str == "#") {
                currentLine.clear();
                continue;
            }
            currentLine >> name2_str >> std::ws;
            currentLine.clear();

            returnedGraph.addVertex(name1_str);
            returnedGraph.addVertex(name2_str);
            returnedGraph.addEdge(name1_str, name2_str);
        }
    }
    return returnedGraph;
}

// DirectedGraph

void writeEdgeListIdxInTextFile(const DirectedGraph& graph, const string& fileName, size_t vertexIndexShift) {
    ofstream fileStream(fileName);
    writeEdgeListIdxInTextFile(graph, fileStream, vertexIndexShift);
    fileStream.close();
}

void writeEdgeListIdxInTextFile(const DirectedGraph& graph, ofstream& fileStream, size_t vertexIndexShift){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (const VertexIndex i: graph)
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i))
            fileStream << i + vertexIndexShift << " " << j + vertexIndexShift << '\n';
}

void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName, ios::out|ios::binary);
    writeEdgeListIdxInBinaryFile(graph, fileStream);
    fileStream.close();
}

void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, ofstream& fileStream){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    size_t byteSize = sizeof(size_t);

    for (const VertexIndex& i: graph){
        for (const VertexIndex& j: graph.getOutEdgesOfIdx(i)) {
            fileStream.write((char*) &i, byteSize);
            fileStream.write((char*) &j, byteSize);
        }
    }
}

DirectedGraph loadDirectedEdgeListIdxFromTextFile(const string& fileName){
    DirectedGraph returnedGraph(0);
    ifstream fileStream(fileName);
    returnedGraph = loadDirectedEdgeListIdxFromTextFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

DirectedGraph loadDirectedEdgeListIdxFromTextFile(ifstream& fileStream){
    DirectedGraph returnedGraph(0);

    stringstream currentLine;
    string full_line, strVertexIdx, strVertex2Idx;
    VertexIndex vertexIdx, vertex2Idx;

    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");
    else {
        while( getline(fileStream, full_line)){
            fileStream >> std::ws;
            currentLine.str(full_line);
            currentLine >> std::ws;
            currentLine >> strVertexIdx >> std::ws;

            // Skips a line of comment.
            if(strVertexIdx == "#") {
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
    }
    return returnedGraph;
}

DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(const string& fileName){
    DirectedGraph returnedGraph(0);
    ifstream fileStream(fileName, ios::out|ios::binary);
    returnedGraph = loadDirectedEdgeListIdxFromBinaryFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(ifstream& fileStream){
    DirectedGraph returnedGraph(0);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    VertexIndex i = 0;
    VertexIndex vertex1, vertex2;
    size_t byteSize = sizeof(size_t);
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 >= returnedGraph.getSize()) returnedGraph.resize(vertex2 + 1);
        if (i % 2 == 1)
            returnedGraph.addEdgeIdx(vertex1, vertex2);
        ++i;
        vertex1 = vertex2;
    }
    return returnedGraph;
}


// UndirectedGraph

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName);
    writeEdgeListIdxInTextFile(graph, fileStream);
    fileStream.close();
}

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, ofstream& fileStream){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (const VertexIndex& i: graph)
        for (const VertexIndex& j: graph.getNeighboursOfIdx(i))
            if (i<j) fileStream << i << "   " << j << '\n'; // i < *j to write edges only once
}

void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName, ios::out|ios::binary);
    writeEdgeListIdxInBinaryFile(graph, fileStream);
    fileStream.close();
}

void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, ofstream& fileStream){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    size_t byteSize = sizeof(size_t);

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
    UndirectedGraph returnedGraph(0);
    ifstream fileStream(fileName);
    returnedGraph = loadUndirectedEdgeListIdxFromTextFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(ifstream& fileStream){
    UndirectedGraph returnedGraph(0);

    stringstream currentLine;
    string full_line, strVertexIdx, strVertex2Idx;
    VertexIndex vertexIdx, vertex2Idx;

    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");
    else {
        while( getline(fileStream, full_line)){
            fileStream >> std::ws;
            currentLine.str(full_line);
            currentLine >> std::ws;
            currentLine >> strVertexIdx >> std::ws;

            // Skips a line of comment.
            if(strVertexIdx == "#") {
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
    }
    return returnedGraph;
}

UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(const string& fileName){
    UndirectedGraph returnedGraph(0);
    ifstream fileStream(fileName, ios::out|ios::binary);
    returnedGraph = loadUndirectedEdgeListIdxFromBinaryFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(ifstream& fileStream){
    UndirectedGraph returnedGraph(0);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    VertexIndex i = 0;
    VertexIndex vertex1, vertex2;
    size_t byteSize = sizeof(size_t);
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 >= returnedGraph.getSize()) returnedGraph.resize(vertex2+1);
        if (i % 2 == 1)
            returnedGraph.addEdgeIdx(vertex1, vertex2);
        vertex1 = vertex2;
        ++i;
    }
    return returnedGraph;
}

} // namespace BaseGraph
