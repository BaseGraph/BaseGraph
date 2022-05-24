#include "BaseGraph/directed_multigraph.h"


namespace BaseGraph {


void DirectedMultigraph::addMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity multiplicity, bool force) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (multiplicity == 0)
        return;

    if (force)
        BaseClass::addEdgeIdx(source, destination, multiplicity, true);

    else if (DirectedGraph::hasEdgeIdx(source, destination)) {
        totalEdgeNumber += multiplicity;
        edgeLabels[{source, destination}] += multiplicity;
    }
    else {
        BaseClass::addEdgeIdx(source, destination, multiplicity, true);
    }
}

void DirectedMultigraph::removeMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity multiplicity) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    const auto& neighbours = getOutEdgesOfIdx(source);
    for (auto j=neighbours.begin(); j!= neighbours.end(); j++) {
        if (*j != destination)
            continue;

        auto& currentMultiplicity = edgeLabels[{source, destination}];
        if (currentMultiplicity > multiplicity) {
            currentMultiplicity -= multiplicity;
            totalEdgeNumber -= multiplicity;
        }
        else {
            edgeNumber--;
            totalEdgeNumber -= currentMultiplicity;
            adjacencyList[source].erase(j);
            edgeLabels.erase({source, destination});
        }
        break;
    }
}

EdgeMultiplicity DirectedMultigraph::getEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    return edgeLabels.count({source, destination})==0 ? 0 : getEdgeLabelOfIdx(source, destination);
}

void DirectedMultigraph::setEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity multiplicity) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (multiplicity == 0) {
        BaseClass::removeEdgeIdx(source, destination);
    }
    else if (DirectedGraph::hasEdgeIdx(source, destination)) {
        auto& currentMultiplicity = edgeLabels[{source, destination}];
        totalEdgeNumber += ((long long int) multiplicity - (long long int) currentMultiplicity);
        currentMultiplicity = multiplicity;
    }
    else {
        BaseClass::addEdgeIdx(source, destination, multiplicity, true);
    }
}

AdjacencyMatrix DirectedMultigraph::getAdjacencyMatrix() const{
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(size, std::vector<size_t>(size, 0));

    for (VertexIndex i=0; i<size; ++i)
        for (auto& j: getOutEdgesOfIdx(i))
            adjacencyMatrix[i][j] += getEdgeMultiplicityIdx(i, j);

    return adjacencyMatrix;
}

size_t DirectedMultigraph::getOutDegreeOfIdx(VertexIndex vertex) const {
    assertVertexInRange(vertex);
    size_t degree = 0;
    for (auto neighbour: adjacencyList[vertex])
        degree += getEdgeMultiplicityIdx(vertex, neighbour);
    return degree;
}

std::vector<size_t> DirectedMultigraph::getOutDegrees() const {
    std::vector<size_t> degrees(getSize(), 0);
    for (size_t vertex=0; vertex<getSize(); vertex++)
        degrees[vertex] = getOutDegreeOfIdx(vertex);
    return degrees;
}

size_t DirectedMultigraph::getInDegreeOfIdx(VertexIndex vertex) const {
    assertVertexInRange(vertex);
    size_t degree=0;

    for (VertexIndex i: *this)
        for (auto j: adjacencyList[i])
            if (j == vertex)
                degree += getEdgeLabelOfIdx(i, j);
    return degree;
}

std::vector<size_t> DirectedMultigraph::getInDegrees() const {
    std::vector<size_t> degrees(getSize(), 0);
    for (size_t vertex=0; vertex<getSize(); vertex++)
        for (auto neighbour: BaseClass::getOutEdgesOfIdx(vertex))
            degrees[neighbour] += getEdgeMultiplicityIdx(vertex, neighbour);
    return degrees;
}

} // BaseGraph
