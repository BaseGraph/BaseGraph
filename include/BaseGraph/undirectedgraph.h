#ifndef BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H
#define BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H

#include "BaseGraph/directedgraph.h"

namespace BaseGraph {

/**
 * Base class for undirected graphs with self-loops and without multiedges.
 *
 * Vertices are identified by their integer index between 0 and \c size -1.
 * Vertices can be added using _UndirectedGraph::resize. Removing vertices is
 * not supported as it would require reindexing.  However, a vertex can be
 * effectively removed by erasing all of its edges with
 * _UndirectedGraph::removeVertexFromEdgeList.
 */
class _UndirectedGraph : protected _DirectedGraph {
  public:
    struct Edges {
        struct constEdgeIterator {
            VertexIndex vertex;
            const VertexIndex endVertex;
            Successors::const_iterator neighbour;
            const _UndirectedGraph &graph;

            constEdgeIterator(const _UndirectedGraph &graph, VertexIndex vertex,
                              Successors::const_iterator neighbour)
                : vertex(vertex), neighbour(neighbour), graph(graph),
                  endVertex(getEndVertex(graph)) {}

            bool operator==(constEdgeIterator rhs) const {
                return vertex == rhs.vertex && neighbour == rhs.neighbour;
            }
            bool operator!=(constEdgeIterator rhs) const {
                return !this->operator==(rhs);
            }
            Edge operator*() { return {vertex, *neighbour}; }
            constEdgeIterator operator++() {
                do {
                    neighbour++;
                    while (neighbour == graph.getOutEdgesOf(vertex).end() &&
                           vertex != endVertex)
                        neighbour = graph.getOutEdgesOf(++vertex).begin();
                } while (!hasReachedEnd() && vertex > *neighbour);

                return *this;
            }
            constEdgeIterator operator++(int) {
                constEdgeIterator tmp =
                    constEdgeIterator(graph, vertex, neighbour);
                operator++();
                return tmp;
            }
            bool hasReachedEnd() const {
                return neighbour == graph.getOutEdgesOf(vertex).end() &&
                       vertex == endVertex;
            }
        };
        const _UndirectedGraph &graph;
        Edges(const _UndirectedGraph &graph) : graph(graph) {}

        constEdgeIterator begin() const {
            VertexIndex endVertex = getEndVertex(graph);

            VertexIndex vertexWithFirstEdge = 0;
            auto neighbour = graph.getOutEdgesOf(0).begin();
            while (neighbour ==
                       graph.getOutEdgesOf(vertexWithFirstEdge).end() &&
                   vertexWithFirstEdge != endVertex)
                neighbour = graph.getOutEdgesOf(++vertexWithFirstEdge).begin();

            return constEdgeIterator(graph, vertexWithFirstEdge, neighbour);
        }
        constEdgeIterator end() const {
            VertexIndex lastVertex = getEndVertex(graph);
            return constEdgeIterator(graph, lastVertex,
                                     graph.getOutEdgesOf(lastVertex).end());
        }

        static VertexIndex getEndVertex(const _UndirectedGraph &graph) {
            auto vertexNumber = graph.getSize();
            if (vertexNumber == 0)
                return 0;
            return vertexNumber - 1;
        }
    };

  public:
    /// Construct _UndirectedGraph with \p size vertices.
    /// @param size Number of vertices.
    explicit _UndirectedGraph(size_t size = 0) : _DirectedGraph(size) {}
    explicit _UndirectedGraph(const _DirectedGraph &);

    /**
     * Construct _UndirectedGraph containing every vertex in \p edges.
     * Graph size is adjusted to the largest index in \p edges.
     *
     * @tparam Container Any template container that accepts type
     * BaseGraph::Edge and that supports range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * accepted.
     *
     * @param edges Edges to add into the graph.
     */
    template <template <class...> class Container, class... Args>
    explicit _UndirectedGraph(const Container<Edge, Args...> &edgeList)
        : _UndirectedGraph(0) {
        VertexIndex maxIndex = 0;
        for (const Edge &edge : edgeList) {
            maxIndex = std::max(edge.first, edge.second);
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addEdge(edge.first, edge.second);
        }
    }

    /// Construct _DirectedGraph containing each reciprocal edge of
    /// _UndirectedGraph instance.
    _DirectedGraph getDirectedGraph() const;

    using _DirectedGraph::getEdgeNumber;
    using _DirectedGraph::getSize;
    using _DirectedGraph::resize;

    /**
     * Return if graph instance and \p other have the same size and have the
     * same edges.
     * @param other Graph to compare to.
     * @return If graph instance is equal to \p other.
     */
    bool operator==(const _UndirectedGraph &other) const {
        return _DirectedGraph::operator==(other);
    }
    /**
     * Return if graph instance and \p other have different sizes and/or have
     * different edges.
     * @param other Graph to compare to.
     * @return If graph instance is different from \p other.
     */
    bool operator!=(const _UndirectedGraph &other) const {
        return _DirectedGraph::operator!=(other);
    }

    /**
     * Add edge between \p vertex1 and \p vertex2.
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * _UndirectedGraph::removeDuplicateEdges.
     * Use UndirectedMultigraph for multigraph support.
     *
     * @param vertex1, vertex2 Index of the vertices to be connected.
     * @param force If \c false, the edge is not added if it already exists.
     *              If \c true, the edge is added without checking its existence
     * (quicker).
     */
    void addEdge(VertexIndex vertex1, VertexIndex vertex2, bool force = false);
    void addEdge(const Edge &edge, bool force = false) {
        addEdge(edge.first, edge.second, force);
    }
    /// Return if \p vertex1 is connected to \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const;
    /// Remove edge (including duplicates) between \p vertex1 and \p vertex2.
    void removeEdge(VertexIndex vertex1, VertexIndex vertex2);
    void removeVertexFromEdgeList(VertexIndex vertex);
    void removeDuplicateEdges();
    using _DirectedGraph::clearEdges;
    using _DirectedGraph::removeSelfLoops;

    /**
     * Construct a _UndirectedGraph that only contains the edges with vertices
     * enumerated by the iterator.
     *
     * @tparam Iterator Any iterator type that is valid in the construction of
     * <a
     * href="https://en.cppreference.com/w/cpp/container/unordered_set">std::unordered_set</a>.
     * @param begin, end Iterators that enumerate vertices to preserve in
     * subgraph.
     * @return Undirected subgraph without vertex remapping. The subgraph has
     * the same number of vertices than the original graph.
     */
    template <typename Iterator>
    _UndirectedGraph getSubgraphOf(Iterator begin, Iterator end) const {
        return getSubgraphOf(std::unordered_set<VertexIndex>(begin, end));
    };
    /**
     * Construct a _UndirectedGraph that only contains the edges in \p vertices.
     * @param vertices Vertices to include in the subgraph.
     * @return Undirected subgraph without vertex remapping. The subgraph has
     *         the same number of vertices than the original graph.
     */
    _UndirectedGraph
    getSubgraphOf(const std::unordered_set<VertexIndex> &vertices) const;
    /**
     * Construct a _UndirectedGraph that only contains the edges with vertices
     * enumerated by the iterator.
     *
     * @tparam Iterator Any iterator type that is valid in the construction of
     * <a
     * href="https://en.cppreference.com/w/cpp/container/unordered_set">std::unordered_set</a>.
     * @param begin, end Iterators that enumerate vertices to preserve in
     * subgraph.
     * @return Undirected subgraph and mapping of the original vertex indices to
     * the subgraph vertex indices.
     */
    template <typename Iterator>
    std::pair<_UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>>
    getSubgraphWithRemapOf(Iterator begin, Iterator end) const {
        return getSubgraphWithRemapOf(
            std::unordered_set<VertexIndex>(begin, end));
    };
    /**
     * Construct a _UndirectedGraph that only contains the edges in \p vertices.
     * @param vertices Vertices to include in the subgraph.
     * @return Undirected subgraph and mapping of the original vertex indices
     *         to the subgraph vertex indices.
     */
    std::pair<_UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>>
    getSubgraphWithRemapOf(
        const std::unordered_set<VertexIndex> &vertices) const;

    using _DirectedGraph::getOutEdgesOf;
    const Successors &getNeighboursOf(VertexIndex vertex) const {
        return getOutEdgesOf(vertex);
    }

    std::vector<Edge> getEdges() const;
    AdjacencyMatrix getAdjacencyMatrix() const;
    /**
     * Return the number of vertices connected to \p vertex.
     * @param vertices Index of a vertex.
     * @param countSelfLoopsTwice If `true`, self-loops are counted twice. If
     * `false`, self-loops are counted once. If there are no self-loops, set to
     * `false` for reduced complexity.
     *
     * @return degree of vertex \p vertex
     */
    size_t getDegreeOf(VertexIndex vertex,
                       bool countSelfLoopsTwice = true) const;
    /// Return the degree of every vertex. See _UndirectedGraph::getDegreeOf for
    /// argument \p countSelfLoopsTwice.
    std::vector<size_t> getDegrees(bool countSelfLoopsTwice = true) const;

    friend std::ostream &operator<<(std::ostream &stream,
                                    const _UndirectedGraph &graph) {
        stream << "Undirected graph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getOutEdgesOf(i))
                stream << neighbour << ", ";
            stream << "\n";
        }
        return stream;
    }
    Edge getSmallestAdjacency(VertexIndex vertex1, VertexIndex vertex2) const {
        return getDegreeOf(vertex1, false) < getDegreeOf(vertex2, false)
                   ? Edge{vertex1, vertex2}
                   : Edge{vertex2, vertex1};
    }

    using _DirectedGraph::begin;
    using _DirectedGraph::end;

    /// Add support for range-based for looping on edges with `for (const Edge&
    /// edge: graph.edges())`.
    Edges edges() const { return Edges(*this); }
};

} // namespace BaseGraph

#endif
