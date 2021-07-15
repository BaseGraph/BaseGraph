#include "BaseGraph/directed_multigraph.h"


namespace BaseGraph {


void DirectedMultigraph::addMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity multiplicity, bool force) {
    assertVertexInRange(source);
    assertVertexInRange(destination);


    if (force)
        BaseClass::addEdgeIdx(source, destination, multiplicity, true);

    else {
        auto neighbour = findNeighbour(source, destination);

        if (neighbour == adjacencyList[source].end())
            BaseClass::addEdgeIdx(source, destination, multiplicity, true);

        else {
            totalEdgeNumber   += multiplicity;
            neighbour->second += multiplicity;
        }
    }
}

void DirectedMultigraph::removeMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity multiplicity) {
    assertVertexInRange(source);
    assertVertexInRange(destination);


    auto neighbour = findNeighbour(source, destination);

    if (neighbour != adjacencyList[source].end()) {
        EdgeMultiplicity& currentMultiplicity = neighbour->second;

        if (currentMultiplicity <= multiplicity) {
            totalEdgeNumber -= currentMultiplicity;
            distinctEdgeNumber--;

            adjacencyList[source].erase(neighbour);
        }
        else {
            currentMultiplicity -= multiplicity;
            totalEdgeNumber     -= multiplicity;
        }
    }
}

void DirectedMultigraph::setEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity multiplicity) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (multiplicity == 0)
        BaseClass::removeEdgeIdx(source, destination);

    else {
        auto neighbour = findNeighbour(source, destination);

        if (neighbour != adjacencyList[source].end()) {
            EdgeMultiplicity& currentMultiplicity = neighbour->second;

            if (currentMultiplicity != multiplicity) {
                totalEdgeNumber += (long int) multiplicity - (long int) currentMultiplicity;
                currentMultiplicity = multiplicity;
            }
        }
        else
            BaseClass::addEdgeIdx(source, destination, multiplicity, true);
    }
}


} // BaseGraph
