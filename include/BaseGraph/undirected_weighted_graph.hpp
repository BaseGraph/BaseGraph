#ifndef BASE_GRAPH_UNDIRECTED_WEIGHTED_GRAPH_HPP
#define BASE_GRAPH_UNDIRECTED_WEIGHTED_GRAPH_HPP

#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

namespace BaseGraph {

/**
 * Undirected graphs with self-loops and weighted edges.
 *
 * Behaves nearly identically to @ref BaseGraph::LabeledUndirectedGraph. The
 * difference is that each edge must have a weight stored in a @ref
 * EdgeMultiplicity.
 */
class UndirectedWeightedGraph : private LabeledUndirectedGraph<EdgeWeight> {
    using BaseClass = LabeledUndirectedGraph<double>;
    long double totalWeight = 0;

  public:
    using BaseClass::begin;
    using BaseClass::edges;
    using BaseClass::end;
    using BaseClass::getAdjacencyMatrix;
    using BaseClass::getEdgeNumber;
    using BaseClass::getOutNeighbours;
    using BaseClass::getSize;
    using BaseClass::resize;
    /// @copydoc LabeledUndirectedGraph::getDegree
    /// Doesn't consider the edge weights.
    using BaseClass::getDegree;
    /// @copydoc LabeledUndirectedGraph::getDegrees
    /// Doesn't consider the edge weights.
    using BaseClass::getDegrees;
    using BaseClass::hasEdge;

    explicit UndirectedWeightedGraph(size_t size = 0) : BaseClass(size) {}

    /**
     * Constructs a graph containing each edge in \p edgeSequence. The graph
     * size is adjusted to the largest index in \p edgeSequence.
     *
     * @tparam Container Any container of @ref LabeledEdge<EdgeWeight>> that
     * supports range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * usable.
     *
     * For example:
     * \code{.cpp}
     * std::list<BaseGraph::LabeledEdge<BaseGraph::EdgeWeight>> edges =
     * {{0, 2, 0.5}, {0, 1, -2}, {0, 10, 10.1}, {5, 0, 0}};
     * BaseGraph::UndirectedWeightedGraph graph(edges);
     * \endcode
     */
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

    /// @copydoc DirectedWeightedGraph::getTotalWeight
    EdgeWeight getTotalWeight() const { return totalWeight; }

    /// @copydoc DirectedWeightedGraph::operator==
    bool operator==(const UndirectedWeightedGraph &other) const {
        return BaseClass::operator==(other);
    }
    /// @copydoc DirectedWeightedGraph::operator!=
    bool operator!=(const UndirectedWeightedGraph &other) const {
        return BaseClass::operator!=(other);
    }

    /**
     * Adds edge of weight \p weight connecting vertex \p vertex1 and \p
     * vertex2.
     *
     * \warning Use <tt>force=true</tt> with caution as it may create
     * duplicate edges. Since this class isn't designed to handle them, it might
     * behave unexpectedly in some algorithms. Remove duplicate edges with @ref
     * removeDuplicateEdges.
     *
     * @param vertex1, vertex2 Index of the vertices to connect.
     * @param weight Edge weight.
     * @param force If \c false, the edge is not added if it already exists.
     *              If \c true, the edge is added without checking its
     *              existence (quicker).
     */
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

    /// @copydoc LabeledDirectedGraph::removeEdge
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

    /// Returns the weight of an edge connnecting \p vertex1 to \p vertex2.
    /// See @ref LabeledDirectedGraph::getEdgeLabel for more details.
    EdgeWeight getEdgeWeight(VertexIndex vertex1, VertexIndex vertex2,
                             bool throwIfInexistent = true) const {
        return getEdgeLabel(vertex1, vertex2, throwIfInexistent);
    }

    /// Changes the weight of the edge connecting \p vertex1 and \p vertex2 to
    /// \p newWeight. If the edge doesn't exist, it is created.
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

    /// @copydoc DirectedWeightedGraph::removeSelfLoops
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
    }

    /// @copydoc DirectedWeightedGraph::removeDuplicateEdges
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

    /// @copydoc DirectedWeightedGraph::removeVertexFromEdgeList
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

    /// @copydoc DirectedWeightedGraph::clearEdges
    void clearEdges() {
        BaseClass::clearEdges();
        totalWeight = 0;
    }

    /// @copydoc DirectedWeightedGraph::asLabeledGraph
    const BaseClass &asLabeledGraph() const {
        return static_cast<const BaseClass &>(*this);
    }

    /// @copydoc DirectedWeightedGraph::getWeightMatrix
    WeightMatrix getWeightMatrix() const {
        WeightMatrix weightMatrix(getSize(),
                                  std::vector<EdgeWeight>(getSize(), 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getOutNeighbours(i))
                weightMatrix[i][j] = getEdgeWeight(i, j);
        return weightMatrix;
    }

    /// @copydoc DirectedWeightedGraph::operator<<
    friend std::ostream &operator<<(std::ostream &stream,
                                    const UndirectedWeightedGraph &graph) {
        stream << "UndirectedWeightedGraph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getOutNeighbours(i))
                stream << neighbour << "(" << graph.getEdgeWeight(i, neighbour)
                       << "), ";
            stream << "\n";
        }
        return stream;
    }
};

} // namespace BaseGraph

#endif
