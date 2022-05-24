#ifndef BASE_GRAPH_DIRECTED_GRAPH_H
#define BASE_GRAPH_DIRECTED_GRAPH_H

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include "BaseGraph/types.h"


namespace BaseGraph{

/** Base class for directed graphs. Vertices are identified by their integer index.
 *
 *  By default, self-loops are allowed but multiedges aren't. Vertices can be added but not removed:
 *  it's only possible to remove edges containing a vertex.
 */
class DirectedGraph {

    public:
        /// Construct DirectedGraph with \p size vertices.
        /// @param size Number of vertices. Defaults to 0.
        explicit DirectedGraph(size_t size=0): size(0), edgeNumber(0) {resize(size);}

        template<template<class ...> class Container, class ...Args>
        explicit DirectedGraph(const Container<Edge>& edgeList): DirectedGraph(0) {
            VertexIndex maxIndex=0;
            for (const Edge& edge: edgeList) {
                maxIndex = std::max(edge.first, edge.second);
                if (maxIndex >= getSize())
                    resize(maxIndex+1);
                addEdgeIdx(edge.first, edge.second);
            }
        }

        /// Change number of vertices.
        /// @param size New number of vertices. Required to be greater or equal to current size.
        void resize(size_t size);
        /// Get number of vertices.
        size_t getSize() const { return size; }
        /// Get number of edges.
        size_t getEdgeNumber() const { return edgeNumber; }

        /// Check if graph instance and \p other have the same size and have the same edges.
        /// @param other DirectedGraph to compare to.
        /// @return If graph instance is equal to \p other.
        bool operator==(const DirectedGraph& other) const;
        /// Check if graph instance and \p other have different sizes and/or have different edges.
        /// @param other DirectedGraph to compare to.
        /// @return If graph instance is different from \p other.
        bool operator!=(const DirectedGraph& other) const { return !(this->operator==(other)); }

        /** Add directed edge the from vertex \p source to \p destination.
         * @param source, destination Indices of the source and destination vertices.
         * @param force If set to false, the edge is not added if it already exists. If set to true, the edge is always added, which can create duplicates.
         *              Because this class doesn't handle multiedges, the methods and metrics computed on the graph might have unexepected behaviors. If one is
         *              interested in using directed multigraphs, the class DirectedMultigraph is more appropriate. Defaults to false.
         */
        virtual void addEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false);
        /** Add reciprocal edges. Is equivalent to calling addEdgeIdx for both edge directions.
         * @param vertex1, vertex2 Vertices of reciprocal edges.
         * @param force If set to false, edges are not added if they already exist. If set to true, edges are always added, which can create duplicates.
         *              Because this class doesn't handle multiedges, the methods and metrics computed on the graph might have unexepected behaviors. If one is
         *              interested in using directed multigraphs, the class DirectedMultigraph is more appropriate. Defaults to false.
         */
        virtual void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false) { addEdgeIdx(vertex1, vertex2, force); addEdgeIdx(vertex2, vertex1, force); }
        /// Return if the directed edge from \p source to \p destination exists.
        /// @param source, destination Indices of the source and destination vertices.
        bool isEdgeIdx(VertexIndex source, VertexIndex destination) const;
        /// Remove all directed edges (including multiedges) from \p source to \p destination.
        /// @param source, destination Indices of the source and destination vertices.
        virtual void removeEdgeIdx(VertexIndex source, VertexIndex destination);
        /// Remove all duplicate edges. Duplicate edges can be created when `force=true` is used when adding edges.
        virtual void removeDuplicateEdges();
        /// Remove all self-loops.
        virtual void removeSelfLoops();
        /// Remove all directed edges that contain `vertex`. It disconnects the vertex from the graph.
        /// @param vertex Vertex to isolate in the graph.
        virtual void removeVertexFromEdgeListIdx(VertexIndex vertex);
        /// Remove all edges.
        virtual void clearEdges();

        /** Construct a DirectedGraph that only contains the edges with vertices enumerated by the iterator.
         * @tparam Iterator Any iterator type that is valid in the construction of `std::unordered_set`.
         * @param begin, end Iterators that enumerate vertices to preserve in subgraph.
         * @return Directed subgraph without vertex remapping. The subgraph has the same number of vertices than the original graph.
         */
        template <typename Iterator>
        DirectedGraph getSubgraphOfIdx(Iterator begin, Iterator end) const { return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        /** Construct a DirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Directed subgraph without vertex remapping. The subgraph has the same number of vertices than the original graph.
         */
        DirectedGraph getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        /** Construct a DirectedGraph that only contains the edges with vertices enumerated by the iterator.
         *  is also returned.
         * @tparam Iterator Any iterator type that is valid in the construction of `std::unordered_set`.
         * @param begin, end Iterators that enumerate vertices to preserve in subgraph.
         * @return Directed subgraph and mapping of the original vertex indices to the subgraph vertex indices.
         */
        template <typename Iterator>
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        /** Construct a DirectedGraph that only contains the edges in \p vertices.
         * @param vertices Vertices to include in the subgraph.
         * @return Directed subgraph and mapping of the original vertex indices to the subgraph vertex indices.
         */
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;

        /// Get vertices to which \p vertex is connected to.
        const Successors& getOutEdgesOfIdx(VertexIndex vertex) const { assertVertexInRange(vertex); return adjacencyList[vertex]; }
        /// Get incoming neigbhours of each vertex.
        AdjacencyLists getInEdges() const;
        /// Construct the adjacency matrix.
        virtual AdjacencyMatrix getAdjacencyMatrix() const;
        /// Count the number of incoming edges of \p vertex.
        virtual size_t getInDegreeOfIdx(VertexIndex vertex) const;
        /// Count the number of incoming edges of each vertex.
        virtual std::vector<size_t> getInDegrees() const;
        /// Count the number of outgoing edges starting from \p vertex.
        virtual size_t getOutDegreeOfIdx(VertexIndex vertex) const;
        /// Count the number of outgoing edges of each vertex.
        virtual std::vector<size_t> getOutDegrees() const;

        /// Construct a DirectedGraph where each directed edge is reversed.
        DirectedGraph getReversedGraph() const;

        /// Output in text format the graph's size and edges to a given `std::stream` object.
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

        /// Iterator that allows range-based looping on vertices of the graph.
        struct iterator {
            VertexIndex position;
            iterator(VertexIndex position) : position(position) {}
            bool operator ==(iterator rhs) {return position == rhs.position;}
            bool operator!=(iterator rhs) {return position != rhs.position;}
            VertexIndex& operator*() {return position;}
            iterator operator++() {++position; return *this;}
            iterator operator++(int) {iterator tmp=iterator(position); operator++(); return tmp;}
        };
        iterator begin() const { return iterator(0); }
        iterator end() const { return iterator(size); }

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
