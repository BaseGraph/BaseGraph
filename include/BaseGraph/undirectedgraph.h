#ifndef BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H
#define BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H

#include "BaseGraph/directedgraph.h"


namespace BaseGraph {

/**
 * Base class for undirected graphs with self-loops and without multiedges.
 *
 * Vertices are identified by their integer index between 0 and \c size -1.
 * Vertices can be added using UndirectedGraph::resize. Removing vertices is
 * not supported as it would require reindexing.  However, a vertex can be
 * effectively removed by erasing all of its edges with
 * UndirectedGraph::removeVertexFromEdgeListIdx.
 */
class UndirectedGraph: protected DirectedGraph {
    public:
        /// Construct UndirectedGraph with \p size vertices.
        /// @param size Number of vertices.
        explicit UndirectedGraph(size_t size=0) : DirectedGraph(size) {}
        explicit UndirectedGraph(const DirectedGraph&);

        /**
         * Construct UndirectedGraph containing every vertex in \p edges.
         * Graph size is adjusted to the largest index in \p edges.
         *
         * @tparam Container Any template container that accepts type BaseGraph::Edge
         *         and that supports range-based loops. Most
         *         <a href="https://en.cppreference.com/w/cpp/container">STL
         *         containers</a> are accepted.
         *
         * @param edges Edges to add into the graph.
         */
        template<template<class ...> class Container, class ...Args>
        explicit UndirectedGraph(const Container<Edge>& edgeList): UndirectedGraph(0) {
            VertexIndex maxIndex=0;
            for (const Edge& edge: edgeList) {
                maxIndex = std::max(edge.first, edge.second);
                if (maxIndex >= getSize())
                    resize(maxIndex+1);
                addEdgeIdx(edge.first, edge.second);
            }
        }

        /// Construct DirectedGraph containing each reciprocal edge of
        /// UndirectedGraph instance.
        DirectedGraph getDirectedGraph() const;

        using DirectedGraph::resize;
        using DirectedGraph::getSize;
        using DirectedGraph::getEdgeNumber;

        /**
         * Return if graph instance and \p other have the same size and have the
         * same edges.
         * @param other Graph to compare to.
         * @return If graph instance is equal to \p other.
         */
        bool operator==(const UndirectedGraph& other) const {
            return DirectedGraph::operator==(other);
        }
        /**
         * Return if graph instance and \p other have different sizes and/or have
         * different edges.
         * @param other Graph to compare to.
         * @return If graph instance is different from \p other.
         */
        bool operator!=(const UndirectedGraph& other) const {
            return DirectedGraph::operator!=(other);
        }

        /**
         * Add edge between \p vertex1 and \p vertex2.
         * \warning
         * Use <tt>force=true</tt> with caution as it may create duplicate edges.
         * Since this class isn't designed to handle them, it might behave unexpectedly
         * in some algorithms. Remove duplicate edges with
         * UndirectedGraph::removeDuplicateEdges.
         * Use UndirectedMultigraph for multigraph support.
         *
         * @param vertex1, vertex2 Index of the vertices to be connected.
         * @param force If \c false, the edge is not added if it already exists.
         *              If \c true, the edge is added without checking its existence (quicker).
         */
        virtual void addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false);
        virtual void addEdgeIdx(const Edge& edge, bool force=false) {
            addEdgeIdx(edge.first, edge.second, force);
        }
        /// Return if \p vertex1 is connected to \p vertex2.
        bool hasEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) const;
        /// Remove edge (including duplicates) between \p vertex1 and \p vertex2.
        virtual void removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);
        virtual void removeVertexFromEdgeListIdx(VertexIndex vertex);
        virtual void removeDuplicateEdges();
        using DirectedGraph::removeSelfLoops;
        using DirectedGraph::clearEdges;

        /**
         * Construct a UndirectedGraph that only contains the edges with vertices enumerated
         * by the iterator.
         *
         * @tparam Iterator Any iterator type that is valid in the construction of
         * <a href="https://en.cppreference.com/w/cpp/container/unordered_set">std::unordered_set</a>.
         * @param begin, end Iterators that enumerate vertices to preserve in subgraph.
         * @return Undirected subgraph without vertex remapping. The subgraph has the
         *         same number of vertices than the original graph.
         */
        template <typename Iterator>
        UndirectedGraph getSubgraphOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end));
        };
        /**
         * Construct a UndirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Undirected subgraph without vertex remapping. The subgraph has
         *         the same number of vertices than the original graph.
         */
        UndirectedGraph getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        /**
         * Construct a UndirectedGraph that only contains the edges with vertices
         * enumerated by the iterator.
         *
         * @tparam Iterator Any iterator type that is valid in the construction of
         * <a href="https://en.cppreference.com/w/cpp/container/unordered_set">std::unordered_set</a>.
         * @param begin, end Iterators that enumerate vertices to preserve in subgraph.
         * @return Undirected subgraph and mapping of the original vertex indices to
         * the subgraph vertex indices.
         */
        template <typename Iterator>
        std::pair<UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>>
        getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end));
        };
        /**
         * Construct a UndirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Undirected subgraph and mapping of the original vertex indices
         *         to the subgraph vertex indices.
         */
        std::pair<UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>>
        getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;

        using DirectedGraph::getOutEdgesOfIdx;
        const Successors& getNeighboursOfIdx(VertexIndex vertex) const {
            return getOutEdgesOfIdx(vertex);
        }

        virtual AdjacencyMatrix getAdjacencyMatrix() const;
        /**
         * Return the number of vertices connected to \p vertex.
         * @param vertices Index of a vertex.
         * @param withSelfLoops If `true`, self-loops are counted twice. If `false`,
         *            self-loops are counted once. If there are no self-loops, set
         *            to `false` for reduced complexity.
         *
         * @return degree of vertex \p vertex
         */
        virtual size_t getDegreeOfIdx(VertexIndex vertex, bool withSelfLoops=true) const;
        /// Return the degree of every vertex. See UndirectedGraph::getDegreeOfIdx for argument
        /// \p withSelfLoops.
        virtual std::vector<size_t> getDegrees(bool withSelfLoops=true) const;

        friend std::ostream& operator <<(std::ostream &stream, const UndirectedGraph& graph) {
            stream << "Undirected graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << neighbour << ", ";
                stream << "\n";
            }
            return stream;
        }
        Edge getSmallestAdjacency(VertexIndex vertex1, VertexIndex vertex2) const {
            return getDegreeOfIdx(vertex1, false) < getDegreeOfIdx(vertex2, false)
                ? Edge{vertex1, vertex2} : Edge{vertex2, vertex1};
        }

        using DirectedGraph::begin;
        using DirectedGraph::end;
};

} // namespace BaseGraph

#endif
