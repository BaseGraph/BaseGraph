#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

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

vector<list<size_t> > DirectedGraph::getInEdgesOfVertices() const{
    vector<list<size_t> > inEdges(size);

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
void DirectedGraph::removeSelfLoops() {
    size_t previousSize;
    for (size_t& i: *this) {
        previousSize = adjacencyList[i].size();
        adjacencyList[i].remove(i);
        edgeNumber -= previousSize-adjacencyList[i].size();
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

vector<vector<size_t> > DirectedGraph::getAdjacencyMatrix() const{
    vector<vector<size_t> > adjacencyMatrix;
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


} // namespace PGL
