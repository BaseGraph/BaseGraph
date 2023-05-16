#ifndef BASE_GRAPH_DIRECTED_WEIGHTED_GRAPH_HPP
#define BASE_GRAPH_DIRECTED_WEIGHTED_GRAPH_HPP

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"

namespace BaseGraph {

/**
 * Directed graphs with self-loops and weighted edges.
 *
 * Behaves nearly identically to @ref BaseGraph::LabeledDirectedGraph. The
 * difference is that each edge must have a weight stored in a @ref
 * EdgeMultiplicity.
 */
class DirectedWeightedGraph : private LabeledDirectedGraph<EdgeWeight> {
    using BaseClass = LabeledDirectedGraph<double>;
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
    /// @copydoc LabeledDirectedGraph::getInDegree
    /// Doesn't consider the edge weights.
    using BaseClass::getInDegree;
    /// @copydoc LabeledDirectedGraph::getInDegrees
    /// Doesn't consider the edge weights.
    using BaseClass::getInDegrees;
    /// @copydoc LabeledDirectedGraph::getOutDegree
    /// Doesn't consider the edge weights.
    using BaseClass::getOutDegree;
    /// @copydoc LabeledDirectedGraph::getOutDegrees
    /// Doesn't consider the edge weights.
    using BaseClass::getOutDegrees;
    using BaseClass::hasEdge;

    /// Constructs an empty graph with \p size vertices.
    explicit DirectedWeightedGraph(size_t size = 0) : BaseClass(size) {}

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
     * BaseGraph::DirectedWeightedGraph graph(edges);
     * \endcode
     */
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

    /**
     * Returns the sum of the edge weights in the graph.
     * \warning As any floating point operation, the result will seldom be
     * exact. The error may increase when edges are added and/or removed
     * frequently.
     */
    long double getTotalWeight() const { return totalWeight; }

    /// Returns if graph instance and \p other have the same size, edges and
    /// edge weights.
    bool operator==(const DirectedWeightedGraph &other) const {
        return BaseClass::operator==(other);
    }
    /// Returns `not` @ref operator==.
    bool operator!=(const DirectedWeightedGraph &other) const {
        return BaseClass::operator!=(other);
    }

    /**
     * Adds directed edge of weight \p weight from vertex \p source to \p
     * destination.
     *
     * \warning Use <tt>force=true</tt> with caution as it may create
     * duplicate edges. Since this class isn't designed to handle them, it might
     * behave unexpectedly in some algorithms. Remove duplicate edges with @ref
     * removeDuplicateEdges.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param weight Edge weight.
     * @param force If \c false, the edge is not added if it already exists.
     *              If \c true, the edge is added without checking its
     *              existence (quicker).
     */
    void addEdge(VertexIndex source, VertexIndex destination, EdgeWeight weight,
                 bool force = false) {
        if (force || !hasEdge(source, destination)) {
            adjacencyList[source].push_back(destination);
            ++edgeNumber;
            _setLabel({source, destination}, weight);
            totalWeight += weight;
        }
    }
    /// @copydoc LabeledDirectedGraph::addReciprocalEdge
    void addReciprocalEdge(VertexIndex source, VertexIndex destination,
                           bool force = false) {
        addEdge(source, destination, force);
        addEdge(destination, source, force);
    }

    /// @copydoc LabeledDirectedGraph::removeEdge
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

    /// Returns the weight of an edge connnecting \p source to \p destination.
    /// See @ref LabeledDirectedGraph::getEdgeLabel for more details.
    EdgeWeight getEdgeWeight(VertexIndex source, VertexIndex destination,
                             bool throwIfInexistent = true) const {
        return getEdgeLabel(source, destination, throwIfInexistent);
    }
    /// Changes the weight of the edge connecting \p source to \p destination to
    /// \p newWeight. If the edge doesn't exist, it is created.
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

    /// @copydoc LabeledDirectedGraph::removeDuplicateEdges
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

    /// @copydoc LabeledDirectedGraph::removeSelfLoops
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
    }
    /// @copydoc LabeledDirectedGraph::clearEdges
    void clearEdges() {
        BaseClass::clearEdges();
        totalWeight = 0;
    }

    /// Casts the weighted graph to a labeled graph, thus ignoring edge weights.
    const BaseClass &asLabeledGraph() const {
        return static_cast<const BaseClass &>(*this);
    }

    /// @copydoc LabeledDirectedGraph::removeVertexFromEdgeList
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

    /// Constructs a matrix in which the element \f$w_{ij}\f$ is the weight of
    /// the edge \f$(i,j)\f$.
    WeightMatrix getWeightMatrix() const {
        WeightMatrix weightMatrix(getSize(),
                                  std::vector<EdgeWeight>(getSize(), 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getOutNeighbours(i))
                weightMatrix[i][j] = getEdgeWeight(i, j);

        return weightMatrix;
    }

    /// @copydoc LabeledDirectedGraph::operator<<
    friend std::ostream &operator<<(std::ostream &stream,
                                    const DirectedWeightedGraph &graph) {
        stream << "DirectedWeightedGraph of size: " << graph.getSize() << "\n"
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
