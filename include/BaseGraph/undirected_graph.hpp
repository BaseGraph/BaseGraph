#ifndef BASE_GRAPH_UNDIRECTED_GRAPH_HPP
#define BASE_GRAPH_UNDIRECTED_GRAPH_HPP

#include <iostream>
#include <set>
#include <stdexcept>
#include <unordered_map>

#include "BaseGraph/boost_hash.hpp"
#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"

namespace BaseGraph {

/**
 * Undirected graph with edge labels, self-loops and without multiedges. When no
 *
 * Vertices are identified an integer index between 0 and \c size -1. Vertices
 * can be added using @ref resize. Vertices cannot be
 * removed because it requires reindexing. However, a vertex can be effectively
 * removed by erasing all of its edges with @ref
 * removeVertexFromEdgeList.
 *
 * @tparam EdgeLabel Container of edge information. Requires a default
 * constructor.
 */
template <typename EdgeLabel>
class LabeledUndirectedGraph : protected LabeledDirectedGraph<EdgeLabel> {
    using Directed = LabeledDirectedGraph<EdgeLabel>;

  public:
    /// Constructs an empty graph with \p size vertices.
    /// @param size Number of vertices.
    explicit LabeledUndirectedGraph<EdgeLabel>(size_t size = 0)
        : Directed(size) {}

    /**
     * Constructs a graph containing each in \p edgeSequence. The graph size is
     * adjusted to the largest index in \p edgeSequence. This method is
     * available only for an unlabeled graph (@ref BaseGraph::UndirectedGraph).
     *
     * @tparam Container Any container of @ref BaseGraph::Edge that supports
     * range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * usable.
     *
     * For example:
     * \code{.cpp}
     * std::list<BaseGraph::Edge> edges = {{0, 2}, {0, 1}, {0, 0}, {5, 10}};
     * BaseGraph::UndirectedGraph graph(edges);
     * \endcode
     */
    template <template <class...> class Container, class... Args,
              typename U = EdgeLabel>
    explicit LabeledUndirectedGraph<EdgeLabel>(
        const Container<Edge, Args...> &edgeSequence,
        typename std::enable_if<std::is_same<U, NoLabel>::value,
                                long long int>::type * = 0)
        : Directed(0) {
        VertexIndex maxIndex = 0;
        for (const Edge &edge : edgeSequence) {
            maxIndex = std::max(edge.first, edge.second);
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addEdge(edge.first, edge.second);
        }
    }

    /**
     * Constructs graph containing every vertex in \p edgeSequence. Graph size
     * is adjusted to the largest index in \p edgeSequence. This method is
     * available to any labeled graph.
     *
     * @tparam Container Any container of BaseGraph::LabeledEdge that supports
     * range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * usable.
     *
     * For example:
     * \code{.cpp}
     * std::list<BaseGraph::LabeledEdge<std::string>> labeledEdges =
     *         {{0, 2, "a"}, {0, 1, "b"}, {0, 0, "c"}, {5, 10, "d"}};
     * BaseGraph::LabeledUndirectedGraph graph(labeledEdges);
     * \endcode
     */
    template <template <class...> class Container, class... Args>
    explicit LabeledUndirectedGraph<EdgeLabel>(
        const Container<LabeledEdge<EdgeLabel>, Args...> &edgeSequence)
        : LabeledUndirectedGraph(0) {

        VertexIndex maxIndex = 0;
        for (const LabeledEdge<EdgeLabel> &labeledEdge : edgeSequence) {
            maxIndex =
                std::max(std::get<0>(labeledEdge), std::get<1>(labeledEdge));
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addEdge(std::get<0>(labeledEdge), std::get<1>(labeledEdge),
                    std::get<2>(labeledEdge));
        }
    }

    /// Constructs a undirected graph containing each edge of \p directedGraph.
    explicit LabeledUndirectedGraph<EdgeLabel>(const Directed &directedGraph)
        : LabeledUndirectedGraph(directedGraph.getSize()) {
        for (VertexIndex i : directedGraph)
            for (VertexIndex j : directedGraph.getOutNeighbours(i))
                addEdge(i, j, directedGraph.getEdgeLabel(i, j));
    }

    using Directed::getEdgeNumber;
    using Directed::getSize;
    using Directed::resize;

    /// @copydoc LabeledDirectedGraph::operator==
    bool operator==(const LabeledUndirectedGraph<EdgeLabel> &other) const {
        return Directed::operator==(other);
    }
    /// @copydoc LabeledDirectedGraph::operator!=
    bool operator!=(const LabeledUndirectedGraph<EdgeLabel> &other) const {
        return !(this->operator==(other));
    }

    /**
     * Adds labeled edge between vertex \p vertex1 and \p vertex2.
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * @ref removeDuplicateEdges.
     *
     * @param vertex1, vertex2 Index of the vertices to be connected.
     * @param label Label of the edge created.
     * @param force If \c false, the edge is not added if it already exists.
     *              If \c true, the edge is added without checking its
     *              existence (quicker).
     */
    void addEdge(VertexIndex vertex1, VertexIndex vertex2,
                 const EdgeLabel &label, bool force = false);
    /**
     * Adds edge connecting \p vertex1 and \p vertex2 with the default label
     * constructor. This is the suggested method to add edges in a @ref
     * BaseGraph::UndirectedGraph. See the other @ref
     * addEdge(VertexIndex,VertexIndex,const
     * EdgeLabel&, bool) "overload".
     */
    void addEdge(VertexIndex vertex1, VertexIndex vertex2, bool force = false) {
        addEdge(vertex1, vertex2, EdgeLabel(), force);
    }

    /// Returns if an edge of any label connects \p vertex1 to \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
        auto edge = orderedEdge(vertex1, vertex2);
        return Directed::hasEdge(edge.first, edge.second);
    }

    /// Returns if an edge of label \p label connects \p vertex1
    /// to \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2,
                 const EdgeLabel &label) const {
        return hasEdge(vertex1, vertex2) &&
               (getEdgeLabel(vertex1, vertex2, false) == label);
    }

    /// Removes edges (including duplicates) between \p vertex1 and
    /// \p vertex2.
    void removeEdge(VertexIndex vertex1, VertexIndex vertex2);

    using Directed::getOutNeighbours;

    /// Same as @ref getOutNeighbours.
    const Successors &getNeighbours(VertexIndex vertex) const {
        return getOutNeighbours(vertex);
    }

    /**
     * Returns label of edge connecting \p vertex1 and \p vertex2.
     * @param vertex1, vertex2 Index of the vertices of the edge.
     * @param throwIfInexistent If `true`, the method throws
     * `std::invalid_argument` if the edge doesn't exist. If `false`, the method
     * returns `EdgeLabel()` when the edge isn't found.
     *
     * @return Label of the edge.
     */
    EdgeLabel getEdgeLabel(VertexIndex vertex1, VertexIndex vertex2,
                           bool throwIfInexistent = true) const {
        auto edge = orderedEdge(vertex1, vertex2);
        return Directed::getEdgeLabel(edge.first, edge.second,
                                      throwIfInexistent);
    }
    /**
     * Changes the label of edge connecting \p vertex1 and \p vertex2.
     * @param vertex1, vertex2 Index of the vertices of the edge.
     * @param label New label value for the edge.
     * @param force If `true`, the method will not check if the edge exists.
     * This may create a label for an inexistent edge. If `false`, the method
     * throws `std::invalid_argument` if the directed edge doesn't exist.
     */
    void setEdgeLabel(VertexIndex vertex1, VertexIndex vertex2,
                      const EdgeLabel &label, bool force = false) {
        auto edge = orderedEdge(vertex1, vertex2);
        return Directed::setEdgeLabel(edge.first, edge.second, label, force);
    }

    /// @copydoc LabeledDirectedGraph::removeDuplicateEdges
    void removeDuplicateEdges();

    /// @copydoc LabeledDirectedGraph::removeSelfLoops
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
    }

    /**
     * Returns the number of vertices connected to \p vertex.
     * @param vertex Index of a vertex.
     * @param countSelfLoopsTwice If `true`, self-loops are counted twice. If
     * `false`, self-loops are counted once. If there are no self-loops, set to
     * `false` for better performance, i.e. constant instead of linear
     * complexity.
     *
     * @return Degree of vertex \p vertex
     */
    size_t getDegree(VertexIndex vertex, bool countSelfLoopsTwice = true) const;

    /// Return the degree of every vertex. See @ref getDegree.
    std::vector<size_t> getDegrees(bool countSelfLoopsTwice = true) const {
        std::vector<size_t> degrees(getSize());
        for (VertexIndex i : *this)
            degrees[i] = getDegree(i, countSelfLoopsTwice);
        return degrees;
    }

    /// @copydoc LabeledDirectedGraph::getAdjacencyMatrix
    AdjacencyMatrix getAdjacencyMatrix(bool countSelfLoopsTwice = true) const;

    /// Constructs a @ref LabeledDirectedGraph containing each reciprocal edge
    /// of the LabeledUndirectedGraph instance.
    Directed getDirectedGraph() const;

    /// @copydoc LabeledDirectedGraph::removeVertexFromEdgeList
    void removeVertexFromEdgeList(VertexIndex vertex);

    using Directed::assertVertexInRange;
    using Directed::begin;
    using Directed::clearEdges;
    using Directed::end;

    /// @copydoc LabeledDirectedGraph::operator<<
    friend std::ostream &
    operator<<(std::ostream &stream,
               const LabeledUndirectedGraph<EdgeLabel> &graph) {
        stream << "Undirected graph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getOutNeighbours(i))
                stream << neighbour << ", ";
            stream << "\n";
        }
        return stream;
    }

    /// @copydoc LabeledDirectedGraph::Edges
    struct Edges {
        struct constEdgeIterator {
            VertexIndex vertex;
            const VertexIndex endVertex;
            Successors::const_iterator neighbour;
            const LabeledUndirectedGraph<EdgeLabel> &graph;

            constEdgeIterator(const LabeledUndirectedGraph<EdgeLabel> &graph,
                              VertexIndex vertex,
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
                    ++neighbour;
                    while (neighbour == graph.getOutNeighbours(vertex).end() &&
                           vertex != endVertex)
                        neighbour = graph.getOutNeighbours(++vertex).begin();
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
                return neighbour == graph.getOutNeighbours(vertex).end() &&
                       vertex == endVertex;
            }
        };
        const LabeledUndirectedGraph<EdgeLabel> &graph;
        Edges(const LabeledUndirectedGraph<EdgeLabel> &graph) : graph(graph) {}

        constEdgeIterator begin() const {
            VertexIndex endVertex = getEndVertex(graph);

            VertexIndex vertexWithFirstEdge = 0;
            auto neighbour = graph.getOutNeighbours(0).begin();
            while (neighbour ==
                       graph.getOutNeighbours(vertexWithFirstEdge).end() &&
                   vertexWithFirstEdge != endVertex)
                neighbour =
                    graph.getOutNeighbours(++vertexWithFirstEdge).begin();

            return constEdgeIterator(graph, vertexWithFirstEdge, neighbour);
        }
        constEdgeIterator end() const {
            VertexIndex lastVertex = getEndVertex(graph);
            return constEdgeIterator(graph, lastVertex,
                                     graph.getOutNeighbours(lastVertex).end());
        }

        static VertexIndex
        getEndVertex(const LabeledUndirectedGraph<EdgeLabel> &graph) {
            auto vertexNumber = graph.getSize();
            if (vertexNumber == 0)
                return 0;
            return vertexNumber - 1;
        }
    };
    /// Creates @ref LabeledUndirectedGraph::Edges object that supports range-based for loop.
    Edges edges() const { return Edges(*this); }

  protected:
    static Edge orderedEdge(VertexIndex i, VertexIndex j) {
        return i < j ? Edge{i, j} : Edge{j, i};
    }
    void setLabel(VertexIndex i, VertexIndex j, const EdgeLabel &label) {
        Directed::_setLabel(orderedEdge(i, j), label);
    }
};

/// Unlabeled undirected graph.
using UndirectedGraph = LabeledUndirectedGraph<NoLabel>;

template <typename EdgeLabel>
void LabeledUndirectedGraph<EdgeLabel>::addEdge(VertexIndex vertex1,
                                                VertexIndex vertex2,
                                                const EdgeLabel &label,
                                                bool force) {
    if (force || !hasEdge(vertex1, vertex2)) {
        if (vertex1 != vertex2)
            Directed::adjacencyList[vertex1].push_back(vertex2);
        Directed::adjacencyList[vertex2].push_back(vertex1);

        setLabel(vertex1, vertex2, label);
        ++Directed::edgeNumber;
    }
}

template <typename EdgeLabel>
void LabeledUndirectedGraph<EdgeLabel>::removeEdge(VertexIndex vertex1,
                                                   VertexIndex vertex2) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    size_t sizeBefore = Directed::adjacencyList[vertex1].size();
    Directed::adjacencyList[vertex1].remove(vertex2);
    size_t sizeDifference =
        sizeBefore - Directed::adjacencyList[vertex1].size();

    if (sizeDifference > 0) {
        Directed::adjacencyList[vertex2].remove(vertex1);
        Directed::edgeNumber -= sizeDifference;
        Directed::edgeLabels.erase(orderedEdge(vertex1, vertex2));
    }
}

template <typename EdgeLabel>
void LabeledUndirectedGraph<EdgeLabel>::removeDuplicateEdges() {
    std::set<VertexIndex> seenVertices;
    Successors::iterator j;

    for (VertexIndex i : *this) {
        j = Directed::adjacencyList[i].begin();

        while (j != Directed::adjacencyList[i].end()) {
            if (!seenVertices.count(*j)) {
                seenVertices.insert(*j);
                ++j;
            } else {
                if (i <= *j) {
                    --Directed::edgeNumber;
                }
                Directed::adjacencyList[i].erase(j++);
            }
        }
        seenVertices.clear();
    }
}

template <typename EdgeLabel>
void LabeledUndirectedGraph<EdgeLabel>::removeVertexFromEdgeList(
    VertexIndex vertex) {
    assertVertexInRange(vertex);

    Successors::iterator j;
    for (VertexIndex i : *this) {
        j = Directed::adjacencyList[i].begin();
        while (j != Directed::adjacencyList[i].end())
            if (i == vertex || *j == vertex) {
                if (i <= *j) {
                    --Directed::edgeNumber;
                }
                Directed::adjacencyList[i].erase(j++);
            } else {
                ++j;
            }
    }
}

template <typename EdgeLabel>
LabeledDirectedGraph<EdgeLabel>
LabeledUndirectedGraph<EdgeLabel>::getDirectedGraph() const {
    LabeledDirectedGraph<EdgeLabel> directedGraph(getSize());

    for (auto edge : edges())
        if (edge.first < edge.second)
            directedGraph.addReciprocalEdge(edge.first, edge.second, true);
        else if (edge.first == edge.second)
            directedGraph.addEdge(edge.first, edge.second,
                                  getEdgeLabel(edge.first, edge.second), true);
    return directedGraph;
}

template <typename EdgeLabel>
size_t
LabeledUndirectedGraph<EdgeLabel>::getDegree(VertexIndex vertex,
                                             bool countSelfLoopsTwice) const {
    assertVertexInRange(vertex);

    if (!countSelfLoopsTwice)
        return Directed::adjacencyList[vertex].size();

    size_t degree = 0;
    for (auto neighbour : getNeighbours(vertex))
        degree += neighbour == vertex ? 2 : 1;
    return degree;
}

template <typename EdgeLabel>
AdjacencyMatrix LabeledUndirectedGraph<EdgeLabel>::getAdjacencyMatrix(
    bool countSelfLoopsTwice) const {
    const size_t &_size = getSize();
    AdjacencyMatrix adjacencyMatrix(_size, std::vector<size_t>(_size, 0));

    for (auto i : *this)
        for (auto j : getOutNeighbours(i))
            adjacencyMatrix[i][j] += i == j && countSelfLoopsTwice ? 2 : 1;

    return adjacencyMatrix;
}

} // namespace BaseGraph

#endif
