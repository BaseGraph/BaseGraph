#ifndef BASE_GRAPH_DIRECTED_MULTIGRAPH_HPP
#define BASE_GRAPH_DIRECTED_MULTIGRAPH_HPP

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/util.hpp"

namespace BaseGraph {

/**
 * Directed graphs with self-loops and multiedges.
 *
 * Behaves nearly identically to @ref BaseGraph::DirectedGraph. The main
 * difference is that @ref addEdge and @ref removeEdge count parallel edges
 * (multiedges). The number of parallel edges is stored in a @ref
 * BaseGraph::EdgeMultiplicity.
 */
class DirectedMultigraph : private LabeledDirectedGraph<EdgeMultiplicity> {
    using BaseClass = LabeledDirectedGraph<EdgeMultiplicity>;
    size_t totalEdgeNumber = 0;

  public:
    /// Returns the edge number excluding parallel edges (any multiplicity
    /// counts as a single edge).
    using BaseClass::begin;
    using BaseClass::edges;
    using BaseClass::end;
    using BaseClass::getEdgeNumber;
    using BaseClass::getOutNeighbours;
    using BaseClass::getSize;
    using BaseClass::resize;

    /// Constructs an empty graph with \p size vertices.
    explicit DirectedMultigraph(size_t size = 0) : BaseClass(size) {}

    /**
     * Constructs graph containing every vertex in \p multiedgeList. Graph size
     * is adjusted to the largest index in \p edgeSequence.
     *
     * @tparam Container Any container of
     * BaseGraph::LabeledEdge<BaseGraph::EdgeMultiplicity> that supports
     * range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * usable.
     *
     * For example:
     * \code{.cpp}
     * std::list<BaseGraph::LabeledEdge<BaseGraph::EdgeMultiplicity>> multiedges
     * =
     *         {{0, 2, 1}, {0, 1, 4}, {0, 0, 1}, {5, 10, 2}};
     * BaseGraph::DirectedMultigraph graph(multiedges);
     * \endcode
     */
    template <template <class...> class Container, class... Args>
    explicit DirectedMultigraph(
        const Container<LabeledEdge<EdgeMultiplicity>, Args...> &multiedgeList
    )
        : BaseClass(0) {

        VertexIndex maxIndex = 0;
        for (const auto &multiedge : multiedgeList) {
            maxIndex = std::max(std::get<0>(multiedge), std::get<1>(multiedge));
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addMultiedge(
                std::get<0>(multiedge), std::get<1>(multiedge),
                std::get<2>(multiedge)
            );
        }
    }

    /// Returns the edge number including parallel edges.
    size_t getTotalEdgeNumber() const { return totalEdgeNumber; }

    /// Returns if graph instance and \p other have the same size, edges and
    /// edge multiplicities.
    bool operator==(const DirectedMultigraph &other) const {
        return BaseClass::operator==(other);
    }
    /// Returns `not` @ref operator==.
    bool operator!=(const DirectedMultigraph &other) const {
        return BaseClass::operator!=(other);
    }

    /// Adds a single edge with @ref addMultiedge.
    void
    addEdge(VertexIndex source, VertexIndex destination, bool force = false) {
        addMultiedge(source, destination, 1, force);
    }
    /// Adds reciprocal edge. Calls @ref addEdge for both edge orientations.
    void addReciprocalEdge(
        VertexIndex source, VertexIndex destination, bool force = false
    ) {
        addEdge(source, destination, force);
        addEdge(destination, source, force);
    }
    /**
     * Adds multiple directed edges from vertex \p source to \p destination.
     * If the edge already exists, the current multiplicity is increased
     * (unless \p force is `true`).
     *
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * @ref removeDuplicateEdges. Note that
     * @ref removeDuplicateEdges does not merge duplicate edges,
     * it only removes them. <b>Duplicate edges are not multiedges</b>.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param multiplicity Edge multiplicity.
     * @param force If \c false and the edge exists, the multiplicity is
     * increased. If \c true, a new edge (potentially duplicate) is added
     * without checking its existence (quicker).
     */
    void addMultiedge(
        VertexIndex source, VertexIndex destination,
        EdgeMultiplicity multiplicity, bool force = false
    ) {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        if (multiplicity == 0)
            return;

        auto edgeExists = hasEdge(source, destination);
        if (force || !edgeExists) {
            BaseClass::addEdge(source, destination, multiplicity, true);
            totalEdgeNumber += multiplicity;
        } else if (edgeExists) {
            totalEdgeNumber += multiplicity;
            BaseClass::edgeLabels[{source, destination}] += multiplicity;
        }
    }
    /// Adds reciprocal edges. Calls @ref addMultiedge for both edge
    /// orientations.
    void addReciprocalMultiedge(
        VertexIndex source, VertexIndex destination,
        EdgeMultiplicity multiplicity, bool force = false
    ) {
        addMultiedge(source, destination, multiplicity, force);
        addMultiedge(destination, source, multiplicity, force);
    }

    /// Removes one directed edge from \p source to \p destination with @ref
    /// removeMultiedge.
    void removeEdge(VertexIndex source, VertexIndex destination) {
        removeMultiedge(source, destination, 1);
    }
    /**
     * Removes multiple directed edges from \p source to \p destination. If \p
     * multiplicity is greater or equal to the current multiplicity, the
     * multiplicity is set to 0.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param multiplicity Number of edges to remove.
     */
    void removeMultiedge(
        VertexIndex source, VertexIndex destination,
        EdgeMultiplicity multiplicity
    ) {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        auto &neighbours = getOutNeighbours(source);
        auto i = findFirst(neighbours, destination);
        if (i < neighbours.size()) {
            auto &currentMultiplicity = edgeLabels[{source, destination}];
            if (currentMultiplicity > multiplicity) {
                currentMultiplicity -= multiplicity;
                totalEdgeNumber -= multiplicity;
            } else {
                edgeNumber--;
                totalEdgeNumber -= currentMultiplicity;

                swapAndPop(adjacencyList[source], i);
                edgeLabels.erase({source, destination});
            }
        }
    }

    /// Returns if there is at least one directed edge that connects \p source
    /// to \p destination.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
        return BaseClass::hasEdge(vertex1, vertex2);
    }

    /// Return the multiplicity of the edge connecting \p source to \p
    /// destination.
    EdgeMultiplicity
    getEdgeMultiplicity(VertexIndex source, VertexIndex destination) const {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        return edgeLabels.count({source, destination}) == 0
                   ? 0
                   : getEdgeLabel(source, destination);
    }
    /**
     * Change the multiplicity of the edge connecting \p source to \p
     * destination. If \p multiplicity is 0, the multiedge is removed. If
     * the edge doesn't exist, it's created.
     *
     * @param source, destination Index of the vertices.
     * @param multiplicity New edge multiplicity.
     */
    void setEdgeMultiplicity(
        VertexIndex source, VertexIndex destination,
        EdgeMultiplicity multiplicity
    ) {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        if (multiplicity == 0) {
            removeAllEdges(source, destination);
        } else if (hasEdge(source, destination)) {
            auto &currentMultiplicity = edgeLabels[{source, destination}];
            totalEdgeNumber +=
                ((long long int)multiplicity -
                 (long long int)currentMultiplicity);
            currentMultiplicity = multiplicity;
        } else {
            addMultiedge(source, destination, multiplicity, true);
        }
    }

    /**
     * Removes duplicate edges that have been created using the flag
     * `force=true` in @ref addMultiedge.
     *
     * \warning
     * The duplicate edges are <b>not</b> merged, meaning that the edge
     * multiplicities are not changed by this method.
     */
    void removeDuplicateEdges() {
        for (VertexIndex source : *this) {
            std::set<VertexIndex> seenVertices;

            for (size_t j=0; j<adjacencyList[source].size();) {
                const auto neighbour = adjacencyList[source][j];
                if (!seenVertices.count(neighbour)) {
                    seenVertices.insert(neighbour);
                    j++;
                } else {
                    totalEdgeNumber -= getEdgeLabel(source, neighbour, false);
                    swapAndPop(adjacencyList[source], j);
                    edgeNumber--;
                    // the value at position j must be checked again
                }
            }
        }
    }

    /// @copydoc LabeledDirectedGraph::removeSelfLoops
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeAllEdges(i, i);
    }

    /// @copydoc LabeledDirectedGraph::removeVertexFromEdgeList
    void removeVertexFromEdgeList(VertexIndex vertex) {
        assertVertexInRange(vertex);

        for (VertexIndex i : *this) {
            if (i == vertex) {
                edgeNumber -= adjacencyList[i].size();
                for (auto neighbour: getOutNeighbours(i))
                    totalEdgeNumber -= getEdgeLabel(i, neighbour, false);
                adjacencyList[i].clear();
                continue;
            }
            for (size_t j=0; j<adjacencyList[i].size(); ) {
                auto neighbour = adjacencyList[i][j];
                if (neighbour == vertex) {
                    totalEdgeNumber -= getEdgeLabel(i, neighbour, false);
                    edgeNumber--;
                    swapAndPop(adjacencyList[i], j);
                    // the value at position j must be checked again
                } else {
                    j++;
                }
            }
        }
    }

    /// @copydoc LabeledDirectedGraph::clearEdges
    void clearEdges() {
        for (VertexIndex i : *this)
            adjacencyList[i].clear();
        edgeNumber = 0;
        totalEdgeNumber = 0;
    }

    /// Casts the multigraph to a labeled graph, thus ignoring edge
    /// multiplicities.
    const BaseClass &asLabeledGraph() const {
        return static_cast<const BaseClass &>(*this);
    }

    /// Constructs the adjacency matrix. The element \f$a_{ij}\f$ of the matrix
    /// is the multiplicity of edge \f$(i,j)\f$.
    AdjacencyMatrix getAdjacencyMatrix() const {
        AdjacencyMatrix adjacencyMatrix;
        adjacencyMatrix.resize(size, std::vector<size_t>(size, 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getOutNeighbours(i))
                adjacencyMatrix[i][j] += getEdgeMultiplicity(i, j);

        return adjacencyMatrix;
    }

    /// Counts the number of out edges of \p vertex, including parallel edges.
    size_t getOutDegree(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        size_t degree = 0;
        for (auto neighbour : adjacencyList[vertex])
            degree += getEdgeMultiplicity(vertex, neighbour);
        return degree;
    }

    /// Counts the number of out edges of each vertex, including parallel edges.
    std::vector<size_t> getOutDegrees() const {
        std::vector<size_t> degrees(getSize(), 0);
        for (auto edge : edges())
            degrees[edge.first] += getEdgeMultiplicity(edge.first, edge.second);
        return degrees;
    }

    /// Counts the number of in edges of \p vertex, including parallel edges.
    /// @ref getInDegrees is more efficient when more than one
    /// in degree is needed.
    size_t getInDegree(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        size_t degree = 0;

        for (auto edge : edges())
            if (edge.second == vertex)
                degree += getEdgeLabel(edge.first, edge.second);
        return degree;
    }

    /// Counts the number of in edges of each vertex, including parallel edges.
    std::vector<size_t> getInDegrees() const {
        std::vector<size_t> inDegrees(getSize(), 0);

        for (auto edge : edges())
            inDegrees[edge.second] += getEdgeLabel(edge.first, edge.second);
        return inDegrees;
    }

    /// @copydoc LabeledDirectedGraph::operator<<
    friend std::ostream &
    operator<<(std::ostream &stream, const DirectedMultigraph &graph) {
        stream << "DirectedMultigraph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getOutNeighbours(i))
                stream << neighbour << "("
                       << graph.getEdgeMultiplicity(i, neighbour) << "), ";
            stream << "\n";
        }
        return stream;
    }

  private:
    void removeAllEdges(VertexIndex source, VertexIndex destination) {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        bool edgeFound = false;
        for (size_t i=0; i<adjacencyList[source].size(); ) {
            if (adjacencyList[source][i] == destination) {
                totalEdgeNumber -= getEdgeLabel(source, destination, false);
                edgeNumber -= 1;
                edgeFound = true;
                swapAndPop(adjacencyList[source], i);
            } else {
                i++;
            }
        }
        if (edgeFound)
            edgeLabels.erase({source, destination});
    }
};
} // namespace BaseGraph

#endif
