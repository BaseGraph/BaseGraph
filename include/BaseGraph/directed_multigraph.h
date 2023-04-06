#ifndef BASE_GRAPH_DIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_DIRECTED_MULTIGRAPH_H

#include "BaseGraph/edgelabeled_directedgraph.hpp"
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
class DirectedMultigraph : public EdgeLabeledDirectedGraph<EdgeMultiplicity> {
    using BaseClass = EdgeLabeledDirectedGraph<EdgeMultiplicity>;

  public:
    using BaseClass::BaseClass;

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
                      EdgeMultiplicity multiplicity, bool force = false);
    /**
     * Add reciprocal edge. Calls DirectedMultigraph::addMultiedge for both
     * edge directions.
     * @param vertex1, vertex2 Vertices of reciprocal edges.
     * @param multiplicity Edge multiplicity.
     * @param force See `force` of addEdge.
     */
    void addReciprocalMultiedge(VertexIndex source, VertexIndex destination,
                                EdgeMultiplicity multiplicity,
                                bool force = false);

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
                         EdgeMultiplicity multiplicity);

    /**
     * Return the multiplicity of the edge connecting \p source to \p
     * destination.
     */
    EdgeMultiplicity getEdgeMultiplicity(VertexIndex source,
                                         VertexIndex destination) const;
    /**
     * Change the multiplicity of the edge connecting \p source to \p
     * destination. If \p multiplicity is 0, the multiedge is removed. If
     * the edge doesn't exist, it's created.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param multiplicity New edge multiplicity.
     */
    void setEdgeMultiplicity(VertexIndex source, VertexIndex destination,
                             EdgeMultiplicity multiplicity);

    AdjacencyMatrix getAdjacencyMatrix() const;
    size_t getOutDegreeOf(VertexIndex vertex) const;
    size_t getInDegreeOf(VertexIndex vertex) const;
    std::vector<size_t> getOutDegrees() const;
    /// Count the number of in edges of \p vertex. Use
    /// DirectedMultigraph::getInDegrees if more than one in degree is needed.
    std::vector<size_t> getInDegrees() const;
};

} // namespace BaseGraph

#endif
