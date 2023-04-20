#ifndef BASE_GRAPH_DIRECTED_MULTIGRAPH_HPP
#define BASE_GRAPH_DIRECTED_MULTIGRAPH_HPP

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"

namespace BaseGraph {

/**
 * Expresses directed graphs with self-loops and multiedges.
 *
 * Behaves identically to BaseGraph::DirectedGraph except that parallel edges
 * are accounted for when adding and removing edges. When the multiplicity of
 * an edge :math:`(i,j)` is 0, :math:`j` is no longer considered a neighbour of
 * :math:`i`.
 */
class DirectedMultigraph : private LabeledDirectedGraph<EdgeMultiplicity> {
    using BaseClass = LabeledDirectedGraph<EdgeMultiplicity>;
    size_t totalEdgeNumber = 0;

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

    explicit DirectedMultigraph(size_t size = 0) : BaseClass(size) {}

    /**
     * Construct _DirectedGraph containing every vertex in \p edges. Graph
     * size is adjusted to the largest index in \p edges.
     *
     * @tparam Container Any template container that accepts type
     * BaseGraph::Edge and that supports range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * accepted.
     *
     * @param edges Edges to add into the graph.
     */
    template <template <class...> class Container, class... Args>
    explicit DirectedMultigraph(
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

    size_t getTotalEdgeNumber() const { return totalEdgeNumber; }

    bool operator==(const DirectedMultigraph &other) const {
        return BaseClass::operator==(other);
    }
    bool operator!=(const DirectedMultigraph &other) const {
        return BaseClass::operator!=(other);
    }

    /**
     * Add directed edge from vertex \p source to \p destination if edge
     * doesn't exist. Increases multiplicity by 1 otherwise (unless \p
     * force is `true`).
     *
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * DirectedMultigraph::removeDuplicateEdges. <b>Duplicate edges are not
     * multiedges</b>.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param force If \c false and the edge exists, the multiplicity is
     * increased by 1. If \c true, a new edge (potentially duplicate) is added
     * without checking its existence (quicker).
     */
    void addEdge(VertexIndex source, VertexIndex destination,
                 bool force = false) {
        addMultiedge(source, destination, 1, force);
    }
    /**
     * Add reciprocal edge. Calls DirectedMultigraph::addEdge for both
     * edge directions.
     * @param vertex1, vertex2 Vertices of reciprocal edges.
     * @param force See `force` of addEdge.
     */
    void addReciprocalEdge(VertexIndex source, VertexIndex destination,
                           bool force = false) {
        addEdge(source, destination, force);
        addEdge(destination, source, force);
    }
    /**
     * Add multiple directed edges from vertex \p source to \p destination.
     * If the edge already exists, the current multiplicity is increased
     * (unless \p force is `true`).
     *
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * DirectedMultigraph::removeDuplicateEdges. <b>Duplicate edges are not
     * multiedges</b>.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param multiplicity Edge multiplicity.
     * @param force If \c false and the edge exists, the multiplicity is
     * increased. If \c true, a new edge (potentially duplicate) is added
     *              without checking its existence (quicker).
     */
    void addMultiedge(VertexIndex source, VertexIndex destination,
                      EdgeMultiplicity multiplicity, bool force = false) {
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
    /**
     * Add reciprocal edge. Calls DirectedMultigraph::addMultiedge for both
     * edge directions.
     * @param vertex1, vertex2 Vertices of reciprocal edges.
     * @param multiplicity Edge multiplicity.
     * @param force See `force` of addEdge.
     */
    void addReciprocalMultiedge(VertexIndex source, VertexIndex destination,
                                EdgeMultiplicity multiplicity,
                                bool force = false) {
        addMultiedge(source, destination, multiplicity, force);
        addMultiedge(destination, source, multiplicity, force);
    }

    /**
     * Remove one directed edge from \p source to \p destination.
     * Effectively decrement the multiplicity.
     *
     * @param source, destination Index of the source and destination vertices.
     */
    void removeEdge(VertexIndex source, VertexIndex destination) {
        removeMultiedge(source, destination, 1);
    }
    /**
     * Remove multiple directed edges from \p source to \p destination. If \p
     * multiplicity is greater or equal to the current multiplicity, the
     * multiplicity is set to 0.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param multiplicity Number of edges to remove.
     */
    void removeMultiedge(VertexIndex source, VertexIndex destination,
                         EdgeMultiplicity multiplicity) {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        const auto &neighbours = getEdgesFrom(source);
        for (auto j = neighbours.begin(); j != neighbours.end(); ++j) {
            if (*j != destination)
                continue;

            auto &currentMultiplicity = edgeLabels[{source, destination}];
            if (currentMultiplicity > multiplicity) {
                currentMultiplicity -= multiplicity;
                totalEdgeNumber -= multiplicity;
            } else {
                edgeNumber--;
                totalEdgeNumber -= currentMultiplicity;
                adjacencyList[source].erase(j);
                edgeLabels.erase({source, destination});
            }
            break;
        }
    }

    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
        return BaseClass::hasEdge(vertex1, vertex2);
    }

    /**
     * Return the multiplicity of the edge connecting \p source to \p
     * destination.
     */
    EdgeMultiplicity getEdgeMultiplicity(VertexIndex source,
                                         VertexIndex destination) const {
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
     * @param source, destination Index of the source and destination vertices.
     * @param multiplicity New edge multiplicity.
     */
    void setEdgeMultiplicity(VertexIndex source, VertexIndex destination,
                             EdgeMultiplicity multiplicity) {
        assertVertexInRange(source);
        assertVertexInRange(destination);

        if (multiplicity == 0) {
            removeAllEdges(source, destination);
        } else if (hasEdge(source, destination)) {
            auto &currentMultiplicity = edgeLabels[{source, destination}];
            totalEdgeNumber += ((long long int)multiplicity -
                                (long long int)currentMultiplicity);
            currentMultiplicity = multiplicity;
        } else {
            addMultiedge(source, destination, multiplicity, true);
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
                    totalEdgeNumber -= getEdgeLabel(i, *j, false);
                    adjacencyList[i].erase(j++);
                    edgeNumber--;
                }
            }
        }
    }

    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeAllEdges(i, i);
    }
    void clearEdges() {
        for (VertexIndex i : *this)
            adjacencyList[i].clear();
        edgeNumber = 0;
        totalEdgeNumber = 0;
    }

    void removeVertexFromEdgeList(VertexIndex vertex) {
        assertVertexInRange(vertex);

        auto &successors = adjacencyList[vertex];
        auto j = successors.begin();
        while (j != successors.end()) {
            totalEdgeNumber -= getEdgeLabel(vertex, *j, false);
            successors.erase(j++);
            edgeNumber--;
        }
        for (VertexIndex i = 0; i < size; ++i)
            removeAllEdges(i, vertex);
    }

    AdjacencyMatrix getAdjacencyMatrix() const {
        AdjacencyMatrix adjacencyMatrix;
        adjacencyMatrix.resize(size, std::vector<size_t>(size, 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getEdgesFrom(i))
                adjacencyMatrix[i][j] += getEdgeMultiplicity(i, j);

        return adjacencyMatrix;
    }

    size_t getOutDegree(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        size_t degree = 0;
        for (auto neighbour : adjacencyList[vertex])
            degree += getEdgeMultiplicity(vertex, neighbour);
        return degree;
    }

    size_t getInDegree(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        size_t degree = 0;

        for (auto edge : edges())
            if (edge.second == vertex)
                degree += getEdgeLabel(edge.first, edge.second);
        return degree;
    }
    std::vector<size_t> getOutDegrees() const {
        std::vector<size_t> degrees(getSize(), 0);
        for (auto edge : edges())
            degrees[edge.first] += getEdgeMultiplicity(edge.first, edge.second);
        return degrees;
    }
    /// Count the number of in edges of \p vertex. Use
    /// DirectedMultigraph::getInDegrees if more than one in degree is needed.
    std::vector<size_t> getInDegrees() const {
        std::vector<size_t> inDegrees(getSize(), 0);

        for (auto edge : edges())
            inDegrees[edge.second] += getEdgeLabel(edge.first, edge.second);
        return inDegrees;
    }

    /// Output graph's size and edges in text to a given `std::stream` object.
    friend std::ostream &operator<<(std::ostream &stream,
                                    const DirectedMultigraph &graph) {
        stream << "DirectedMultigraph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getEdgesFrom(i))
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

        size_t sizeBefore = adjacencyList[source].size();
        adjacencyList[source].remove(destination);
        size_t sizeAfter = sizeBefore - adjacencyList[source].size();

        edgeNumber -= sizeAfter;
        totalEdgeNumber -= getEdgeLabel(source, destination, false) * sizeAfter;
        edgeLabels.erase({source, destination});
    }
};
} // namespace BaseGraph

#endif
