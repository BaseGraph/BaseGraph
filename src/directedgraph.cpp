#include <fstream>
#include <sstream>
#include <string>

#include "pgl/directedgraph.h"


using namespace std;
using namespace PGL;

namespace PGL{

DirectedGraph::DirectedGraph(const DirectedGraph& source){
    size = 0;
    resize(source.size);
    edgeNumber = source.edgeNumber;
    adjacencyList = source.adjacencyList;
}

DirectedGraph DirectedGraph::operator=(const DirectedGraph& source){
    if (this != &source){
        size = 0;
        resize(source.size);
        edgeNumber = source.edgeNumber;
        adjacencyList = source.adjacencyList;
    }
    return *this;
}

bool DirectedGraph::operator==(const DirectedGraph& other) const{
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

void DirectedGraph::resize(size_t newSize){
    if (newSize < size) throw invalid_argument("Graph's size cannot be reduced.");
    size = newSize;
    adjacencyList.resize(newSize, list<size_t>());
}

list<size_t> DirectedGraph::getOutEdgesOfIdx(size_t vertex) const{
    if (vertex >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    return adjacencyList[vertex];
}

vector<list<size_t>> DirectedGraph::getInEdgesOfVertices() const{
    vector<list<size_t>> inEdges(size);
    
    for (size_t i=0; i<size; i++)
        for (const size_t& j: getOutEdgesOfIdx(i))
            inEdges[j].push_back(i);
    return inEdges;
}

void DirectedGraph::addEdgeIdx(size_t source, size_t destination, bool force){
    if (source >= size || destination >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    // Separating the conditions avoids the loop in isEdgeIdx when force=true
    if (force) {
        adjacencyList[source].push_back(destination);
        edgeNumber++;
    }
    else if (!isEdgeIdx(source, destination)) {
        adjacencyList[source].push_back(destination);
        edgeNumber++;
    }
}

void DirectedGraph::removeEdgeIdx(size_t source, size_t destination) {
    if (source >= size || destination >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    size_t sizeBefore = adjacencyList[source].size();
    adjacencyList[source].remove(destination);
    edgeNumber -= sizeBefore - adjacencyList[source].size();
}

bool DirectedGraph::isEdgeIdx(size_t source, size_t destination) const{
    if (source >= size || destination >= size)throw invalid_argument("Vertex index greater than the graph's size.");
    
    const auto& outEdges = getOutEdgesOfIdx(source);
    return find(outEdges.begin(), outEdges.end(), destination) != outEdges.end();
}

void DirectedGraph::removeMultiedges(){
    list<size_t> seenVertices;
    list<size_t>::iterator j;
    for (size_t i=0; i<size; ++i){
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (find(seenVertices.begin(), seenVertices.end(), *j) != seenVertices.end()) {
                adjacencyList[i].erase(j++);
                edgeNumber--;
            }
            else {
                seenVertices.push_back(*j);
                j++;
            }
        }
        seenVertices.clear();
    }
}

void DirectedGraph::removeVertexFromEdgeListIdx(size_t vertex){
    if (vertex >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    size_t sizeBefore;

    for (size_t i=0; i<size; ++i){
        sizeBefore = adjacencyList[i].size();

        if (i == vertex)
            adjacencyList[i].clear();
        else
            adjacencyList[i].remove(vertex);
        edgeNumber -= sizeBefore - adjacencyList[i].size();
    }
}

vector<vector<size_t>> DirectedGraph::getAdjacencyMatrix() const{
    vector<vector<size_t>> adjacencyMatrix;
    adjacencyMatrix.resize(size, vector<size_t>(size));

    size_t j;
    for (size_t i=0; i<size; ++i){
        for (size_t& j: getOutEdgesOfIdx(i)) {
            adjacencyMatrix[i][j] += 1;
            j++;
        }
    }
    return adjacencyMatrix;
}

size_t DirectedGraph::getInDegreeIdx(size_t vertex) const{
    if (vertex >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    size_t inDegree = 0;

    for (size_t i=0; i<size; ++i)
        for (size_t& j: getOutEdgesOfIdx(i))
            if (j == vertex) inDegree++;
    return inDegree;
}

vector<size_t> DirectedGraph::getInDegrees() const {
    vector<size_t> inDegrees(size, 0);

    for (size_t i=0; i<size; i++){
        for (size_t& j: getOutEdgesOfIdx(i))
            inDegrees[j]++;
    }

    return inDegrees;
}

size_t DirectedGraph::getOutDegreeIdx(size_t vertex) const{
    if (vertex >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    return adjacencyList[vertex].size();
}

vector<size_t> DirectedGraph::getOutDegrees() const {
    vector<size_t> outDegrees(size, 0);

    for (size_t i=0; i<size; i++)
        outDegrees[i] += getOutDegreeIdx(i);
    return outDegrees;
}

void DirectedGraph::writeEdgeListIdxInTextFile(const string& fileName) const{
    ofstream fileStream(fileName);
    writeEdgeListIdxInTextFile(fileStream);
    fileStream.close();
}

void DirectedGraph::writeEdgeListIdxInTextFile(ofstream& fileStream) const{
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (size_t i=0; i<size; ++i)
        for (size_t& j: getOutEdgesOfIdx(i))
            fileStream << i << "   " << j << '\n';
}

void DirectedGraph::writeEdgeListIdxInBinaryFile(const string& fileName) const{
    ofstream fileStream(fileName, ios::out|ios::binary);
    writeEdgeListIdxInBinaryFile(fileStream);
    fileStream.close();
}

void DirectedGraph::writeEdgeListIdxInBinaryFile(ofstream& fileStream) const{
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    size_t byteSize = sizeof(size_t);

    for (size_t i=0; i<size; ++i){
        for (size_t& j: getOutEdgesOfIdx(i)) {
            fileStream.write((char*) &i, byteSize);
            fileStream.write((char*) &j, byteSize);
        }
    }
}

DirectedGraph DirectedGraph::loadEdgeListIdxFromTextFile(const string& fileName){
    DirectedGraph returnedGraph(0);
    ifstream fileStream(fileName);
    returnedGraph = loadEdgeListIdxFromTextFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

DirectedGraph DirectedGraph::loadEdgeListIdxFromTextFile(ifstream& fileStream){
    DirectedGraph returnedGraph;

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

DirectedGraph DirectedGraph::loadEdgeListIdxFromBinaryFile(const string& fileName){
    DirectedGraph returnedGraph(0);
    ifstream fileStream(fileName, ios::out|ios::binary);
    returnedGraph = loadEdgeListIdxFromBinaryFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

DirectedGraph DirectedGraph::loadEdgeListIdxFromBinaryFile(ifstream& fileStream){
    DirectedGraph returnedGraph;

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    size_t i = 0;
    size_t vertex1, vertex2;
    size_t byteSize = sizeof(size_t);
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 > returnedGraph.getSize()) returnedGraph.resize(vertex2);
        if (i % 2 == 1)
            returnedGraph.addEdgeIdx(vertex1, vertex2);
        ++i;
        vertex1 = vertex2;
    }
    return returnedGraph;
}


} // namespace PGL
