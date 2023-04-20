#ifndef BASE_GRAPH_DIRECTED_WEIGHTED_GRAPH_HPP
#define BASE_GRAPH_DIRECTED_WEIGHTED_GRAPH_HPP

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"

namespace BaseGraph {

class DirectedWeightedGraph : private LabeledDirectedGraph<EdgeWeight> {
    using BaseClass = LabeledDirectedGraph<double>;
    long double totalWeight = 0;

  public:
    using BaseClass::getEdgeNumber;
    using BaseClass::getEdgesFrom;
    using BaseClass::getSize;
    using BaseClass::resize;
    using BaseClass::operator==;
    using BaseClass::operator!=;
    using BaseClass::begin;
    using BaseClass::edges;
    using BaseClass::end;
    using BaseClass::getAdjacencyMatrix;
    using BaseClass::getInDegree;
    using BaseClass::getInDegrees;
    using BaseClass::getOutDegree;
    using BaseClass::getOutDegrees;
    using BaseClass::hasEdge;

    explicit DirectedWeightedGraph(size_t size = 0) : BaseClass(size) {}

    template <template <class...> class Container, class... Args>
    explicit DirectedWeightedGraph(
        const Container<LabeledEdge<EdgeWeight>, Args...> &weightedEdgeList)
        : BaseClass(0) {

        VertexIndex maxIndex = 0;
        for (const auto &multiedge : weightedEdgeList) {
            maxIndex = std::max(std::get<0>(multiedge), std::get<1>(multiedge));
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addMultiedge(std::get<0>(multiedge), std::get<1>(multiedge),
                         std::get<2>(multiedge));
        }
    }

    long double getTotalWeight() const { return totalWeight; }

    bool operator==(const DirectedWeightedGraph &other) const {
        return BaseClass::operator==(other);
    }
    bool operator!=(const DirectedWeightedGraph &other) const {
        return BaseClass::operator!=(other);
    }

    void addEdge(VertexIndex source, VertexIndex destination, EdgeWeight weight,
                 bool force = false) {
        if (force || !hasEdge(source, destination)) {
            adjacencyList[source].push_back(destination);
            ++edgeNumber;
            _setLabel({source, destination}, weight);
            totalWeight += weight;
        }
    }
    void addReciprocalEdge(VertexIndex source, VertexIndex destination,
                           bool force = false) {
        addEdge(source, destination, force);
        addEdge(destination, source, force);
    }

    void removeEdge(VertexIndex source, VertexIndex destination) {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        size_t sizeBefore = adjacencyList[source].size();
        adjacencyList[source].remove(destination);
        size_t sizeAfter = sizeBefore - adjacencyList[source].size();

        edgeNumber -= sizeAfter;
        totalWeight -= getEdgeLabel(source, destination, false) * sizeAfter;
        edgeLabels.erase({source, destination});
    }

    EdgeWeight getEdgeWeight(VertexIndex source,
                             VertexIndex destination, bool throwIfInexistent=true) const {
        return getEdgeLabel(source, destination, throwIfInexistent);
    }
    void setEdgeWeight(VertexIndex source, VertexIndex destination,
                       EdgeWeight newWeight) {
        if (hasEdge(source, destination)) {
            auto &currentWeight = edgeLabels[{source, destination}];
            totalWeight += newWeight - currentWeight;
            currentWeight = newWeight;
        } else {
            addEdge(source, destination, newWeight);
        }
    }

    void removeDuplicateEdges() {
        for (VertexIndex i : *this) {
            std::set<VertexIndex> seenVertices;
            auto j = adjacencyList[i].begin();

            while (j != adjacencyList[i].end()) {
                if (!seenVertices.count(*j)) {
                    seenVertices.insert(*j);
                    ++j;
                } else {
                    totalWeight -= getEdgeLabel(i, *j, false);
                    adjacencyList[i].erase(j++);
                    edgeNumber--;
                }
            }
        }
    }

    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
    }
    void clearEdges() {
        BaseClass::clearEdges();
        totalWeight = 0;
    }

    void removeVertexFromEdgeList(VertexIndex vertex) {
        assertVertexInRange(vertex);

        auto &successors = adjacencyList[vertex];
        auto j = successors.begin();
        while (j != successors.end()) {
            totalWeight -= getEdgeLabel(vertex, *j, false);
            successors.erase(j++);
            edgeNumber--;
        }
        for (VertexIndex i = 0; i < size; ++i)
            removeEdge(i, vertex);
    }

    WeightMatrix getWeightMatrix() const {
        WeightMatrix weightMatrix(getSize(),
                                  std::vector<EdgeWeight>(getSize(), 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getEdgesFrom(i))
                weightMatrix[i][j] = getEdgeWeight(i, j);

        return weightMatrix;
    }

    /// Output graph's size and edges in text to a given `std::stream` object.
    friend std::ostream &operator<<(std::ostream &stream,
                                    const DirectedWeightedGraph &graph) {
        stream << "DirectedWeightedGraph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getEdgesFrom(i))
                stream << neighbour << "(" << graph.getEdgeWeight(i, neighbour)
                       << "), ";
            stream << "\n";
        }
        return stream;
    }
};

} // namespace BaseGraph

#endif
