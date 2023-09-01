#ifndef BASE_GRAPH_UNDIRECTED_MULTIGRAPH_HPP
#define BASE_GRAPH_UNDIRECTED_MULTIGRAPH_HPP

#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"
#include "BaseGraph/util.hpp"

#include <type_traits>

namespace BaseGraph {

/**
 * Undirected graphs with self-loops and multiedges.
 *
 * Behaves nearly identically to @ref BaseGraph::UndirectedGraph. The main
 * difference is that @ref addEdge and @ref removeEdge count parallel edges
 * (multiedges). The number of parallel edges is stored in a @ref
 * BaseGraph::EdgeMultiplicity.
 */
class UndirectedMultigraph : private LabeledUndirectedGraph<EdgeMultiplicity> {
    size_t totalEdgeNumber = 0;

  public:
    using BaseClass = LabeledUndirectedGraph<EdgeMultiplicity>;

    /// @copydoc DirectedMultigraph::getEdgeNumber
    using BaseClass::begin;
    using BaseClass::edges;
    using BaseClass::end;
    using BaseClass::getEdgeNumber;
    using BaseClass::getOutNeighbours;
    using BaseClass::getSize;
    using BaseClass::resize;

    /// Constructs an empty graph with \p size vertices.
    explicit UndirectedMultigraph(size_t size = 0) : BaseClass(size) {}

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
     * BaseGraph::UndirectedMultigraph graph(multiedges);
     * \endcode
     */
    template <template <class...> class Container, class... Args>
    explicit UndirectedMultigraph(
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

    /// @copydoc DirectedMultigraph::getTotalEdgeNumber
    size_t getTotalEdgeNumber() const { return totalEdgeNumber; }

    /// @copydoc DirectedMultigraph::operator==
    bool operator==(const UndirectedMultigraph &other) const {
        return BaseClass::operator==(other);
    }
    /// @copydoc DirectedMultigraph::operator!=
    bool operator!=(const UndirectedMultigraph &other) const {
        return BaseClass::operator!=(other);
    }

    /// @copydoc DirectedMultigraph::addEdge
    void addEdge(VertexIndex vertex1, VertexIndex vertex2, bool force = false) {
        addMultiedge(vertex1, vertex2, 1, force);
    }
    /**
     * Add multiple directed edges connecting \p vertex1 to \p vertex2.
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
     * @param vertex1, vertex2 Index of the vertices to connect.
     * @param multiplicity Edge multiplicity.
     * @param force If \c false and the edge exists, the multiplicity is
     * increased. If \c true, a new edge (potentially duplicate) is added
     * without checking its existence (quicker).
     */
    void addMultiedge(
        VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity,
        bool force = false
    ) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        if (multiplicity == 0)
            return;

        auto edgeExists = hasEdge(vertex1, vertex2);
        if (force || !edgeExists) {
            BaseClass::addEdge(vertex1, vertex2, multiplicity, true);
            totalEdgeNumber += multiplicity;
        } else if (edgeExists) {
            totalEdgeNumber += multiplicity;
            BaseClass::edgeLabels[orderedEdge(vertex1, vertex2)] +=
                multiplicity;
        }
    }

    /// @copydoc DirectedMultigraph::removeEdge
    void removeEdge(VertexIndex vertex1, VertexIndex vertex2) {
        removeMultiedge(vertex1, vertex2, 1);
    }
    /**
     * Remove multiple edges connecting \p vertex1 and \p vertex2. If \p
     * multiplicity is greater or equal to the current multiplicity, the
     * multiplicity is set to 0.
     *
     * @param vertex1, vertex2 Index of the vertices to remove.
     * @param multiplicity Number of edges to remove.
     */
    void removeMultiedge(
        VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity
    ) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        size_t i = findFirst(adjacencyList[vertex1], vertex2);
        if (i < adjacencyList[vertex1].size()) {
            auto &currentMultiplicity =
                edgeLabels[orderedEdge(vertex1, vertex2)];
            if (currentMultiplicity > multiplicity) {
                currentMultiplicity -= multiplicity;
                totalEdgeNumber -= multiplicity;
            } else {
                edgeNumber--;
                totalEdgeNumber -= currentMultiplicity;
                swapAndPop(adjacencyList[vertex1], i);

                if (vertex1 != vertex2) {
                    i = findFirst(adjacencyList[vertex2], vertex1);
                    swapAndPop(adjacencyList[vertex2], i);
                }
                edgeLabels.erase(orderedEdge(vertex1, vertex2));
            }
        }
    }

    /// Returns if there is at least one directed edge that connects \p vertex1
    /// to \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
        return BaseClass::hasEdge(vertex1, vertex2);
    }

    /// Returns the multiplicity of the edge connecting \p vertex1 to \p
    /// vertex2.
    EdgeMultiplicity
    getEdgeMultiplicity(VertexIndex vertex1, VertexIndex vertex2) const {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        return edgeLabels.count(orderedEdge(vertex1, vertex2)) == 0
                   ? 0
                   : getEdgeLabel(vertex1, vertex2);
    }
    /**
     * Change the multiplicity of the edge connecting \p vertex1 and \p
     * vertex2. If \p multiplicity is 0, the multiedge is removed. If
     * the edge doesn't exist, it's created.
     *
     * @param vertex1, vertex2 Index of the vertices.
     * @param multiplicity New edge multiplicity.
     */
    void setEdgeMultiplicity(
        VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity
    ) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        if (multiplicity == 0) {
            removeEdge(vertex1, vertex2);
        } else if (hasEdge(vertex1, vertex2)) {
            auto &currentMultiplicity =
                edgeLabels[orderedEdge(vertex1, vertex2)];
            totalEdgeNumber +=
                ((long long int)multiplicity -
                 (long long int)currentMultiplicity);
            currentMultiplicity = multiplicity;
        } else {
            addMultiedge(vertex1, vertex2, multiplicity, true);
        }
    }

    /// @copydoc DirectedMultigraph::removeDuplicateEdges
    void removeDuplicateEdges() {
        for (VertexIndex i : *this) {
            std::set<VertexIndex> seenVertices;

            for (size_t j=0; j<adjacencyList[i].size();) {
                const auto neighbour = adjacencyList[i][j];
                if (!seenVertices.count(neighbour)) {
                    seenVertices.insert(neighbour);
                    j++;
                } else {
                    if (i <= neighbour) {
                        totalEdgeNumber -= getEdgeLabel(i, neighbour, false);
                        edgeNumber--;
                    }
                    swapAndPop(adjacencyList[i], j);
                    // the value at position j must be checked again
                }
            }
            seenVertices.clear();
        }
    }

    /// @copydoc DirectedMultigraph::removeSelfLoops
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeAllEdges(i, i);
    }

    /// @copydoc DirectedMultigraph::removeVertexFromEdgeList
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
                    if (i <= neighbour) {
                        totalEdgeNumber -= getEdgeLabel(i, neighbour, false);
                        edgeNumber--;
                    }
                    swapAndPop(adjacencyList[i], j);
                    // the value at position j must be checked again
                } else {
                    j++;
                }
            }
        }
    }

    /// @copydoc DirectedMultigraph::clearEdges
    void clearEdges() {
        for (VertexIndex i : *this)
            adjacencyList[i].clear();
        edgeNumber = 0;
        totalEdgeNumber = 0;
    }

    /// @copydoc DirectedMultigraph::asLabeledGraph
    const BaseClass &asLabeledGraph() const {
        return static_cast<const BaseClass &>(*this);
    }

    /// @copydoc DirectedMultigraph::getAdjacencyMatrix
    AdjacencyMatrix getAdjacencyMatrix(bool countSelfLoopsTwice = true) const {
        AdjacencyMatrix adjacencyMatrix;
        adjacencyMatrix.resize(size, std::vector<size_t>(size, 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getOutNeighbours(i)) {
                const auto &multiplicity = getEdgeLabel(i, j);
                adjacencyMatrix[i][j] += i == j && countSelfLoopsTwice
                                             ? 2 * multiplicity
                                             : multiplicity;
            }
        return adjacencyMatrix;
    }

    /// Counts the number of edges connected to @p vertex, including parallel
    /// edges.
    size_t
    getDegree(VertexIndex vertex, bool countSelfLoopsTwice = true) const {
        assertVertexInRange(vertex);
        size_t degree = 0;
        EdgeMultiplicity multiplicity;

        for (auto &neighbour : getNeighbours(vertex)) {
            multiplicity = getEdgeMultiplicity(vertex, neighbour);
            degree += countSelfLoopsTwice && vertex == neighbour
                          ? 2 * multiplicity
                          : multiplicity;
        }
        return degree;
    }

    /// Counts the number of edges connected to each vertex, including parallel
    /// edges.
    std::vector<size_t> getDegrees(bool countSelfLoopsTwice = true) const {
        std::vector<size_t> degrees(getSize(), 0);
        for (size_t vertex : *this)
            degrees[vertex] = getDegree(vertex, countSelfLoopsTwice);
        return degrees;
    }

    /// @copydoc DirectedMultigraph::operator<<
    friend std::ostream &
    operator<<(std::ostream &stream, const UndirectedMultigraph &graph) {
        stream << "UndirectedMultigraph of size: " << graph.getSize() << "\n"
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
    void removeAllEdges(VertexIndex vertex1, VertexIndex vertex2) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        bool edgeFound = false;
        for (size_t i=0; i<adjacencyList[vertex1].size(); ) {
            if (adjacencyList[vertex1][i] == vertex2) {
                totalEdgeNumber -= getEdgeLabel(vertex1, vertex2, false);
                edgeNumber -= 1;
                edgeFound = true;
                swapAndPop(adjacencyList[vertex1], i);
            } else {
                i++;
            }
        }
        if (edgeFound) {
            edgeLabels.erase(orderedEdge(vertex1, vertex2));

            for (size_t i=0; i<adjacencyList[vertex2].size(); ) {
                if (adjacencyList[vertex2][i] == vertex1)
                    swapAndPop(adjacencyList[vertex2], i);
                else
                    i++;
            }
        }

    }
};

} // namespace BaseGraph

#endif
