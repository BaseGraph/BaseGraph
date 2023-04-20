#ifndef BASE_GRAPH_UNDIRECTED_WEIGHTED_GRAPH_HPP
#define BASE_GRAPH_UNDIRECTED_WEIGHTED_GRAPH_HPP

#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

namespace BaseGraph {

class UndirectedWeightedGraph : private LabeledUndirectedGraph<EdgeWeight> {
    using BaseClass = LabeledUndirectedGraph<double>;
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
    using BaseClass::getDegree;
    using BaseClass::getDegrees;
    using BaseClass::hasEdge;

    explicit UndirectedWeightedGraph(size_t size = 0) : BaseClass(size) {}

    template <template <class...> class Container, class... Args>
    explicit UndirectedWeightedGraph(
        const Container<LabeledEdge<EdgeMultiplicity>, Args...> &multiedgeList)
        : BaseClass(0) {

        VertexIndex maxIndex = 0;
        for (const auto &multiedge : multiedgeList) {
            maxIndex = std::max(std::get<0>(multiedge), std::get<1>(multiedge));
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addMultiedge(std::get<0>(multiedge), std::get<1>(multiedge),
                         std::get<2>(multiedge));
        }
    }

    EdgeWeight getTotalWeight() const { return totalWeight; }

    bool operator==(const UndirectedWeightedGraph &other) const {
        return BaseClass::operator==(other);
    }
    bool operator!=(const UndirectedWeightedGraph &other) const {
        return BaseClass::operator!=(other);
    }

    void addEdge(VertexIndex vertex1, VertexIndex vertex2, EdgeWeight weight,
                 bool force = false) {
        if (force || !hasEdge(vertex1, vertex2)) {
            if (vertex1 != vertex2)
                adjacencyList[vertex1].push_back(vertex2);
            adjacencyList[vertex2].push_back(vertex1);
            setLabel(vertex1, vertex2, weight);
            ++edgeNumber;
            totalWeight += weight;
        }
    }
    void removeEdge(VertexIndex vertex1, VertexIndex vertex2) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        size_t sizeBefore = adjacencyList[vertex1].size();
        adjacencyList[vertex1].remove(vertex2);
        size_t sizeDifference = sizeBefore - adjacencyList[vertex1].size();

        if (sizeDifference > 0) {
            adjacencyList[vertex2].remove(vertex1);
            edgeNumber -= sizeDifference;
            totalWeight -=
                getEdgeLabel(vertex1, vertex2, false) * sizeDifference;
            edgeLabels.erase(orderedEdge(vertex1, vertex2));
        }
    }

    EdgeWeight getEdgeWeight(VertexIndex vertex1, VertexIndex vertex2, bool throwIfInexistent=true) const {
        return getEdgeLabel(vertex1, vertex2, throwIfInexistent);
    }

    void setEdgeWeight(VertexIndex vertex1, VertexIndex vertex2,
                       EdgeWeight newWeight) {
        if (hasEdge(vertex1, vertex2)) {
            auto &currentWeight = edgeLabels[{vertex1, vertex2}];
            totalWeight += newWeight - currentWeight;
            currentWeight = newWeight;
        } else {
            addEdge(vertex1, vertex2, newWeight);
        }
    }

    WeightMatrix getWeightMatrix() const {
        WeightMatrix weightMatrix(getSize(),
                                  std::vector<EdgeWeight>(getSize(), 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getEdgesFrom(i))
                weightMatrix[i][j] = getEdgeWeight(i, j);
        return weightMatrix;
    }

    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
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
                    if (i <= *j) {
                        totalWeight -= getEdgeLabel(i, *j, false);
                        --edgeNumber;
                    }
                    adjacencyList[i].erase(j++);
                }
            }
            seenVertices.clear();
        }
    }

    void removeVertexFromEdgeList(VertexIndex vertex) {
        assertVertexInRange(vertex);

        Successors::iterator j;
        for (VertexIndex i : *this) {
            j = adjacencyList[i].begin();
            while (j != adjacencyList[i].end())
                if (i == vertex || *j == vertex) {
                    if (i <= *j) {
                        totalWeight -= getEdgeLabel(i, *j, false);
                        --edgeNumber;
                    }
                    adjacencyList[i].erase(j++);
                } else {
                    ++j;
                }
        }
    }

    void clearEdges() {
        BaseClass::clearEdges();
        totalWeight = 0;
    }

    /// Output graph's size and edges in text to a given `std::stream` object.
    friend std::ostream &operator<<(std::ostream &stream,
                                    const UndirectedWeightedGraph &graph) {
        stream << "UndirectedWeightedGraph of size: " << graph.getSize() << "\n"
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
