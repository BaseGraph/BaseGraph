#include "BaseGraph/undirected_multigraph.h"


namespace BaseGraph {


void UndirectedMultigraph::addMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity, bool force) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);


    if (force)
        BaseClass::addEdgeIdx(vertex1, vertex2, multiplicity, true);

    else {
        Edge optimalEdge = getSmallestAdjacency(vertex1, vertex2);
        auto neighbour = findNeighbour(optimalEdge);


        if (neighbour == adjacencyList[optimalEdge.first].end())
            BaseClass::addEdgeIdx(vertex1, vertex2, multiplicity, true);

        else {
            neighbour->second += multiplicity;
            totalEdgeNumber   += multiplicity;

            if (vertex1 != vertex2)
                findNeighbour(optimalEdge.second, optimalEdge.first)->second += multiplicity;
        }
    }
}

void UndirectedMultigraph::removeMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);


    Edge optimalEdge = getSmallestAdjacency(vertex1, vertex2);
    auto neighbour = findNeighbour(optimalEdge);

    if (neighbour != adjacencyList[optimalEdge.first].end()) {
        EdgeMultiplicity& currentMultiplicity = neighbour->second;

        if (currentMultiplicity <= multiplicity) {
            totalEdgeNumber -= currentMultiplicity;
            distinctEdgeNumber--;

            adjacencyList[optimalEdge.first].erase(neighbour);
            if (vertex1 != vertex2)
                adjacencyList[optimalEdge.second].erase( findNeighbour(optimalEdge.second, optimalEdge.first) );
        }
        else {
            currentMultiplicity -= multiplicity;
            totalEdgeNumber     -= multiplicity;

            if (vertex1 != vertex2)
                findNeighbour(optimalEdge.second, optimalEdge.first)->second -= multiplicity;
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

        if (neighbour != adjacencyList[optimalEdge.first].end()) {
            EdgeMultiplicity& currentMultiplicity = neighbour->second;

            if (currentMultiplicity != multiplicity) {
                totalEdgeNumber += (long int) multiplicity - (long int) currentMultiplicity;
                currentMultiplicity = multiplicity;

                if (vertex1 != vertex2)
                    findNeighbour(optimalEdge.second, optimalEdge.first)->second = multiplicity;
            }
        }
        else
            BaseClass::addEdgeIdx(vertex1, vertex2, multiplicity, true);
    }
}


} // BaseGraph
