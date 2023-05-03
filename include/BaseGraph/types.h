#ifndef BASE_GRAPH_TYPES_H
#define BASE_GRAPH_TYPES_H

#include <cstddef>
#include <list>
#include <tuple>
#include <vector>

namespace BaseGraph {

/// Integer that identifies a vertex.
typedef unsigned int VertexIndex;

/// Empty type used for unlabeled graphs.
struct NoLabel {
    bool operator==(const NoLabel &) const { return true; }
};

/// Pair of vertex indices that represents an edge connecting `Edge::first` to
/// `Edge::second`.
typedef std::pair<VertexIndex, VertexIndex> Edge;
/// Contains the out neighbours of a vertex.
typedef std::list<VertexIndex> Successors;
/// Contains the out neighbours of each vertex.
typedef std::vector<Successors> AdjacencyLists;

/// Contains an edge and its associated label in edge-labeled graphs.
template <typename EdgeLabel>
using LabeledEdge = std::tuple<VertexIndex, VertexIndex, EdgeLabel>;

/// Multiplicity of an edge (number of parallel edges) in a multigraph.
typedef unsigned int EdgeMultiplicity;

/// Weight of an edge in a weighted graph.
typedef double EdgeWeight;

/// Matrix where element \f$ a_{ij} \f$ is the number of edges connecting vertex
/// of index \f$ i \f$ to vertex of index \f$ j \f$ .
typedef std::vector<std::vector<size_t>> AdjacencyMatrix;

/// Matrix where element \f$ a_{ij} \f$ is the weight of the edge connecting
/// vertex of index \f$ i \f$ to vertex of index \f$ j \f$. The weight is 0 if
/// the edge doesn't exist.
typedef std::vector<std::vector<EdgeWeight>> WeightMatrix;

/// Iterator that allows range-based loops on a graph's vertices.
struct VertexIterator {
    VertexIndex position;
    VertexIterator(VertexIndex position) : position(position) {}
    bool operator==(VertexIterator rhs) { return position == rhs.position; }
    bool operator!=(VertexIterator rhs) { return position != rhs.position; }
    VertexIndex &operator*() { return position; }
    VertexIterator operator++() {
        ++position;
        return *this;
    }
    VertexIterator operator++(int) {
        VertexIterator tmp = VertexIterator(position);
        operator++();
        return tmp;
    }
};

} // namespace BaseGraph

#endif
