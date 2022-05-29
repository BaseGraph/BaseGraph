#ifndef BASE_GRAPH_DIRECTED_GRAPH_H
#define BASE_GRAPH_DIRECTED_GRAPH_H

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include "BaseGraph/types.h"


namespace BaseGraph {

/**
 * Base class for directed graphs with self-loops and without multiedges.
 *
 * Vertices are identified by their integer index between 0 and \c size -1.
 * Vertices can be added using DirectedGraph::resize. Removing vertices is not
 * supported as it would require reindexing. However, a vertex can be
 * effectively removed by erasing all of its edges with
 * DirectedGraph::removeVertexFromEdgeListIdx.
 */
class DirectedGraph {

    public:
        /// Construct DirectedGraph with \p size vertices.
        /// @param size Number of vertices.
        explicit DirectedGraph(size_t size=0): size(0), edgeNumber(0) {resize(size);}

        /**
         * Construct DirectedGraph containing every vertex in \p edges. Graph
         * size is adjusted to the largest index in \p edges.
         *
         * @tparam Container Any template container that accepts type BaseGraph::Edge
         *         and that supports range-based loops. Most
         *         <a href="https://en.cppreference.com/w/cpp/container" STL
         *         containers</a> are accepted.
         *
         * @param edges Edges to add into the graph.
         */
        template<template<class ...> class Container, class ...Args>
        explicit DirectedGraph(const Container<Edge>& edges): DirectedGraph(0) {
            VertexIndex maxIndex=0;
            for (const Edge& edge: edges) {
                maxIndex = std::max(edge.first, edge.second);
                if (maxIndex >= getSize())
                    resize(maxIndex+1);
                addEdgeIdx(edge.first, edge.second);
            }
        }

        /// Change number of vertices.
        /// @param size New number of vertices. \b Must be greater or equal to
        ///             current size.
        void resize(size_t size);
        /// Get number of vertices.
        size_t getSize() const { return size; }
        /// Get number of edges.
        size_t getEdgeNumber() const { return edgeNumber; }

        /**
         * Check if graph instance and \p other have the same size and have the
         * same edges.
         * @param other Graph to compare to.
         * @return If graph instance is equal to \p other.
         */
        bool operator==(const DirectedGraph& other) const;
        /**
         * Check if graph instance and \p other have different sizes and/or have
         * different edges.
         * @param other Graph to compare to.
         * @return If graph instance is different from \p other.
         */
        bool operator!=(const DirectedGraph& other) const {
            return !(this->operator==(other));
        }

        /**
         * Add directed edge the from vertex \p source to \p destination.
         * \warning
         * Use <tt>force=true</tt> with caution as it may create duplicate edges.
         * Since this class isn't designed to handle them, it might behave
         * unexpectedly in some algorithms. Use DirectedGraph::removeDuplicateEdges
         * to remove duplicate in some algorithms. Remove duplicate edges with
         * DirectedGraph::removeDuplicateEdges. Use DirectedMultigraph for
         * multigraph support.
         *
         * @param source, destination Index of the source and destination vertices.
         * @param force If \c false, the edge is not added if it already exists.
         *              If \c true, the edge is added without checking its
         *              existence (quicker).
         */
        virtual void addEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false);
        void addEdgeIdx(const Edge& edge, bool force=false) {
            addEdgeIdx(edge.first, edge.second, force);
        }
        /**
         * Add reciprocal edges. Calls DirectedGraph::addEdgeIdx for both
         * edge directions.
         * @param vertex1, vertex2 Vertices of reciprocal edges.
         * @param force See `force` of addEdgeIdx.
         */
        virtual void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false) {
            addEdgeIdx(vertex1, vertex2, force); addEdgeIdx(vertex2, vertex1, force);
        }
        /// Return if a directed edge connects \p source to \p destination.
        bool hasEdgeIdx(VertexIndex source, VertexIndex destination) const;
        /// Remove directed edge (including duplicates) from \p source to
        /// \p destination.
        /// @param source, destination Index of the source and destination vertices.
        virtual void removeEdgeIdx(VertexIndex source, VertexIndex destination);
        /// Remove all duplicate edges. Duplicate edges might exist when adding edges
        /// using `force=true`.
        virtual void removeDuplicateEdges();
        /// Remove all self-loops.
        virtual void removeSelfLoops();
        /// Remove all edges that contain \p vertex. It disconnects the vertex
        /// from the graph.
        /// @param vertex Vertex to isolate in the graph.
        virtual void removeVertexFromEdgeListIdx(VertexIndex vertex);
        /// Remove all edges.
        virtual void clearEdges();

        /**
         * Construct a DirectedGraph that only contains the edges with vertices
         * enumerated by the iterator.
         *
         * @tparam Iterator Any iterator type that is valid in the construction of
         * <a href="https://en.cppreference.com/w/cpp/container/unordered_set">std::unordered_set</a>.
         * @param begin, end Iterators that enumerate vertices to preserve in subgraph.
         * @return Directed subgraph without vertex remapping. The subgraph has
         *         the same number of vertices than the original graph.
         */
        template <typename Iterator>
        DirectedGraph getSubgraphOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end));
        }
        /**
         * Construct a DirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Directed subgraph without vertex remapping. The subgraph has
         *         the same number of vertices than the original graph.
         */
        DirectedGraph getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        /**
         * Construct a DirectedGraph that only contains the edges with vertices
         * enumerated by the iterator.
         *
         * @tparam Iterator Any iterator type that is valid in the construction of
         * <a href="https://en.cppreference.com/w/cpp/container/unordered_set">std::unordered_set</a>.
         * @param begin, end Iterators that enumerate vertices to preserve in subgraph.
         * @return Directed subgraph and mapping of the original vertex indices
         *         to the subgraph vertex indices.
         */
        template <typename Iterator>
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>>
        getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end));
        }
        /**
         * Construct a DirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Directed subgraph and mapping of the original vertex indices
         *         to the subgraph vertex indices.
         */
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>>
        getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;

        /// Get vertices to which \p vertex is connected.
        const Successors& getOutEdgesOfIdx(VertexIndex vertex) const {
            assertVertexInRange(vertex); return adjacencyList[vertex];
        }
        /// Get in edges of each vertex.
        AdjacencyLists getInEdges() const;
        /// Construct the adjacency matrix.
        virtual AdjacencyMatrix getAdjacencyMatrix() const;
        /// Count the number of in edges of \p vertex. Use DirectedGraph::getInDegrees
        /// if more than one in degree is needed.
        virtual size_t getInDegreeOfIdx(VertexIndex vertex) const;
        /// Count the number of in edges of each vertex.
        virtual std::vector<size_t> getInDegrees() const;
        /// Count the number of out edges starting from \p vertex.
        virtual size_t getOutDegreeOfIdx(VertexIndex vertex) const;
        /// Count the number of out edges of each vertex.
        virtual std::vector<size_t> getOutDegrees() const;

        /// Construct a DirectedGraph where each directed edge is reversed.
        DirectedGraph getReversedGraph() const;

        /// Output graph's size and edges in text to a given `std::stream` object.
        friend std::ostream& operator <<(std::ostream &stream, const DirectedGraph& graph) {
            stream << "Directed graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << neighbour << ", ";
                stream << "\n";
            }
            return stream;
        }

        /// Return VertexIterator of first vertex.
        VertexIterator begin() const { return VertexIterator(0); }
        /// Return VertexIterator of last vertex.
        VertexIterator end() const { return VertexIterator(size); }

    protected:
        AdjacencyLists adjacencyList;
        size_t size;
        size_t edgeNumber;

        void assertVertexInRange(VertexIndex vertex) const{
            if (vertex >= size)
                throw std::out_of_range("Vertex index (" + std::to_string(vertex) +
                        ") greater than the graph's size("+ std::to_string(size) +").");
        }
};

} // namespace BaseGraph

#endif
