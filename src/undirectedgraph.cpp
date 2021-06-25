#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <unordered_map>
#include <algorithm>

#include "BaseGraph/undirectedgraph.h"


using namespace std;


namespace BaseGraph{


UndirectedGraph::UndirectedGraph(const DirectedGraph& directedgraph): DirectedGraph(directedgraph.getSize()) {
    for (VertexIndex i: directedgraph)
        for (VertexIndex j: directedgraph.getOutEdgesOfIdx(i))
            addEdgeIdx(i, j);
}

DirectedGraph UndirectedGraph::getDirectedGraph() const {
    DirectedGraph directedGraph(size);

    for (VertexIndex i: *this)
        for (VertexIndex j: getNeighboursOfIdx(i))
            if (i<j)
                directedGraph.addReciprocalEdgeIdx(i, j, true);

    return directedGraph;
}

bool UndirectedGraph::operator==(const UndirectedGraph& other) const{
    bool sameObject = size == other.size && edgeNumber == other.edgeNumber;

    list<VertexIndex>::const_iterator it;
    for (VertexIndex i=0; i<size && sameObject; ++i){
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
    for (VertexIndex i: *this)
        degrees[i] = adjacencyList[i].size();
    return degrees;
}

void UndirectedGraph::addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force){
    if (vertex1 >= size || vertex2 >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    if (force || !isEdgeIdx(vertex1, vertex2)) {
        adjacencyList[vertex1].push_back(vertex2);
        adjacencyList[vertex2].push_back(vertex1);
        edgeNumber++;
    }
}

bool UndirectedGraph::isEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) const {
    if (vertex1 >= size || vertex2 >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    if (adjacencyList[vertex1].size() < adjacencyList[vertex2].size())
        return DirectedGraph::isEdgeIdx(vertex1, vertex2);
    else
        return DirectedGraph::isEdgeIdx(vertex2, vertex1);
}

void UndirectedGraph::removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2){
    if (vertex1 >= size || vertex2 >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    size_t sizeBefore = adjacencyList[vertex1].size();

    adjacencyList[vertex1].remove(vertex2);
    size_t sizeDifference = sizeBefore - adjacencyList[vertex1].size();
    
    if (sizeDifference > 0) {
        adjacencyList[vertex2].remove(vertex1);
        edgeNumber -= sizeDifference;
    }
}

void UndirectedGraph::removeVertexFromEdgeListIdx(VertexIndex vertex) {
    if (vertex >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    for (const VertexIndex& neighbour: getNeighboursOfIdx(vertex))
        DirectedGraph::removeEdgeIdx(neighbour, vertex);  // Also takes care of edgeNumber update

    adjacencyList[vertex].clear();
}

void UndirectedGraph::removeMultiedges() {
    list<VertexIndex> seenVertices;
    list<VertexIndex>::iterator j;

    for (VertexIndex i: *this) {
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (find(seenVertices.begin(), seenVertices.end(), *j) != seenVertices.end()) {
                 if (i < *j) {
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

UndirectedGraph UndirectedGraph::getSubgraph(const std::unordered_set<VertexIndex>& vertices) const{
    UndirectedGraph subgraph(size);

    for (VertexIndex i: vertices) {
        if (i >= size) throw invalid_argument("Vertex index greater than the graph's size.");

        for (VertexIndex j: getOutEdgesOfIdx(i))
            if (i < j && vertices.find(j) != vertices.end())
                subgraph.addEdgeIdx(i, j, true);
    }

    return subgraph;
}

pair<UndirectedGraph, unordered_map<VertexIndex, VertexIndex>> UndirectedGraph::getSubgraphWithRemap(const std::unordered_set<VertexIndex>& vertices) const{
    UndirectedGraph subgraph(vertices.size());

    unordered_map<VertexIndex, VertexIndex> newMapping;

    VertexIndex position=0;
    for (VertexIndex vertex: vertices) {
        newMapping[vertex] = position;
        position++;
    }

    for (VertexIndex i: vertices) {
        if (i >= size) throw invalid_argument("Vertex index greater than the graph's size.");

        for (VertexIndex j: getOutEdgesOfIdx(i))
            if (i < j && vertices.find(j) != vertices.end())
                subgraph.addEdgeIdx(newMapping[i], newMapping[j], true);
    }

    return {subgraph, newMapping};
}


} // namespace BaseGraph
