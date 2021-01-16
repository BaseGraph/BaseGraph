#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>

#include "pgl/undirectedgraph.h"


using namespace std;

namespace PGL{


bool UndirectedGraph::operator==(const UndirectedGraph& other) const{
    bool sameObject = size == other.size && edgeNumber == other.edgeNumber;

    list<size_t>::const_iterator it;
    for (size_t i=0; i<size && sameObject; ++i){
        for (it=adjacencyList[i].begin(); it != adjacencyList[i].end() && sameObject; ++it){
            if (!other.isEdgeIdx(i, *it))
                sameObject = false;
        }

        for (it=other.adjacencyList[i].begin(); it != other.adjacencyList[i].end() && sameObject; ++it){
            if (!isEdgeIdx(i, *it))
                sameObject = false;
        }
    }
    return sameObject;
}

vector<size_t> UndirectedGraph::getDegrees() const{
    vector<size_t> degrees(size);
    for (size_t i=0; i<size; i++)
        degrees[i] = adjacencyList[i].size();
    return degrees;
}

void UndirectedGraph::addEdgeIdx(size_t source, size_t destination, bool force){
    if (source >= size || destination >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    // Separating the conditions avoids the loop in isEdgeIdx when force=true
    if (force) {
        adjacencyList[source].push_back(destination);
        adjacencyList[destination].push_back(source);
        edgeNumber++;
    }
    else if (!isEdgeIdx(source, destination)) {
        adjacencyList[source].push_back(destination);
        adjacencyList[destination].push_back(source);
        edgeNumber++;
    }
}

void UndirectedGraph::removeEdgeIdx(size_t source, size_t destination){
    if (source >= size || destination >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    size_t sizeBefore = adjacencyList[source].size();

    adjacencyList[source].remove(destination);
    adjacencyList[destination].remove(source);

    edgeNumber -= sizeBefore - adjacencyList[source].size();
}

void UndirectedGraph::removeVertexFromEdgeListIdx(size_t vertex) {
    list<size_t> neigbours = getNeighboursOfIdx(vertex);
    for (size_t& neighbour: getNeighboursOfIdx(vertex))
        removeEdgeIdx(neighbour, vertex);

    adjacencyList[vertex].clear();
}

void UndirectedGraph::removeMultiedges() {
    list<size_t> seenVertices;
    list<size_t>::iterator j;
    for (size_t i=0; i<size; ++i){
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (find(seenVertices.begin(), seenVertices.end(), *j) != seenVertices.end()) {
                 if (i >= *j) {
                    edgeNumber--;
                 }
                adjacencyList[i].erase(j++);
            }
            else {
                seenVertices.push_back(*j);
                j++;
            }
        }
        seenVertices.clear();
    }
}


void UndirectedGraph::writeEdgeListIdxInTextFile(const string& fileName) const{
    ofstream fileStream(fileName);
    writeEdgeListIdxInTextFile(fileStream);
    fileStream.close();
}

void UndirectedGraph::writeEdgeListIdxInTextFile(ofstream& fileStream) const{
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (size_t i=0; i<size; ++i)
        for (size_t& j: getNeighboursOfIdx(i))
            if (i<j) fileStream << i << "   " << j << '\n'; // i < *j to write edges only once
}

void UndirectedGraph::writeEdgeListIdxInBinaryFile(const string& fileName) const{
    ofstream fileStream(fileName, ios::out|ios::binary);
    writeEdgeListIdxInBinaryFile(fileStream);
    fileStream.close();
}

void UndirectedGraph::writeEdgeListIdxInBinaryFile(ofstream& fileStream) const{
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    size_t byteSize = sizeof(size_t);

    for (size_t i=0; i<size; ++i){
        for (size_t& j: getNeighboursOfIdx(i)) {
            if (i <= j) { // write edges once
                fileStream.write((char*) &i, byteSize);
                fileStream.write((char*) &j, byteSize);
            }
        }
    }
}

UndirectedGraph UndirectedGraph::loadEdgeListIdxFromTextFile(const string& fileName){
    UndirectedGraph returnedGraph;
    ifstream fileStream(fileName);
    returnedGraph = loadEdgeListIdxFromTextFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

UndirectedGraph UndirectedGraph::loadEdgeListIdxFromTextFile(ifstream& fileStream){
    UndirectedGraph returnedGraph;

    stringstream currentLine;
    string full_line, strVertexIdx, strVertex2Idx;
    size_t vertexIdx, vertex2Idx;

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
            if (vertexIdx > returnedGraph.getSize()) returnedGraph.resize(vertexIdx);
            if (vertex2Idx > returnedGraph.getSize()) returnedGraph.resize(vertex2Idx);
            returnedGraph.addEdgeIdx(vertexIdx, vertex2Idx);
        }
    }
    return returnedGraph;
}

UndirectedGraph UndirectedGraph::loadEdgeListIdxFromBinaryFile(const string& fileName){
    UndirectedGraph returnedGraph;
    ifstream fileStream(fileName, ios::out|ios::binary);
    returnedGraph = loadEdgeListIdxFromBinaryFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

UndirectedGraph UndirectedGraph::loadEdgeListIdxFromBinaryFile(ifstream& fileStream){
    UndirectedGraph returnedGraph;

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    size_t i = 0;
    size_t vertex1, vertex2;
    size_t byteSize = sizeof(size_t);
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 > returnedGraph.getSize()) returnedGraph.resize(vertex2);
        if (i % 2 == 1){
            returnedGraph.addEdgeIdx(vertex1, vertex2);
            returnedGraph.addEdgeIdx(vertex2, vertex1);
        }
        vertex1 = vertex2;
        ++i;
    }
    return returnedGraph;
}

} // namespace PGL
