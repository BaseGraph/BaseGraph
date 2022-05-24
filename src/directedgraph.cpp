#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "BaseGraph/directedgraph.h"


using namespace std;


namespace BaseGraph{


bool DirectedGraph::operator==(const DirectedGraph& other) const{
    bool isEqual = size == other.size && edgeNumber == other.edgeNumber;

    list<VertexIndex>::const_iterator it;
    for (VertexIndex i=0; i<size && isEqual; ++i){
        for (it=adjacencyList[i].begin(); it != adjacencyList[i].end() && isEqual; ++it){
            if (!other.hasEdgeIdx(i, *it))
                isEqual = false;
        }

        for (it=other.adjacencyList[i].begin(); it != other.adjacencyList[i].end() && isEqual; ++it){
            if (!hasEdgeIdx(i, *it))
                isEqual = false;
        }
    }
    return isEqual;
}

void DirectedGraph::resize(size_t newSize){
    if (newSize < size) throw invalid_argument("Graph's size cannot be reduced.");
    size = newSize;
    adjacencyList.resize(newSize, list<VertexIndex>());
}

AdjacencyLists DirectedGraph::getInEdges() const{
    AdjacencyLists inEdges(size);

    for (VertexIndex i=0; i<size; i++)
        for (const VertexIndex& j: getOutEdgesOfIdx(i))
            inEdges[j].push_back(i);
    return inEdges;
}

void DirectedGraph::addEdgeIdx(VertexIndex source, VertexIndex destination, bool force){
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (force || !hasEdgeIdx(source, destination)) {
        adjacencyList[source].push_back(destination);
        edgeNumber++;
    }
}

void DirectedGraph::removeEdgeIdx(VertexIndex source, VertexIndex destination) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    size_t sizeBefore = adjacencyList[source].size();
    adjacencyList[source].remove(destination);
    edgeNumber -= sizeBefore - adjacencyList[source].size();
}

bool DirectedGraph::hasEdgeIdx(VertexIndex source, VertexIndex destination) const{
    assertVertexInRange(source);
    assertVertexInRange(destination);

    const auto& outEdges = getOutEdgesOfIdx(source);
    return find(outEdges.begin(), outEdges.end(), destination) != outEdges.end();
}

void DirectedGraph::removeDuplicateEdges(){
    list<VertexIndex> seenVertices;
    Successors::iterator j;
    for (VertexIndex i=0; i<size; ++i){
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
    for (VertexIndex& i: *this) {
        previousSize = adjacencyList[i].size();
        adjacencyList[i].remove(i);
        edgeNumber -= previousSize-adjacencyList[i].size();
    }
}

void DirectedGraph::removeVertexFromEdgeListIdx(VertexIndex vertex){
    assertVertexInRange(vertex);
    size_t sizeBefore;

    for (VertexIndex i=0; i<size; ++i){
        sizeBefore = adjacencyList[i].size();

        if (i == vertex)
            adjacencyList[i].clear();
        else
            adjacencyList[i].remove(vertex);
        edgeNumber -= sizeBefore - adjacencyList[i].size();
    }
}

void DirectedGraph::clearEdges() {
    for (VertexIndex i: *this)
        adjacencyList[i].clear();
    edgeNumber = 0;
}

DirectedGraph DirectedGraph::getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const{
    DirectedGraph subgraph(size);

    for (VertexIndex i: vertices) {
        assertVertexInRange(i);

        for (VertexIndex j: getOutEdgesOfIdx(i))
            if (vertices.find(j) != vertices.end())
                subgraph.addEdgeIdx(i, j, true);
    }

    return subgraph;
}

pair<DirectedGraph, unordered_map<VertexIndex, VertexIndex>> DirectedGraph::getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const{
    DirectedGraph subgraph(vertices.size());

    unordered_map<VertexIndex, VertexIndex> newMapping;

    VertexIndex position=0;
    for (VertexIndex vertex: vertices) {
        newMapping[vertex] = position;
        position++;
    }

    for (VertexIndex i: vertices) {
        assertVertexInRange(i);

        for (VertexIndex j: getOutEdgesOfIdx(i))
            if (vertices.find(j) != vertices.end())
                subgraph.addEdgeIdx(newMapping[i], newMapping[j], true);
    }

    return {subgraph, newMapping};
}

AdjacencyMatrix DirectedGraph::getAdjacencyMatrix() const{
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(size, vector<size_t>(size, 0));

    for (VertexIndex i=0; i<size; ++i)
        for (const VertexIndex& j: getOutEdgesOfIdx(i))
            adjacencyMatrix[i][j] += 1;

    return adjacencyMatrix;
}

size_t DirectedGraph::getInDegreeOfIdx(VertexIndex vertex) const{
    assertVertexInRange(vertex);
    size_t inDegree = 0;

    for (VertexIndex i=0; i<size; ++i)
        for (const VertexIndex& j: getOutEdgesOfIdx(i))
            if (j == vertex) inDegree++;
    return inDegree;
}

vector<size_t> DirectedGraph::getInDegrees() const {
    vector<size_t> inDegrees(size, 0);

    for (VertexIndex i=0; i<size; i++){
        for (const VertexIndex& j: getOutEdgesOfIdx(i))
            inDegrees[j]++;
    }

    return inDegrees;
}

size_t DirectedGraph::getOutDegreeOfIdx(VertexIndex vertex) const{
    assertVertexInRange(vertex);
    return adjacencyList[vertex].size();
}

vector<size_t> DirectedGraph::getOutDegrees() const {
    vector<size_t> outDegrees(size, 0);

    for (VertexIndex i=0; i<size; i++)
        outDegrees[i] += getOutDegreeOfIdx(i);
    return outDegrees;
}

DirectedGraph DirectedGraph::getReversedGraph() const {
    DirectedGraph reversedGraph(size);

    for (VertexIndex i: *this)
        for (VertexIndex j: getOutEdgesOfIdx(i))
            reversedGraph.addEdgeIdx(j, i);

    return reversedGraph;
}

} // namespace BaseGraph
