#include "BaseGraph/directed_multigraph.h"

namespace BaseGraph {

void DirectedMultigraph::addMultiedge(VertexIndex source,
                                      VertexIndex destination,
                                      EdgeMultiplicity multiplicity,
                                      bool force) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (multiplicity == 0)
        return;

    if (force)
        BaseClass::addEdge(source, destination, multiplicity, true);

    else if (hasEdge(source, destination)) {
        totalEdgeNumber += multiplicity;
        edgeLabels[{source, destination}] += multiplicity;
    } else {
        BaseClass::addEdge(source, destination, multiplicity, true);
    }
}

void DirectedMultigraph::removeMultiedge(VertexIndex source,
                                         VertexIndex destination,
                                         EdgeMultiplicity multiplicity) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    const auto &neighbours = getOutEdgesOf(source);
    for (auto j = neighbours.begin(); j != neighbours.end(); j++) {
        if (*j != destination)
            continue;

        auto &currentMultiplicity = edgeLabels[{source, destination}];
        if (currentMultiplicity > multiplicity) {
            currentMultiplicity -= multiplicity;
            totalEdgeNumber -= multiplicity;
        } else {
            edgeNumber--;
            totalEdgeNumber -= currentMultiplicity;
            adjacencyList[source].erase(j);
            edgeLabels.erase({source, destination});
        }
        break;
    }
}

EdgeMultiplicity
DirectedMultigraph::getEdgeMultiplicity(VertexIndex source,
                                        VertexIndex destination) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    return edgeLabels.count({source, destination}) == 0
               ? 0
               : getEdgeLabelOf(source, destination);
}

void DirectedMultigraph::setEdgeMultiplicity(VertexIndex source,
                                             VertexIndex destination,
                                             EdgeMultiplicity multiplicity) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (multiplicity == 0) {
        BaseClass::removeEdge(source, destination);
    } else if (hasEdge(source, destination)) {
        auto &currentMultiplicity = edgeLabels[{source, destination}];
        totalEdgeNumber +=
            ((long long int)multiplicity - (long long int)currentMultiplicity);
        currentMultiplicity = multiplicity;
    } else {
        BaseClass::addEdge(source, destination, multiplicity, true);
    }
}

AdjacencyMatrix DirectedMultigraph::getAdjacencyMatrix() const {
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(size, std::vector<size_t>(size, 0));

    for (VertexIndex i = 0; i < size; ++i)
        for (auto &j : getOutEdgesOf(i))
            adjacencyMatrix[i][j] += getEdgeMultiplicity(i, j);

    return adjacencyMatrix;
}

size_t DirectedMultigraph::getOutDegreeOf(VertexIndex vertex) const {
    assertVertexInRange(vertex);
    size_t degree = 0;
    for (auto neighbour : adjacencyList[vertex])
        degree += getEdgeMultiplicity(vertex, neighbour);
    return degree;
}

std::vector<size_t> DirectedMultigraph::getOutDegrees() const {
    std::vector<size_t> degrees(getSize(), 0);
    for (size_t vertex = 0; vertex < getSize(); vertex++)
        degrees[vertex] = getOutDegreeOf(vertex);
    return degrees;
}

size_t DirectedMultigraph::getInDegreeOf(VertexIndex vertex) const {
    assertVertexInRange(vertex);
    size_t degree = 0;

    for (VertexIndex i : *this)
        for (auto j : adjacencyList[i])
            if (j == vertex)
                degree += getEdgeLabelOf(i, j);
    return degree;
}

std::vector<size_t> DirectedMultigraph::getInDegrees() const {
    std::vector<size_t> degrees(getSize(), 0);
    for (size_t vertex = 0; vertex < getSize(); vertex++)
        for (auto neighbour : BaseClass::getOutEdgesOf(vertex))
            degrees[neighbour] += getEdgeMultiplicity(vertex, neighbour);
    return degrees;
}

} // namespace BaseGraph
