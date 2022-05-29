#ifndef BASE_GRAPH_TYPES
#define BASE_GRAPH_TYPES

#include <cstddef>
#include <vector>
#include <list>


namespace BaseGraph {

/// Integer that identifies a vertex.
typedef size_t VertexIndex;
/// Pair of vertex indices that represent an edge connecting `Edge.first` to `Edge.second`.
typedef std::pair<VertexIndex, VertexIndex> Edge;
/// Contains the out neighbours of a vertex.
typedef std::list<VertexIndex> Successors;
/// Contains the out neighbours of each vertex.
typedef std::vector<Successors> AdjacencyLists;
/// Matrix where element \f$ A_{ij} \f$ is the number of edges connecting vertex of
/// index \f$ i \f$ to vertex of index \f$ j \f$ .
typedef std::vector<std::vector<size_t>> AdjacencyMatrix;

/// Contains an edge and its associated label in edge-labeled graphs.
template<typename EdgeLabel>
using LabeledEdge = std::tuple<VertexIndex, VertexIndex, EdgeLabel>;

/// Multiplicity of an edge (number of parallel edges) in a multigraph.
typedef unsigned int EdgeMultiplicity;

/// Iterator that allows range-based looping on vertex indices of a graph.
struct VertexIterator {
    VertexIndex position;
    VertexIterator(VertexIndex position) : position(position) {}
    bool operator ==(VertexIterator rhs) {return position == rhs.position;}
    bool operator!=(VertexIterator rhs) {return position != rhs.position;}
    VertexIndex& operator*() {return position;}
    VertexIterator operator++() {++position; return *this;}
    VertexIterator operator++(int) {VertexIterator tmp=VertexIterator(position); operator++(); return tmp;}
};

} // namespace BaseGraph

#endif
