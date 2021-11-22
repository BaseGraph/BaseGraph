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
            neighbour->label += multiplicity;
        }
    }
}

void DirectedMultigraph::removeMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity multiplicity) {
    assertVertexInRange(source);
    assertVertexInRange(destination);


    auto neighbour = findNeighbour(source, destination);

    if (neighbour != adjacencyList[source].end()) {
        EdgeMultiplicity& currentMultiplicity = neighbour->label;

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
            EdgeMultiplicity& currentMultiplicity = neighbour->label;

            if (currentMultiplicity != multiplicity) {
                totalEdgeNumber += (long int) multiplicity - (long int) currentMultiplicity;
                currentMultiplicity = multiplicity;
            }
        }
        else
            BaseClass::addEdgeIdx(source, destination, multiplicity, true);
    }
}

EdgeMultiplicity DirectedMultigraph::getEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    auto neighbour = const_findNeighbour(source, destination);
    if (neighbour == adjacencyList[source].end())
        return 0;
    return neighbour->label;
}

size_t DirectedMultigraph::getOutDegreeOfIdx(VertexIndex vertex) const {
    assertVertexInRange(vertex);
    size_t degree = 0;
    for (auto neighbour: adjacencyList[vertex])
        degree += neighbour.label;
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

    for (VertexIndex v: *this)
        for (auto neighbour: adjacencyList[v])
            if (neighbour.vertexIndex == vertex)
                degree += neighbour.label;
    return degree;
}

std::vector<size_t> DirectedMultigraph::getInDegrees() const {
    std::vector<size_t> degrees(getSize(), 0);
    for (size_t vertex=0; vertex<getSize(); vertex++)
        for (auto neighbour: BaseClass::getOutEdgesOfIdx(vertex))
            degrees[neighbour.vertexIndex] += neighbour.label;
    return degrees;
}

} // BaseGraph
