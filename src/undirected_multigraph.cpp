#include "BaseGraph/undirected_multigraph.h"


namespace BaseGraph {

#define TOTAL_EDGE_NUMBER BaseClass::EdgeLabeledDirectedGraph<EdgeMultiplicity>::totalEdgeNumber
#define DISTINCT_EDGE_NUMBER BaseClass::EdgeLabeledDirectedGraph<EdgeMultiplicity>::distinctEdgeNumber
#define ADJACENCY_LISTS BaseClass::EdgeLabeledDirectedGraph<EdgeMultiplicity>::adjacencyList


void UndirectedMultigraph::addMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity, bool force) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);


    if (force)
        BaseClass::addEdgeIdx(vertex1, vertex2, multiplicity, true);

    else {
        Edge optimalEdge = getSmallestAdjacency(vertex1, vertex2);
        auto neighbour = findNeighbour(optimalEdge);


        if (neighbour == ADJACENCY_LISTS[optimalEdge.first].end())
            BaseClass::addEdgeIdx(vertex1, vertex2, multiplicity, true);

        else {
            neighbour->label += multiplicity;
            TOTAL_EDGE_NUMBER += multiplicity;

            if (vertex1 != vertex2)
                findNeighbour(optimalEdge.second, optimalEdge.first)->label += multiplicity;
        }
    }
}

void UndirectedMultigraph::removeMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);


    Edge optimalEdge = getSmallestAdjacency(vertex1, vertex2);
    auto neighbour = findNeighbour(optimalEdge);

    if (neighbour != ADJACENCY_LISTS[optimalEdge.first].end()) {
        EdgeMultiplicity& currentMultiplicity = neighbour->label;

        if (currentMultiplicity <= multiplicity) {
            TOTAL_EDGE_NUMBER -= currentMultiplicity;
            DISTINCT_EDGE_NUMBER--;

            ADJACENCY_LISTS[optimalEdge.first].erase(neighbour);
            if (vertex1 != vertex2)
                ADJACENCY_LISTS[optimalEdge.second].erase( findNeighbour(optimalEdge.second, optimalEdge.first) );
        }
        else {
            currentMultiplicity -= multiplicity;
            TOTAL_EDGE_NUMBER   -= multiplicity;

            if (vertex1 != vertex2)
                findNeighbour(optimalEdge.second, optimalEdge.first)->label -= multiplicity;
        }
    }
}

void UndirectedMultigraph::setEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);


    if (multiplicity == 0)
        BaseClass::removeEdgeIdx(vertex1, vertex2);

    else {
        Edge optimalEdge = getSmallestAdjacency(vertex1, vertex2);
        auto neighbour = findNeighbour(optimalEdge);

        if (neighbour != ADJACENCY_LISTS[optimalEdge.first].end()) {
            EdgeMultiplicity& currentMultiplicity = neighbour->label;

            if (currentMultiplicity != multiplicity) {
                TOTAL_EDGE_NUMBER += (long int) multiplicity - (long int) currentMultiplicity;
                currentMultiplicity = multiplicity;

                if (vertex1 != vertex2)
                    findNeighbour(optimalEdge.second, optimalEdge.first)->label = multiplicity;
            }
        }
        else
            BaseClass::addEdgeIdx(vertex1, vertex2, multiplicity, true);
    }
}

EdgeMultiplicity UndirectedMultigraph::getEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2) const {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    Edge optimalEdge = getSmallestAdjacency(vertex1, vertex2);
    auto neighbour = const_findNeighbour(optimalEdge);
    if (neighbour == adjacencyList[optimalEdge.first].end())
        return 0;
    return neighbour->label;
}


size_t UndirectedMultigraph::getDegreeOfIdx(VertexIndex vertex, bool) const {
    assertVertexInRange(vertex);
    size_t degree = 0;
    for (auto neighbour: ADJACENCY_LISTS[vertex])
        degree += vertex==neighbour.vertexIndex? 2*neighbour.label : neighbour.label;
    return degree;
}

std::vector<size_t> UndirectedMultigraph::getDegrees(bool) const {
    std::vector<size_t> degrees(getSize(), 0);
    for (size_t vertex=0; vertex<getSize(); vertex++)
        degrees[vertex] = getDegreeOfIdx(vertex);
    return degrees;
}


} // BaseGraph
