#ifndef BASE_GRAPH_UNDIRECTED_MULTIGRAPH_HPP
#define BASE_GRAPH_UNDIRECTED_MULTIGRAPH_HPP

#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"
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
    void addMultiedge(VertexIndex vertex1, VertexIndex vertex2,
                      EdgeMultiplicity multiplicity, bool force = false) {
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
    void removeMultiedge(VertexIndex vertex1, VertexIndex vertex2,
                         EdgeMultiplicity multiplicity) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        const auto &neighbours = getOutNeighbours(vertex1);
        for (auto j = neighbours.begin(); j != neighbours.end(); ++j) {
            if (*j != vertex2)
                continue;

            auto &currentMultiplicity =
                edgeLabels[orderedEdge(vertex1, vertex2)];
            if (currentMultiplicity > multiplicity) {
                currentMultiplicity -= multiplicity;
                totalEdgeNumber -= multiplicity;
            } else {
                edgeNumber--;
                totalEdgeNumber -= currentMultiplicity;
                adjacencyList[vertex1].erase(j);

                if (vertex1 != vertex2)
                    adjacencyList[vertex2].remove(vertex1);
                edgeLabels.erase(orderedEdge(vertex1, vertex2));
            }
            break;
        }
    }

    /// Returns if there is at least one directed edge that connects \p vertex1
    /// to \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
        return BaseClass::hasEdge(vertex1, vertex2);
    }

    /// Returns the multiplicity of the edge connecting \p vertex1 to \p
    /// vertex2.
    EdgeMultiplicity getEdgeMultiplicity(VertexIndex vertex1,
                                         VertexIndex vertex2) const {
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
    void setEdgeMultiplicity(VertexIndex vertex1, VertexIndex vertex2,
                             EdgeMultiplicity multiplicity) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        if (multiplicity == 0) {
            removeEdge(vertex1, vertex2);
        } else if (hasEdge(vertex1, vertex2)) {
            auto &currentMultiplicity =
                edgeLabels[orderedEdge(vertex1, vertex2)];
            totalEdgeNumber += ((long long int)multiplicity -
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
            auto j = BaseClass::adjacencyList[i].begin();

            while (j != BaseClass::adjacencyList[i].end()) {
                if (!seenVertices.count(*j)) {
                    seenVertices.insert(*j);
                    ++j;
                } else {
                    if (i <= *j) {
                        totalEdgeNumber -= getEdgeLabel(i, *j, false);
                        --BaseClass::edgeNumber;
                    }
                    BaseClass::adjacencyList[i].erase(j++);
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

        Successors::iterator j;
        for (VertexIndex i : *this) {
            j = BaseClass::adjacencyList[i].begin();
            while (j != BaseClass::adjacencyList[i].end())
                if (i == vertex || *j == vertex) {
                    if (i <= *j) {
                        totalEdgeNumber -= getEdgeLabel(i, *j, false);
                        --BaseClass::edgeNumber;
                    }
                    BaseClass::adjacencyList[i].erase(j++);
                } else {
                    ++j;
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
    size_t getDegree(VertexIndex vertex,
                     bool countSelfLoopsTwice = true) const {
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
    friend std::ostream &operator<<(std::ostream &stream,
                                    const UndirectedMultigraph &graph) {
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

        size_t sizeBefore = BaseClass::adjacencyList[vertex1].size();
        BaseClass::adjacencyList[vertex1].remove(vertex2);
        size_t sizeDifference =
            sizeBefore - BaseClass::adjacencyList[vertex1].size();

        if (sizeDifference > 0) {
            BaseClass::adjacencyList[vertex2].remove(vertex1);
            BaseClass::edgeNumber -= sizeDifference;
            totalEdgeNumber -=
                getEdgeLabel(vertex1, vertex2, false) * sizeDifference;
            BaseClass::edgeLabels.erase(orderedEdge(vertex1, vertex2));
        }
    }
};

} // namespace BaseGraph

#endif
