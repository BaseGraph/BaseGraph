#include "BaseGraph/undirected_multigraph.h"


namespace BaseGraph {


void UndirectedMultigraph::addMultiedge(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity, bool force) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    if (multiplicity == 0)
        return;

    if (force)
        BaseClass::addEdge(vertex1, vertex2, multiplicity, true);

    else if (UndirectedGraph::hasEdge(vertex1, vertex2)) {
        totalEdgeNumber += multiplicity;
        edgeLabels[orderedEdge(vertex1, vertex2)] += multiplicity;
    }
    else {
        BaseClass::addEdge(vertex1, vertex2, multiplicity, true);
    }
}

void UndirectedMultigraph::removeMultiedge(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    const auto& neighbours = getOutEdgesOf(vertex1);
    for (auto j=neighbours.begin(); j!= neighbours.end(); j++) {
        if (*j != vertex2)
            continue;

        auto& currentMultiplicity = edgeLabels[orderedEdge(vertex1, vertex2)];
        if (currentMultiplicity > multiplicity) {
            currentMultiplicity -= multiplicity;
            totalEdgeNumber -= multiplicity;
        }
        else {
            edgeNumber--;
            totalEdgeNumber -= currentMultiplicity;
            adjacencyList[vertex1].erase(j);

            if (vertex1 != vertex2)
                adjacencyList[vertex2].remove(vertex1);
            edgeLabels.erase(orderedEdge(vertex1, vertex2));
        }
        break;
    }
}

void UndirectedMultigraph::setEdgeMultiplicity(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    if (multiplicity == 0) {
        removeEdge(vertex1, vertex2);
    }
    else if (UndirectedGraph::hasEdge(vertex1, vertex2)) {
        auto& currentMultiplicity = edgeLabels[orderedEdge(vertex1, vertex2)];
        totalEdgeNumber += ((long long int) multiplicity - (long long int) currentMultiplicity);
        currentMultiplicity = multiplicity;
    }
    else {
        BaseClass::addEdge(vertex1, vertex2, multiplicity, true);
    }
}

AdjacencyMatrix UndirectedMultigraph::getAdjacencyMatrix() const{
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(size, std::vector<size_t>(size, 0));

    for (VertexIndex i=0; i<size; ++i)
        for (auto& j: getOutEdgesOf(i)) {
            const auto& multiplicity = getEdgeLabelOf(i, j);
            adjacencyMatrix[i][j] += i!=j ? multiplicity : 2*multiplicity;
        }

    return adjacencyMatrix;
}


EdgeMultiplicity UndirectedMultigraph::getEdgeMultiplicity(VertexIndex vertex1, VertexIndex vertex2) const {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    return edgeLabels.count(orderedEdge(vertex1, vertex2))==0 ? 0 : getEdgeLabelOf(vertex1, vertex2);
}


size_t UndirectedMultigraph::getDegreeOf(VertexIndex vertex, bool countSelfLoopsTwice) const {
    assertVertexInRange(vertex);
    size_t degree = 0;
    EdgeMultiplicity multiplicity;

    for (auto& neighbour: getNeighboursOf(vertex)) {
        multiplicity = getEdgeMultiplicity(vertex, neighbour);
        degree += countSelfLoopsTwice && vertex==neighbour ? 2*multiplicity : multiplicity;
    }
    return degree;
}

std::vector<size_t> UndirectedMultigraph::getDegrees(bool countSelfLoopsTwice) const {
    std::vector<size_t> degrees(getSize(), 0);
    for (size_t vertex=0; vertex<getSize(); vertex++)
        degrees[vertex] = getDegreeOf(vertex, countSelfLoopsTwice);
    return degrees;
}


} // BaseGraph
