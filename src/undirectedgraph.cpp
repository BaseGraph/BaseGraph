#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <unordered_map>
#include <algorithm>

#include "BaseGraph/types.h"
#include "BaseGraph/undirectedgraph.h"


using namespace std;


namespace BaseGraph{


UndirectedGraph::UndirectedGraph(const DirectedGraph& directedgraph): DirectedGraph(directedgraph.getSize()) {
    for (VertexIndex i: directedgraph)
        for (VertexIndex j: directedgraph.getOutEdgesOf(i))
            addEdge(i, j);
}

DirectedGraph UndirectedGraph::getDirectedGraph() const {
    DirectedGraph directedGraph(size);

    for (VertexIndex i: *this)
        for (VertexIndex j: getNeighboursOf(i))
            if (i<j)
                directedGraph.addReciprocalEdge(i, j, true);
            else if (i==j)
                directedGraph.addEdge(i, j, true);

    return directedGraph;
}

vector<size_t> UndirectedGraph::getDegrees(bool countSelfLoopsTwice) const{
    vector<size_t> degrees(size);
    for (VertexIndex i: *this)
        degrees[i] = getDegreeOf(i, countSelfLoopsTwice);
    return degrees;
}

size_t UndirectedGraph::getDegreeOf(VertexIndex vertex, bool countSelfLoopsTwice) const {
    assertVertexInRange(vertex);

    if (!countSelfLoopsTwice)
        return adjacencyList[vertex].size();

    size_t degree=0;
    for (auto neighbor: getNeighboursOf(vertex))
        degree += neighbor==vertex ? 2:1;
    return degree;
}

void UndirectedGraph::addEdge(VertexIndex vertex1, VertexIndex vertex2, bool force){
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    if (force || !hasEdge(vertex1, vertex2)) {
        if (vertex1 != vertex2)
            adjacencyList[vertex1].push_back(vertex2);
        adjacencyList[vertex2].push_back(vertex1);

        edgeNumber++;
    }
}

bool UndirectedGraph::hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    auto optimalSearchEdge = getSmallestAdjacency(vertex1, vertex2);
    return DirectedGraph::hasEdge(optimalSearchEdge.first, optimalSearchEdge.second);
}

void UndirectedGraph::removeEdge(VertexIndex vertex1, VertexIndex vertex2){
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    size_t sizeBefore = adjacencyList[vertex1].size();

    adjacencyList[vertex1].remove(vertex2);
    size_t sizeDifference = sizeBefore - adjacencyList[vertex1].size();

    if (sizeDifference > 0) {
        adjacencyList[vertex2].remove(vertex1);
        edgeNumber -= sizeDifference;
    }
}

void UndirectedGraph::removeVertexFromEdgeList(VertexIndex vertex) {
    assertVertexInRange(vertex);


    for (const VertexIndex& neighbour: getNeighboursOf(vertex)) {
        if (neighbour != vertex)
            DirectedGraph::removeEdge(neighbour, vertex);  // Takes care of edgeNumber update
        else
            // Calling DirectedGraph::removeEdge breaks the current iterator. Only the
            // edgeNumber must be accounted for here. The adjacency of "vertex" is emptied later
            edgeNumber--;
    }

    adjacencyList[vertex].clear();
}

void UndirectedGraph::removeDuplicateEdges() {
    list<VertexIndex> seenVertices;
    list<VertexIndex>::iterator j;

    for (VertexIndex i: *this) {
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (find(seenVertices.begin(), seenVertices.end(), *j) != seenVertices.end()) {
                 if (i <= *j) {
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

std::vector<Edge> UndirectedGraph::getEdges() const {
    std::vector<Edge> edges;
    edges.reserve(getEdgeNumber());

    for (auto vertex: *this)
        for (auto neighbour: getOutEdgesOf(vertex))
            if (vertex<=neighbour)
                edges.push_back({vertex, neighbour});

    return edges;
}

AdjacencyMatrix UndirectedGraph::getAdjacencyMatrix() const{
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(size, vector<size_t>(size, 0));

    for (VertexIndex i=0; i<size; ++i)
        for (const VertexIndex& j: getOutEdgesOf(i))
            adjacencyMatrix[i][j] += i!=j ? 1:2;

    return adjacencyMatrix;
}

UndirectedGraph UndirectedGraph::getSubgraphOf(const std::unordered_set<VertexIndex>& vertices) const{
    UndirectedGraph subgraph(size);

    for (VertexIndex i: vertices) {
        assertVertexInRange(i);

        for (VertexIndex j: getOutEdgesOf(i))
            if (i <= j && vertices.find(j) != vertices.end())
                subgraph.addEdge(i, j, true);
    }

    return subgraph;
}

pair<UndirectedGraph, unordered_map<VertexIndex, VertexIndex> > UndirectedGraph::getSubgraphWithRemapOf(const std::unordered_set<VertexIndex>& vertices) const{
    UndirectedGraph subgraph(vertices.size());

    unordered_map<VertexIndex, VertexIndex> newMapping;

    VertexIndex position=0;
    for (VertexIndex vertex: vertices) {
        newMapping[vertex] = position;
        position++;
    }

    for (VertexIndex i: vertices) {
        assertVertexInRange(i);

        for (VertexIndex j: getOutEdgesOf(i))
            if (i <= j && vertices.find(j) != vertices.end())
                subgraph.addEdge(newMapping[i], newMapping[j], true);
    }

    return {subgraph, newMapping};
}


} // namespace BaseGraph
