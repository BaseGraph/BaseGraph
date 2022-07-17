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
 * DirectedGraph::removeVertexFromEdgeList.
 */
class DirectedGraph {
    public:
        struct Edges {
            struct constEdgeIterator {
                VertexIndex vertex;
                const VertexIndex lastVertex;
                Successors::const_iterator neighbour;
                const DirectedGraph& graph;

                constEdgeIterator(const DirectedGraph& graph, VertexIndex vertex, Successors::const_iterator neighbour) :
                    vertex(vertex), neighbour(neighbour), graph(graph),
                    lastVertex(graph.getSize()==0 ? 0: graph.getSize()-1) {}

                bool operator ==(constEdgeIterator rhs) const { return vertex==rhs.vertex && neighbour==rhs.neighbour; }
                bool operator!=(constEdgeIterator rhs) const { return !(*this==rhs); }
                Edge operator*() { return {vertex, *neighbour}; }
                constEdgeIterator operator++() {
                    neighbour++;
                    while (neighbour == graph.getOutEdgesOf(vertex).end() && vertex != lastVertex)
                        neighbour = graph.getOutEdgesOf(++vertex).begin();

                    return *this;
                }
                constEdgeIterator operator++(int) {constEdgeIterator tmp=constEdgeIterator(graph, vertex, neighbour); operator++(); return tmp;}
            };

            const DirectedGraph& graph;
            Edges(const DirectedGraph& graph): graph(graph) {}

            constEdgeIterator begin() const {
                VertexIndex lastVertex = graph.getSize()==0 ? 0: graph.getSize()-1;

                VertexIndex vertexWithFirstEdge=0;
                Successors::const_iterator neighbour = graph.getOutEdgesOf(0).begin();

                while (neighbour == graph.getOutEdgesOf(vertexWithFirstEdge).end() && vertexWithFirstEdge != lastVertex)
                    neighbour = graph.getOutEdgesOf(++vertexWithFirstEdge).begin();

                return constEdgeIterator(graph, vertexWithFirstEdge, neighbour);
            }
            constEdgeIterator end() const {
                VertexIndex lastVertex = graph.getSize()==0 ? 0: graph.getSize()-1;
                return constEdgeIterator(graph, lastVertex, graph.getOutEdgesOf(lastVertex).end());
            }
        };


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
         *         <a href="https://en.cppreference.com/w/cpp/container">STL
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
                addEdge(edge.first, edge.second);
            }
        }

        /// Change number of vertices.
        /// @param size New number of vertices. \b Must be greater or equal to
        ///             current size.
        void resize(size_t size);
        /// Return number of vertices.
        size_t getSize() const { return size; }
        /// Return number of edges.
        size_t getEdgeNumber() const { return edgeNumber; }

        /**
         * Return if graph instance and \p other have the same size and have the
         * same edges.
         * @param other Graph to compare to.
         * @return If graph instance is equal to \p other.
         */
        bool operator==(const DirectedGraph& other) const;
        /**
         * Return if graph instance and \p other have different sizes and/or have
         * different edges.
         * @param other Graph to compare to.
         * @return If graph instance is different from \p other.
         */
        bool operator!=(const DirectedGraph& other) const {
            return !(this->operator==(other));
        }

        /**
         * Add directed edge from vertex \p source to \p destination.
         * \warning
         * Use <tt>force=true</tt> with caution as it may create duplicate edges.
         * Since this class isn't designed to handle them, it might behave
         * unexpectedly in some algorithms. Remove duplicate edges with
         * DirectedGraph::removeDuplicateEdges. Use DirectedMultigraph for
         * multigraph support.
         *
         * @param source, destination Index of the source and destination vertices.
         * @param force If \c false, the edge is not added if it already exists.
         *              If \c true, the edge is added without checking its
         *              existence (quicker).
         */
        virtual void addEdge(VertexIndex source, VertexIndex destination, bool force=false);
        void addEdge(const Edge& edge, bool force=false) {
            addEdge(edge.first, edge.second, force);
        }
        /**
         * Add reciprocal edge. Calls DirectedGraph::addEdge for both
         * edge directions.
         * @param vertex1, vertex2 Vertices of reciprocal edges.
         * @param force See `force` of addEdge.
         */
        virtual void addReciprocalEdge(VertexIndex vertex1, VertexIndex vertex2, bool force=false) {
            addEdge(vertex1, vertex2, force); addEdge(vertex2, vertex1, force);
        }
        /// Return if a directed edge connects \p source to \p destination.
        bool hasEdge(VertexIndex source, VertexIndex destination) const;
        /// Remove directed edge (including duplicates) from \p source to
        /// \p destination.
        /// @param source, destination Index of the source and destination vertices.
        virtual void removeEdge(VertexIndex source, VertexIndex destination);
        /// Remove all duplicate edges. Duplicate edges might exist when adding edges
        /// using `force=true`.
        virtual void removeDuplicateEdges();
        /// Remove all self-loops.
        virtual void removeSelfLoops();
        /// Remove all edges that contain \p vertex. It disconnects the vertex
        /// from the graph.
        /// @param vertex Vertex to isolate in the graph.
        virtual void removeVertexFromEdgeList(VertexIndex vertex);
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
        DirectedGraph getSubgraphOf(Iterator begin, Iterator end) const {
            return getSubgraphOf(std::unordered_set<VertexIndex>(begin, end));
        }
        /**
         * Construct a DirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Directed subgraph without vertex remapping. The subgraph has
         *         the same number of vertices than the original graph.
         */
        DirectedGraph getSubgraphOf(const std::unordered_set<VertexIndex>& vertices) const;
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
        getSubgraphWithRemapOf(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOf(std::unordered_set<VertexIndex>(begin, end));
        }
        /**
         * Construct a DirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Directed subgraph and mapping of the original vertex indices
         *         to the subgraph vertex indices.
         */
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>>
        getSubgraphWithRemapOf(const std::unordered_set<VertexIndex>& vertices) const;

        /// Return vertices to which \p vertex is connected.
        const Successors& getOutEdgesOf(VertexIndex vertex) const {
            assertVertexInRange(vertex); return adjacencyList[vertex];
        }
        /// Return in edges of each vertex.
        AdjacencyLists getInEdges() const;
        /// Return a sequence with every edge in the graph.
        virtual std::vector<Edge> getEdges() const;
        /// Construct the adjacency matrix.
        virtual AdjacencyMatrix getAdjacencyMatrix() const;
        /// Count the number of in edges of \p vertex. Use DirectedGraph::getInDegrees
        /// if more than one in degree is needed.
        virtual size_t getInDegreeOf(VertexIndex vertex) const;
        /// Count the number of in edges of each vertex.
        virtual std::vector<size_t> getInDegrees() const;
        /// Count the number of out edges starting from \p vertex.
        virtual size_t getOutDegreeOf(VertexIndex vertex) const;
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
                for (auto& neighbour: graph.getOutEdgesOf(i))
                    stream << neighbour << ", ";
                stream << "\n";
            }
            return stream;
        }

        /// Return VertexIterator of first vertex.
        VertexIterator begin() const { return VertexIterator(0); }
        /// Return VertexIterator of last vertex.
        VertexIterator end() const { return VertexIterator(size); }

        /// Add support for range-based for looping on edges with `for (const Edge& edge: graph.edges)`.
        const Edges edges = Edges(*this);

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
