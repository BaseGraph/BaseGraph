#ifndef BASE_GRAPH_UNDIRECTED_MULTIGRAPH_HPP
#define BASE_GRAPH_UNDIRECTED_MULTIGRAPH_HPP

#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

namespace BaseGraph {

/**
 * Expresses undirected graphs with self-loops and multiedges.
 *
 * Behaves identically to BaseGraph::UndirectedGraph except that parallel edges
 * are accounted for when adding and removing edges. When the multiplicity of
 * an edge :math:`(i,j)` is 0, :math:`j` is no longer considered a neighbour of
 * :math:`i`.
 */
class UndirectedMultigraph : public LabeledUndirectedGraph<EdgeMultiplicity> {

  public:
    using BaseClass = LabeledUndirectedGraph<EdgeMultiplicity>;

    using BaseClass::BaseClass;
    using BaseClass::resize;
    using BaseClass::getSize;
    using BaseClass::getEdgeNumber;
    using BaseClass::getTotalEdgeNumber;
    using BaseClass::getOutEdgesOf;
    using BaseClass::removeVertexFromEdgeList;
    using BaseClass::removeDuplicateEdges;
    using BaseClass::removeSelfLoops;
    using BaseClass::clearEdges;
    using BaseClass::edges;
    using BaseClass::begin;
    using BaseClass::end;

    bool operator==(const UndirectedMultigraph& other) const {
        return BaseClass::operator==(other);
    }
    bool operator!=(const UndirectedMultigraph& other) const {
        return BaseClass::operator!=(other);
    }

    /**
     * Add directed edge connecting \p vertex1 to \p vertex2 if the edge
     * doesn't exist. Increases multiplicity by 1 otherwise (unless \p
     * force is `true`).
     *
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * UndirectedMultigraph::removeDuplicateEdges. <b>Duplicate edges are not
     * multiedges</b>.
     *
     * @param vertex1, vertex2 Index of the vertices to connect.
     * @param force If \c false and the edge exists, the multiplicity is
     * increased by 1. If \c true, a new edge (potentially duplicate) is added
     * without checking its existence (quicker).
     */
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
     * UndirectedMultigraph::removeDuplicateEdges. <b>Duplicate edges are not
     * multiedges</b>.
     *
     * @param vertex1, vertex2 Index of the vertices to connect.
     * @param multiplicity Edge multiplicity.
     * @param force If \c false and the edge exists, the multiplicity is
     * increased. If \c true, a new edge (potentially duplicate) is added
     *              without checking its existence (quicker).
     */
    void addMultiedge(VertexIndex vertex1, VertexIndex vertex2,
                      EdgeMultiplicity multiplicity, bool force = false) {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        if (multiplicity == 0)
            return;

        if (force)
            BaseClass::addEdge(vertex1, vertex2, multiplicity, true);

        else if (hasEdge(vertex1, vertex2)) {
            totalEdgeNumber += multiplicity;
            edgeLabels[orderedEdge(vertex1, vertex2)] += multiplicity;
        } else {
            BaseClass::addEdge(vertex1, vertex2, multiplicity, true);
        }
    }

    /**
     * Remove one edge connecting \p vertex1 and \p vertex2. Effectively
     * decrement the multiplicity.
     *
     * @param vertex1, vertex2 Index of the vertices to connect.
     */
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

        const auto &neighbours = getOutEdgesOf(vertex1);
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

    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
        return BaseClass::hasEdge(vertex1 ,vertex2);
    }

    /**
     * Return the multiplicity of the edge connecting \p source to \p
     * destination.
     */
    EdgeMultiplicity getEdgeMultiplicity(VertexIndex vertex1,
                                         VertexIndex vertex2) const {
        assertVertexInRange(vertex1);
        assertVertexInRange(vertex2);

        return edgeLabels.count(orderedEdge(vertex1, vertex2)) == 0
                   ? 0
                   : getEdgeLabelOf(vertex1, vertex2);
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
            auto &currentMultiplicity = edgeLabels[orderedEdge(vertex1, vertex2)];
            totalEdgeNumber +=
                ((long long int)multiplicity - (long long int)currentMultiplicity);
            currentMultiplicity = multiplicity;
        } else {
            BaseClass::addEdge(vertex1, vertex2, multiplicity, true);
        }
    }

    AdjacencyMatrix getAdjacencyMatrix() const {
        AdjacencyMatrix adjacencyMatrix;
        adjacencyMatrix.resize(size, std::vector<size_t>(size, 0));

        for (VertexIndex i = 0; i < size; ++i)
            for (auto &j : getOutEdgesOf(i)) {
                const auto &multiplicity = getEdgeLabelOf(i, j);
                adjacencyMatrix[i][j] += i != j ? multiplicity : 2 * multiplicity;
            }

        return adjacencyMatrix;
    }
    size_t getDegreeOf(VertexIndex vertex,
                       bool countSelfLoopsTwice = true) const {
        assertVertexInRange(vertex);
        size_t degree = 0;
        EdgeMultiplicity multiplicity;

        for (auto &neighbour : getNeighboursOf(vertex)) {
            multiplicity = getEdgeMultiplicity(vertex, neighbour);
            degree += countSelfLoopsTwice && vertex == neighbour ? 2 * multiplicity
                                                                 : multiplicity;
        }
        return degree;
    }
    std::vector<size_t> getDegrees(bool countSelfLoopsTwice = true) const {
        std::vector<size_t> degrees(getSize(), 0);
        for (size_t vertex: *this)
            degrees[vertex] = getDegreeOf(vertex, countSelfLoopsTwice);
        return degrees;
    }

    /// Output graph's size and edges in text to a given `std::stream` object.
    friend std::ostream &operator<<(std::ostream &stream,
                                    const UndirectedMultigraph &graph) {
        stream << "UndirectedMultigraph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getOutEdgesOf(i))
                stream << neighbour << "(" << graph.getEdgeMultiplicity(i, neighbour) << "), ";
            stream << "\n";
        }
        return stream;
    }
};

} // namespace BaseGraph

#endif
