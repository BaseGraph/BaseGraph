#ifndef BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H


#include "BaseGraph/edgelabeled_undirectedgraph.hpp"
#include "BaseGraph/types.h"


namespace BaseGraph {

/**
 * Expresses undirected graphs with self-loops and multiedges.
 *
 * Behaves identically to BaseGraph::UndirectedGraph except that parallel edges
 * are accounted for when adding and removing edges. When the multiplicity of
 * an edge :math:`(i,j)` is 0, :math:`j` is no longer considered a neighbour of
 * :math:`i`.
 */
class UndirectedMultigraph: public EdgeLabeledUndirectedGraph<EdgeMultiplicity> {

    public:
        using BaseClass = EdgeLabeledUndirectedGraph<EdgeMultiplicity>;

        using BaseClass::BaseClass;

        /**
         * Add directed edge connecting \p vertex1 to \p vertex2 if the edge
         * doesn't exist. Increases multiplicity by 1 otherwise (unless \p
         * force is `true`).
         *
         * \warning
         * Use <tt>force=true</tt> with caution as it may create duplicate edges.
         * Since this class isn't designed to handle them, it might behave
         * unexpectedly in some algorithms. Remove duplicate edges with
         * UndirectedMultigraph::removeDuplicateEdges. <b>Duplicate edges are not multiedges</b>.
         *
         * @param vertex1, vertex2 Index of the vertices to connect.
         * @param force If \c false and the edge exists, the multiplicity is increased by 1.
         *              If \c true, a new edge (potentially duplicate) is
         *              added without checking its existence (quicker).
         */
        void addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false) override {
            addMultiedgeIdx(vertex1, vertex2, 1, force);
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
         * UndirectedMultigraph::removeDuplicateEdges. <b>Duplicate edges are not multiedges</b>.
         *
         * @param vertex1, vertex2 Index of the vertices to connect.
         * @param multiplicity Edge multiplicity.
         * @param force If \c false and the edge exists, the multiplicity is increased.
         *              If \c true, a new edge (potentially duplicate) is added
         *              without checking its existence (quicker).
         */
        void addMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity, bool force=false);

        /**
         * Remove one edge connecting \p vertex1 and \p vertex2. Effectively
         * decrement the multiplicity.
         *
         * @param vertex1, vertex2 Index of the vertices to connect.
         */
        void removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) override {
            removeMultiedgeIdx(vertex1, vertex2, 1);
        }
        /**
         * Remove multiple edges connecting \p vertex1 and \p vertex2. If \p multiplicity
         * is greater or equal to the current multiplicity, the multiplicity is set to 0.
         *
         * @param vertex1, vertex2 Index of the vertices to remove.
         * @param multiplicity Number of edges to remove.
         */
        void removeMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity);

        /**
         * Return the multiplicity of the edge connecting \p source to \p destination.
         */
        EdgeMultiplicity getEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2) const;
        /**
         * Change the multiplicity of the edge connecting \p vertex1 and \p
         * vertex2. If \p multiplicity is 0, the multiedge is removed. If
         * the edge doesn't exist, it's created.
         *
         * @param vertex1, vertex2 Index of the vertices.
         * @param multiplicity New edge multiplicity.
         */
        void setEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity multiplicity);

        AdjacencyMatrix getAdjacencyMatrix() const override;
        size_t getDegreeOfIdx(VertexIndex vertex, bool countSelfLoopsTwice=true) const override;
        std::vector<size_t> getDegrees(bool countSelfLoopsTwice=true) const override;
};



} // namespace BaseGraph

#endif
