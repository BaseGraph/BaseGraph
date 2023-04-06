#include "BaseGraph/directedgraph.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

namespace BaseGraph {

bool DirectedGraph::operator==(const DirectedGraph &other) const {
    bool isEqual = size == other.size && edgeNumber == other.edgeNumber;

    list<VertexIndex>::const_iterator it;
    for (VertexIndex i = 0; i < size && isEqual; ++i) {
        for (it = adjacencyList[i].begin();
             it != adjacencyList[i].end() && isEqual; ++it) {
            if (!other.hasEdge(i, *it))
                isEqual = false;
        }

        for (it = other.adjacencyList[i].begin();
             it != other.adjacencyList[i].end() && isEqual; ++it) {
            if (!hasEdge(i, *it))
                isEqual = false;
        }
    }
    return isEqual;
}

void DirectedGraph::resize(size_t newSize) {
    if (newSize < size)
        throw invalid_argument("Graph's size cannot be reduced.");
    size = newSize;
    adjacencyList.resize(newSize, list<VertexIndex>());
}

AdjacencyLists DirectedGraph::getInEdges() const {
    AdjacencyLists inEdges(size);

    for (VertexIndex i = 0; i < size; i++)
        for (const VertexIndex &j : getOutEdgesOf(i))
            inEdges[j].push_back(i);
    return inEdges;
}

void DirectedGraph::addEdge(VertexIndex source, VertexIndex destination,
                            bool force) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (force || !hasEdge(source, destination)) {
        adjacencyList[source].push_back(destination);
        edgeNumber++;
    }
}

void DirectedGraph::removeEdge(VertexIndex source, VertexIndex destination) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    size_t sizeBefore = adjacencyList[source].size();
    adjacencyList[source].remove(destination);
    edgeNumber -= sizeBefore - adjacencyList[source].size();
}

bool DirectedGraph::hasEdge(VertexIndex source, VertexIndex destination) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    const auto &outEdges = getOutEdgesOf(source);
    return find(outEdges.begin(), outEdges.end(), destination) !=
           outEdges.end();
}

void DirectedGraph::removeDuplicateEdges() {
    list<VertexIndex> seenVertices;
    Successors::iterator j;
    for (VertexIndex i = 0; i < size; ++i) {
        j = adjacencyList[i].begin();

        while (j != adjacencyList[i].end()) {
            if (find(seenVertices.begin(), seenVertices.end(), *j) !=
                seenVertices.end()) {
                adjacencyList[i].erase(j++);
                edgeNumber--;
            } else {
                seenVertices.push_back(*j);
                j++;
            }
        }
        seenVertices.clear();
    }
}
void DirectedGraph::removeSelfLoops() {
    size_t previousSize;
    for (VertexIndex &i : *this) {
        previousSize = adjacencyList[i].size();
        adjacencyList[i].remove(i);
        edgeNumber -= previousSize - adjacencyList[i].size();
    }
}

void DirectedGraph::removeVertexFromEdgeList(VertexIndex vertex) {
    assertVertexInRange(vertex);
    size_t sizeBefore;

    for (VertexIndex i = 0; i < size; ++i) {
        sizeBefore = adjacencyList[i].size();

        if (i == vertex)
            adjacencyList[i].clear();
        else
            adjacencyList[i].remove(vertex);
        edgeNumber -= sizeBefore - adjacencyList[i].size();
    }
}

std::vector<Edge> DirectedGraph::getEdges() const {
    std::vector<Edge> edges;
    edges.reserve(getEdgeNumber());

    for (auto vertex : *this)
        for (auto neighbour : getOutEdgesOf(vertex))
            edges.push_back({vertex, neighbour});

    return edges;
}

void DirectedGraph::clearEdges() {
    for (VertexIndex i : *this)
        adjacencyList[i].clear();
    edgeNumber = 0;
}

DirectedGraph DirectedGraph::getSubgraphOf(
    const std::unordered_set<VertexIndex> &vertices) const {
    DirectedGraph subgraph(size);

    for (VertexIndex i : vertices) {
        assertVertexInRange(i);

        for (VertexIndex j : getOutEdgesOf(i))
            if (vertices.find(j) != vertices.end())
                subgraph.addEdge(i, j, true);
    }

    return subgraph;
}

pair<DirectedGraph, unordered_map<VertexIndex, VertexIndex>>
DirectedGraph::getSubgraphWithRemapOf(
    const std::unordered_set<VertexIndex> &vertices) const {
    DirectedGraph subgraph(vertices.size());

    unordered_map<VertexIndex, VertexIndex> newMapping;

    VertexIndex position = 0;
    for (VertexIndex vertex : vertices) {
        newMapping[vertex] = position;
        position++;
    }

    for (VertexIndex i : vertices) {
        assertVertexInRange(i);

        for (VertexIndex j : getOutEdgesOf(i))
            if (vertices.find(j) != vertices.end())
                subgraph.addEdge(newMapping[i], newMapping[j], true);
    }

    return {subgraph, newMapping};
}

AdjacencyMatrix DirectedGraph::getAdjacencyMatrix() const {
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(size, vector<size_t>(size, 0));

    for (VertexIndex i = 0; i < size; ++i)
        for (const VertexIndex &j : getOutEdgesOf(i))
            adjacencyMatrix[i][j] += 1;

    return adjacencyMatrix;
}

size_t DirectedGraph::getInDegreeOf(VertexIndex vertex) const {
    assertVertexInRange(vertex);
    size_t inDegree = 0;

    for (VertexIndex i = 0; i < size; ++i)
        for (const VertexIndex &j : getOutEdgesOf(i))
            if (j == vertex)
                inDegree++;
    return inDegree;
}

vector<size_t> DirectedGraph::getInDegrees() const {
    vector<size_t> inDegrees(size, 0);

    for (VertexIndex i = 0; i < size; i++) {
        for (const VertexIndex &j : getOutEdgesOf(i))
            inDegrees[j]++;
    }

    return inDegrees;
}

size_t DirectedGraph::getOutDegreeOf(VertexIndex vertex) const {
    assertVertexInRange(vertex);
    return adjacencyList[vertex].size();
}

vector<size_t> DirectedGraph::getOutDegrees() const {
    vector<size_t> outDegrees(size, 0);

    for (VertexIndex i = 0; i < size; i++)
        outDegrees[i] += getOutDegreeOf(i);
    return outDegrees;
}

DirectedGraph DirectedGraph::getReversedGraph() const {
    DirectedGraph reversedGraph(size);

    for (VertexIndex i : *this)
        for (VertexIndex j : getOutEdgesOf(i))
            reversedGraph.addEdge(j, i);

    return reversedGraph;
}

} // namespace BaseGraph
