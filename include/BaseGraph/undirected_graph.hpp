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
 * BaseGraph::UndirectedGraph in which each edge has a label.
 *
 * Inherits from BaseGraph::UndirectedGraph publicly. Since this allows the
 * creation of edges without labels, a missing edge label is built with its
 * default constructor.
 *
 * @tparam EdgeLabel Container of edge information. Requires a default
 * constructor.
 */
template <typename EdgeLabel>
class LabeledUndirectedGraph : protected LabeledDirectedGraph<EdgeLabel> {
    using Directed = LabeledDirectedGraph<EdgeLabel>;

  public:
    /// Construct EdgeLabeledUndirectedGraph with \p size vertices.
    /// @param size Number of vertices.
    explicit LabeledUndirectedGraph<EdgeLabel>(size_t size = 0)
        : Directed(size) {}

    /**
     * Construct EdgeLabeledUndirectedGraph containing every vertex in \p edges.
     * Graph size is adjusted to the largest index in \p edges.
     *
     * @tparam Container Any template container that accepts
     * BaseGraph::LabeledEdge and that supports range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * accepted.
     *
     * @param edges Edges to add into the graph.
     */
    template <template <class...> class Container, class... Args,
              typename U = EdgeLabel>
    explicit LabeledUndirectedGraph<EdgeLabel>(
        const Container<Edge, Args...> &edges,
        typename std::enable_if<std::is_same<U, NoLabel>::value,
                                long long int>::type * = 0)
        : Directed(0) {
        VertexIndex maxIndex = 0;
        for (const Edge &edge : edges) {
            maxIndex = std::max(edge.first, edge.second);
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addEdge(edge.first, edge.second);
        }
    }

    /**
     * Construct LabeledDirectedGraph containing every vertex in \p edges.
     * Graph size is adjusted to the largest index in \p edges.
     *
     * @tparam Container Any template container that accepts
     * BaseGraph::LabeledEdge and that supports range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * accepted.
     *
     * @param edges Edges to add into the graph.
     */
    template <template <class...> class Container, class... Args>
    explicit LabeledUndirectedGraph<EdgeLabel>(
        const Container<LabeledEdge<EdgeLabel>, Args...> &edgeList)
        : LabeledUndirectedGraph(0) {

        VertexIndex maxIndex = 0;
        for (const LabeledEdge<EdgeLabel> &labeledEdge : edgeList) {
            maxIndex =
                std::max(std::get<0>(labeledEdge), std::get<1>(labeledEdge));
            if (maxIndex >= getSize())
                resize(maxIndex + 1);
            addEdge(std::get<0>(labeledEdge), std::get<1>(labeledEdge),
                    std::get<2>(labeledEdge));
        }
    }

    /// Construct LabeledUndirectedGraph containing each edge of the
    /// LabeledDirectedGraph
    explicit LabeledUndirectedGraph<EdgeLabel>(const Directed &directedGraph)
        : LabeledUndirectedGraph(directedGraph.getSize()) {
        for (VertexIndex i : directedGraph)
            for (VertexIndex j : directedGraph.getOutEdgesOf(i))
                addEdge(i, j, directedGraph.getEdgeLabelOf(i, j));
    }

    using Directed::getEdgeNumber;
    using Directed::getSize;
    using Directed::resize;

    /**
     * Return if graph instance and \p other have the same size, edges and/or
     * edge labels.
     * @param other Graph to compare to.
     * @return If graph instance is equal to \p other.
     */
    bool operator==(const LabeledUndirectedGraph<EdgeLabel> &other) const {
        return Directed::operator==(other);
    }
    /// Return `not *this==other`.
    bool operator!=(const LabeledUndirectedGraph<EdgeLabel> &other) const {
        return !(this->operator==(other));
    }

    void addEdge(VertexIndex vertex1, VertexIndex vertex2, bool force = false) {
        addEdge(vertex1, vertex2, EdgeLabel(), force);
    }
    /**
     * Add labeled edge between vertex \p vertex1 and \p vertex2.
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * EdgeLabeledUndirectedGraph::removeDuplicateEdges.
     *
     * @param vertex1, vertex2 Index of the vertices to be connected.
     * @param label Label of the edge created.
     * @param force If \c false, the edge is not added if it already exists.
     *              If \c true, the edge is added without checking its
     *              existence (quicker).
     */
    void addEdge(VertexIndex vertex1, VertexIndex vertex2,
                 const EdgeLabel &label, bool force = false);
    /// Return if a directed edge connects \p vertex1 to \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2) const {
        auto edge = orderedEdge(vertex1, vertex2);
        return Directed::hasEdge(edge.first, edge.second);
    }

    /// Return if a directed edge of label \p label connects \p vertex1
    /// to \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2,
                 const EdgeLabel &label) const {
        return hasEdge(vertex1, vertex2) &&
               (getEdgeLabelOf(vertex1, vertex2, false) == label);
    }

    /// Remove labeled edge (including duplicates) between \p vertex1 and
    /// \p vertex2. Edge label is deleted.
    void removeEdge(VertexIndex vertex1, VertexIndex vertex2);
    using Directed::getOutEdgesOf;
    const Successors &getNeighboursOf(VertexIndex vertex) const {
        return getOutEdgesOf(vertex);
    }

    /**
     * Return label of edge connecting \p vertex1 and \p vertex2.
     * @param vertex1, vertex2 Index of the vertices of the edge.
     * @param throwIfInexistent If `true`, the method throws
     *            `std::invalid_argument` if the edge doesn't exist. If
     *            `false`, the method returns an EdgeLabeled built
     *            with its default constructor for inexistent edges.
     * @return Label of edge.
     */
    EdgeLabel getEdgeLabelOf(VertexIndex vertex1, VertexIndex vertex2,
                             bool throwIfInexistent = true) const {
        auto edge = orderedEdge(vertex1, vertex2);
        return Directed::getEdgeLabelOf(edge.first, edge.second,
                                        throwIfInexistent);
    }
    /**
     * Change the label of edge connecting \p vertex1 and \p vertex2.
     * @param vertex1, vertex2 Index of the vertices of the edge.
     * @param label New label value for the edge.
     * @param force If `true`, the method will not check if the edge exists.
     * This may create a label to an inexistent edge. If `false`, the method
     * throws `std::invalid_argument` if the edge doesn't exist.
     */
    void setEdgeLabel(VertexIndex vertex1, VertexIndex vertex2,
                      const EdgeLabel &label, bool force = false) {
        auto edge = orderedEdge(vertex1, vertex2);
        return Directed::setEdgeLabel(edge.first, edge.second, label, force);
    }

    void removeDuplicateEdges();
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
    }

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
    std::vector<size_t> getDegrees(bool countSelfLoopsTwice = true) const {
        std::vector<size_t> degrees(getSize());
        for (VertexIndex i : *this)
            degrees[i] = getDegreeOf(i, countSelfLoopsTwice);
        return degrees;
    }

    AdjacencyMatrix getAdjacencyMatrix(bool countSelfLoopsTwice = true) const;

    /// Construct _DirectedGraph containing each reciprocal edge of
    /// _UndirectedGraph instance.
    Directed getDirectedGraph() const;

    void removeVertexFromEdgeList(VertexIndex vertex);
    using Directed::assertVertexInRange;
    using Directed::begin;
    using Directed::clearEdges;
    using Directed::end;

    /// Output graph's size and edges in text to a given `std::stream` object.
    friend std::ostream &
    operator<<(std::ostream &stream,
               const LabeledUndirectedGraph<EdgeLabel> &graph) {
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
        const LabeledUndirectedGraph<EdgeLabel> &graph;
        Edges(const LabeledUndirectedGraph<EdgeLabel> &graph) : graph(graph) {}

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

        static VertexIndex
        getEndVertex(const LabeledUndirectedGraph<EdgeLabel> &graph) {
            auto vertexNumber = graph.getSize();
            if (vertexNumber == 0)
                return 0;
            return vertexNumber - 1;
        }
    };
    Edges edges() const { return Edges(*this); }

  protected:
    static Edge orderedEdge(VertexIndex i, VertexIndex j) {
        return i < j ? Edge{i, j} : Edge{j, i};
    }
    void setLabel(VertexIndex i, VertexIndex j, const EdgeLabel &label) {
        Directed::_setLabel(orderedEdge(i, j), label);
    }
};

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
                                  getEdgeLabelOf(edge.first, edge.second),
                                  true);
    return directedGraph;
}

template <typename EdgeLabel>
size_t
LabeledUndirectedGraph<EdgeLabel>::getDegreeOf(VertexIndex vertex,
                                               bool countSelfLoopsTwice) const {
    assertVertexInRange(vertex);

    if (!countSelfLoopsTwice)
        return Directed::adjacencyList[vertex].size();

    size_t degree = 0;
    for (auto neighbour : getNeighboursOf(vertex))
        degree += neighbour == vertex ? 2 : 1;
    return degree;
}

template <typename EdgeLabel>
AdjacencyMatrix LabeledUndirectedGraph<EdgeLabel>::getAdjacencyMatrix(
    bool countSelfLoopsTwice) const {
    const size_t &_size = getSize();
    AdjacencyMatrix adjacencyMatrix(_size, std::vector<size_t>(_size, 0));

    for (auto i : *this)
        for (auto j : getOutEdgesOf(i))
            adjacencyMatrix[i][j] += i == j && countSelfLoopsTwice ? 2 : 1;

    return adjacencyMatrix;
}

} // namespace BaseGraph

#endif
