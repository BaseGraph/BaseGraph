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
class DirectedGraph{

    public:
        explicit DirectedGraph(size_t size=0): size(0), edgeNumber(0) {resize(size);}

        /// Change number of vertices. New size is required to be greater or equal to current size.
        void resize(size_t size);
        /// Get number of vertices.
        size_t getSize() const { return size; }
        /// Get number of edges.
        size_t getEdgeNumber() const { return edgeNumber; }

        /// Check if DirectedGraph have the same size and have the same edges.
        bool operator==(const DirectedGraph& other) const;
        /// Check if DirectedGraph have different sizes and/or have different edges.
        bool operator!=(const DirectedGraph& other) const { return !(this->operator==(other)); }

        /** Add to graph the directed edge from vertex `source` to `destination`.
         * @param force If set to false, the edge is not added if it already exists. If set to true, the edge is always added, which can create multiedges.
         *              Since these multiedges are not exepected, the methods and metrics computed on the graph might have unexepected behaviors. If one is
         *              interested in using directed multigraphs, the class DirectedMultigraph is more appropriate.
         */
        void addEdgeIdx          (VertexIndex source, VertexIndex destination, bool force=false);
        /// Add to graph the directed edge `edge`. Is equivalent to calling `addEdgeIdx` with the source and destination vertices.
        void addEdgeIdx          (const Edge& edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        /// Add to graph two directed edges: from `vertex1` to `vertex2` and from `vertex2` to `vertex1`. Is equivalent to calling `addEdgeIdx` for both edge directions.
        void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false) { addEdgeIdx(vertex1, vertex2, force); addEdgeIdx(vertex2, vertex1, force); }
        /// Add to graph both orientations of `edge`.
        void addReciprocalEdgeIdx(const Edge& edge, bool force=false) {addReciprocalEdgeIdx(edge, force);}
        /// Return if the directed edge from `source` to `destination` exists.
        bool isEdgeIdx           (VertexIndex source, VertexIndex destination) const;
        /// Return if the directed edge `edge` exists.
        bool isEdgeIdx           (const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }
        /// Remove all directed edges (including multiedges) starting from `source` going to `destination`.
        void removeEdgeIdx       (VertexIndex source, VertexIndex destination);
        /// Remove all directed edges (including multiedges) `edge`.
        void removeEdgeIdx       (const Edge& edge) {removeEdgeIdx(edge.first, edge.second);}
        /// Remove all multiedges.
        void removeMultiedges();
        /// Remove all self-loops.
        void removeSelfLoops();
        /// Remove all directed edges that contain `vertex`, which disconnects the vertex from the graph.
        void removeVertexFromEdgeListIdx(VertexIndex vertex);
        /// Remove all edges.
        void clearEdges();

        /// Construct a DirectedGraph that only contains the edges with vertices enumerated by the iterator. Returned graph has the same size as the caller object
        /// such that the vertices keep the same indices.
        template <typename Iterator>
        DirectedGraph getSubgraphOfIdx(Iterator begin, Iterator end) const { return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        /// Construct a DirectedGraph that only contains the edges with `vertices`. Returned graph has the same size as the caller object
        /// such that the vertices keep the same indices.
        DirectedGraph getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        /// Construct a DirectedGraph that only contains the vertices enumerated by the iterator. The mapping of the original vertex indices to the subgraph vertex indices
        /// is also returned.
        template <typename Iterator>
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        /// Construct a DirectedGraph that only contains `vertices`. The mapping of the original vertex indices to the subgraph vertex indices
        /// is also returned.
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;

        /// Get vertices to which `vertex` is connected to.
        const Successors&   getOutEdgesOfIdx(VertexIndex vertex) const { assertVertexInRange(vertex); return adjacencyList[vertex]; }
        /// Get incoming neigbhours of each vertex.
        AdjacencyLists      getInEdges() const;
        /// Construct the adjacency matrix.
        AdjacencyMatrix     getAdjacencyMatrix() const;
        /// Count the number of incoming edges of `vertex`.
        size_t              getInDegreeOfIdx(VertexIndex vertex) const;
        /// Count the number of incoming edges of each vertex.
        std::vector<size_t> getInDegrees() const;
        /// Count the number of outgoing edges starting from `vertex`.
        size_t              getOutDegreeOfIdx(VertexIndex vertex) const;
        /// Count the number of outgoing edges of each vertex.
        std::vector<size_t> getOutDegrees() const;

        /// Construct a DirectedGraph where each edge has its orientation reversed.
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

        iterator begin() const {return iterator(0);}
        iterator end() const {return iterator(size);}

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
