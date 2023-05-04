#ifndef BASE_GRAPH_DIRECTED_GRAPH_HPP
#define BASE_GRAPH_DIRECTED_GRAPH_HPP

#include <iostream>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>

#include "BaseGraph/boost_hash.hpp"
#include "BaseGraph/types.h"

namespace BaseGraph {

/**
 * Directed graph with edge labels, self-loops and without multiedges. When no
 * \p EdgeLabel is specified, it acts as an unlabeled graph.
 *
 * Vertices are identified an integer index between 0 and \c size -1. Vertices
 * can be added using @ref resize. Vertices cannot be removed because it
 * requires reindexing. However, a vertex can be effectively removed by erasing
 * all of its edges with @ref removeVertexFromEdgeList.
 *
 * @tparam EdgeLabel Container of edge information. Requires a default
 * constructor.
 */
template <typename EdgeLabel> class LabeledDirectedGraph {
  protected:
    AdjacencyLists adjacencyList;
    size_t size = 0;
    size_t edgeNumber = 0;

    std::unordered_map<Edge, EdgeLabel, hashEdge> edgeLabels;

  public:
    /// Constructs an empty graph with \p _size vertices.
    explicit LabeledDirectedGraph<EdgeLabel>(size_t _size = 0) {
        resize(_size);
    }

    /**
     * Constructs a graph containing each edge in \p edgeSequence. The graph
     * size is adjusted to the largest index in \p edgeSequence. This method is
     * available only for a unlabeled graph (@ref BaseGraph::DirectedGraph).
     *
     * @tparam Container Any container of @ref BaseGraph::Edge that supports
     * range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * usable.
     *
     * For example:
     * \code{.cpp}
     * std::list<BaseGraph::Edge> edges = {{0, 2}, {0, 1}, {0, 0}, {5, 10}};
     * BaseGraph::DirectedGraph graph(edges);
     * \endcode
     */
    template <template <class...> class Container, class... Args,
              typename U = EdgeLabel>
    explicit LabeledDirectedGraph<EdgeLabel>(
        const Container<Edge, Args...> &edgeSequence,
        typename std::enable_if<std::is_same<U, NoLabel>::value,
                                long long int>::type * = 0)
        : LabeledDirectedGraph<EdgeLabel>(0) {
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
     * BaseGraph::LabeledDirectedGraph graph(labeledEdges);
     * \endcode
     */
    template <template <class...> class Container, class... Args>
    explicit LabeledDirectedGraph<EdgeLabel>(
        const Container<LabeledEdge<EdgeLabel>, Args...> &edgeSequence)
        : LabeledDirectedGraph(0) {

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

    /// Returns the number of vertices.
    size_t getSize() const { return size; }

    /// Sets the number of vertices to \p newSize.
    /// @param newSize Number of vertices. Must be larger than the current number
    /// of vertices.
    void resize(size_t newSize);

    /// Returns the number of edges.
    size_t getEdgeNumber() const { return edgeNumber; }

    /// Returns if graph instance and \p other have the same size, edges and
    /// edge labels.
    bool operator==(const LabeledDirectedGraph<EdgeLabel> &other) const;
    /// Returns `not` @ref operator==.
    bool operator!=(const LabeledDirectedGraph<EdgeLabel> &other) const {
        return !(this->operator==(other));
    }

    /**
     * Adds labeled directed edge from vertex \p source to \p destination.
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with @ref
     * removeDuplicateEdges.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param label Label of the edge created.
     * @param force If \c false, the edge is not added if it already exists.
     *              If \c true, the edge is added without checking its
     *              existence (quicker).
     */
    void addEdge(VertexIndex source, VertexIndex destination,
                 const EdgeLabel &label, bool force = false);
    /**
     * Adds edge from vertex \p source to \p destination with the default label
     * constructor. This is the suggested method to add edges in a @ref
     * BaseGraph::DirectedGraph. See the other @ref
     * addEdge(VertexIndex,VertexIndex,const EdgeLabel&, bool) "overload".
     */
    void addEdge(VertexIndex source, VertexIndex destination,
                 bool force = false) {
        addEdge(source, destination, EdgeLabel(), force);
    }

    /// Calls @ref addEdge for both edge orientations.
    void addReciprocalEdge(VertexIndex vertex1, VertexIndex vertex2,
                           const EdgeLabel &label, bool force = false) {
        addEdge(vertex1, vertex2, label, force);
        addEdge(vertex2, vertex1, label, force);
    }
    /// Calls @ref addReciprocalEdge using the label `EdgeLabel()`.
    void addReciprocalEdge(VertexIndex vertex1, VertexIndex vertex2,
                           bool force = false) {
        addReciprocalEdge(vertex1, vertex2, EdgeLabel(), force);
    }
    /// Returns if a directed edge of any label connects \p source to \p
    /// destination.
    bool hasEdge(VertexIndex source, VertexIndex destination) const;

    /// Returns if a directed edge of label \p label connects \p source to \p
    /// destination.
    bool hasEdge(VertexIndex source, VertexIndex destination,
                 const EdgeLabel &label) const {
        return hasEdge(source, destination) &&
               (getEdgeLabel(source, destination, false) == label);
    }
    /// Returns vertices to which \p vertex is connected.
    const Successors &getOutNeighbours(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        return adjacencyList[vertex];
    }

    /// Removes directed edges (including duplicates) from \p source to \p
    /// destination.
    void removeEdge(VertexIndex source, VertexIndex destination);
    /**
     * Returns label of directed edge connecting \p source to \p destination.
     * @param source, destination Index of the source and destination vertices.
     * @param throwIfInexistent If `true`, the method throws
     * `std::invalid_argument` if the directed edge doesn't exist. If `false`,
     * the method returns `EdgeLabel()` when the edge isn't found.
     *
     * @return Label of the edge.
     */
    EdgeLabel getEdgeLabel(VertexIndex source, VertexIndex destination,
                           bool throwIfInexistent = true) const {
        return _getLabel({source, destination}, throwIfInexistent);
    }
    /**
     * Changes the label of directed edge connecting \p source to \p
     * destination.
     * @param source, destination Index of the source and destination vertices.
     * @param label New edge label.
     * @param force If `true`, the method will not check if the edge exists.
     * This may create a label for an inexistent edge. If `false`, the method
     * throws `std::invalid_argument` if the directed edge doesn't exist.
     */
    void setEdgeLabel(VertexIndex source, VertexIndex destination,
                      const EdgeLabel &label, bool force = false);

    /// Constructs a graph where each edge orientation is reversed.
    LabeledDirectedGraph<EdgeLabel> getReversedGraph() const {
        LabeledDirectedGraph<EdgeLabel> reversedGraph(size);
        for (auto edge : edges())
            reversedGraph.addEdge(edge.second, edge.first,
                                  getEdgeLabel(edge.first, edge.second));
        return reversedGraph;
    }

    /// Removes duplicate edges that have been created using the flag
    /// `force=true` in @ref addEdge.
    void removeDuplicateEdges();

    /// Removes each edge which connects a vertex to itself.
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
    }

    /// Removes all edges that connect \p vertex to another vertex. This is
    /// nearly equivalent to removing a vertex from the graph.
    void removeVertexFromEdgeList(VertexIndex vertex);

    /// Removes all the edges from the graph.
    void clearEdges() {
        for (VertexIndex i : *this)
            adjacencyList[i].clear();
        edgeNumber = 0;
    }

    /// Counts the number of in edges of \p vertex. @ref getInDegrees is more
    /// efficient when more than one in degree is needed.
    size_t getInDegree(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        size_t inDegree = 0;
        for (auto edge : edges())
            if (edge.second == vertex)
                ++inDegree;
        return inDegree;
    }

    /// Counts the number of in edges of each vertex.
    std::vector<size_t> getInDegrees() const {
        std::vector<size_t> inDegrees(size, 0);
        for (auto edge : edges())
            ++inDegrees[edge.second];
        return inDegrees;
    }

    /// Counts the number of edges coming from \p vertex.
    size_t getOutDegree(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        return adjacencyList[vertex].size();
    }

    /// Counts the number of out edges of each vertex.
    std::vector<size_t> getOutDegrees() const {
        std::vector<size_t> outDegrees(size, 0);
        for (auto i : *this)
            outDegrees[i] += getOutDegree(i);
        return outDegrees;
    }

    /// Constructs the adjacency matrix.
    AdjacencyMatrix getAdjacencyMatrix() const {
        AdjacencyMatrix adjacencyMatrix(size, std::vector<size_t>(size, 0));
        for (auto edge : edges())
            ++adjacencyMatrix[edge.first][edge.second];
        return adjacencyMatrix;
    }

    /// Outputs graph's size and edges in text to a given `std::stream` object.
    friend std::ostream &
    operator<<(std::ostream &stream,
               const LabeledDirectedGraph<EdgeLabel> &graph) {
        stream << "Directed graph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getOutNeighbours(i))
                stream << neighbour << ", ";
            stream << "\n";
        }
        return stream;
    }

    /// Returns VertexIterator of first vertex. Allows ranged-based loop on the
    /// graph's vertices.
    VertexIterator begin() const { return VertexIterator(0); }
    /// Returns VertexIterator of last vertex. Allows ranged-based loop on the
    /// graph's vertices.
    VertexIterator end() const { return VertexIterator(size); }

    /// Structure that iterates on the graph's edges.
    struct Edges {
        struct constEdgeIterator {
            VertexIndex vertex;
            const VertexIndex endVertex;
            Successors::const_iterator neighbour;
            const LabeledDirectedGraph<EdgeLabel> &graph;

            constEdgeIterator(const LabeledDirectedGraph<EdgeLabel> &graph,
                              VertexIndex vertex,
                              Successors::const_iterator neighbour)
                : vertex(vertex), neighbour(neighbour), graph(graph),
                  endVertex(getEndVertex(graph)) {}

            bool operator==(constEdgeIterator rhs) const {
                return vertex == rhs.vertex && neighbour == rhs.neighbour;
            }
            bool operator!=(constEdgeIterator rhs) const {
                return !(*this == rhs);
            }
            Edge operator*() { return {vertex, *neighbour}; }
            constEdgeIterator operator++() {
                ++neighbour;
                while (neighbour == graph.getOutNeighbours(vertex).end() &&
                       vertex != endVertex)
                    neighbour = graph.getOutNeighbours(++vertex).begin();

                return *this;
            }
            constEdgeIterator operator++(int) {
                constEdgeIterator tmp =
                    constEdgeIterator(graph, vertex, neighbour);
                operator++();
                return tmp;
            }
        };

        const LabeledDirectedGraph<EdgeLabel> &graph;
        Edges(const LabeledDirectedGraph<EdgeLabel> &graph) : graph(graph) {}

        constEdgeIterator begin() const {
            VertexIndex endVertex = getEndVertex(graph);

            VertexIndex vertexWithFirstEdge = 0;
            Successors::const_iterator neighbour =
                graph.getOutNeighbours(0).begin();

            while (neighbour ==
                       graph.getOutNeighbours(vertexWithFirstEdge).end() &&
                   vertexWithFirstEdge != endVertex)
                neighbour =
                    graph.getOutNeighbours(++vertexWithFirstEdge).begin();

            return constEdgeIterator(graph, vertexWithFirstEdge, neighbour);
        }
        constEdgeIterator end() const {
            VertexIndex endVertex = getEndVertex(graph);
            return constEdgeIterator(graph, endVertex,
                                     graph.getOutNeighbours(endVertex).end());
        }
        static VertexIndex
        getEndVertex(const LabeledDirectedGraph<EdgeLabel> &graph) {
            auto vertexNumber = graph.getSize();
            if (vertexNumber == 0)
                return 0;
            return vertexNumber - 1;
        }
    };
    /// Creates @ref LabeledDirectedGraph::Edges object that supports range-based for loop.
    Edges edges() const { return Edges(*this); }

    /// Throws `std::out_of_range` if \p vertex is not contained in the graph.
    void assertVertexInRange(VertexIndex vertex) const {
        if (vertex >= size)
            throw std::out_of_range("Vertex index (" + std::to_string(vertex) +
                                    ") greater than the graph's size(" +
                                    std::to_string(size) + ").");
    }

  protected:
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_same<U, NoLabel>::value>::type
    _setLabel(const Edge &edge, const EdgeLabel &label) {}
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<!std::is_same<U, NoLabel>::value>::type
    _setLabel(const Edge &edge, const EdgeLabel &label) {
        edgeLabels[edge] = label;
    }

    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_same<U, NoLabel>::value, EdgeLabel>::type
    _getLabel(const Edge &edge, bool throwIfInexistent) const {
        assertVertexInRange(edge.first);
        assertVertexInRange(edge.second);
        return NoLabel();
    }
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<!std::is_same<U, NoLabel>::value, EdgeLabel>::type
    _getLabel(const Edge &edge, bool throwIfInexistent) const {
        assertVertexInRange(edge.first);
        assertVertexInRange(edge.second);
        try {
            return edgeLabels.at(edge);
        } catch (std::out_of_range &err) {
            if (throwIfInexistent)
                throw std::invalid_argument("Edge label does not exist.");
            return EdgeLabel();
        }
    }
};

/// Unlabeled directed graph.
using DirectedGraph = LabeledDirectedGraph<NoLabel>;

template <typename EdgeLabel>
void LabeledDirectedGraph<EdgeLabel>::resize(size_t newSize) {
    if (newSize < size)
        throw std::invalid_argument("Graph's size cannot be reduced.");
    size = newSize;
    adjacencyList.resize(newSize, std::list<VertexIndex>());
}

template <typename EdgeLabel>
bool LabeledDirectedGraph<EdgeLabel>::operator==(
    const LabeledDirectedGraph<EdgeLabel> &other) const {

    bool isEqual = size == other.size && edgeNumber == other.edgeNumber &&
                   edgeLabels == other.edgeLabels;

    std::list<VertexIndex>::const_iterator it;
    for (VertexIndex i = 0; i < size && isEqual; ++i) {
        for (it = adjacencyList[i].begin();
             it != adjacencyList[i].end() && isEqual; ++it) {
            if (!other.hasEdge(i, *it))
                isEqual = false;
        }

        for (it = other.adjacencyList[i].begin();
             it != other.adjacencyList[i].end() && isEqual; ++it) {
            if (!hasEdge(i, *it))
                isEqual = false;
        }
    }
    return isEqual;
}

template <typename EdgeLabel>
void LabeledDirectedGraph<EdgeLabel>::addEdge(VertexIndex source,
                                              VertexIndex destination,
                                              const EdgeLabel &label,
                                              bool force) {
    if (force || !hasEdge(source, destination)) {
        adjacencyList[source].push_back(destination);
        ++edgeNumber;
        _setLabel({source, destination}, label);
    }
}

template <typename EdgeLabel>
bool LabeledDirectedGraph<EdgeLabel>::hasEdge(VertexIndex source,
                                              VertexIndex destination) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    const auto &outEdges = getOutNeighbours(source);
    return find(outEdges.begin(), outEdges.end(), destination) !=
           outEdges.end();
}

template <typename EdgeLabel>
void LabeledDirectedGraph<EdgeLabel>::setEdgeLabel(VertexIndex source,
                                                   VertexIndex destination,
                                                   const EdgeLabel &label,
                                                   bool force) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (!force && !hasEdge(source, destination))
        throw std::invalid_argument("Cannot set label of inexistent edge.");
    _setLabel({source, destination}, label);
}

template <typename EdgeLabel>
void LabeledDirectedGraph<EdgeLabel>::removeEdge(VertexIndex source,
                                                 VertexIndex destination) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    size_t sizeBefore = adjacencyList[source].size();
    adjacencyList[source].remove(destination);
    edgeNumber -= sizeBefore - adjacencyList[source].size();

    edgeLabels.erase({source, destination});
}

template <typename EdgeLabel>
void LabeledDirectedGraph<EdgeLabel>::removeDuplicateEdges() {

    for (VertexIndex i : *this) {
        std::set<VertexIndex> seenVertices;
        auto j = adjacencyList[i].begin();

        while (j != adjacencyList[i].end()) {
            if (!seenVertices.count(*j)) {
                seenVertices.insert(*j);
                ++j;
            } else {
                adjacencyList[i].erase(j++);
                edgeNumber--;
            }
        }
        seenVertices.clear();
    }
}

template <typename EdgeLabel>
void LabeledDirectedGraph<EdgeLabel>::removeVertexFromEdgeList(
    VertexIndex vertex) {
    assertVertexInRange(vertex);

    auto &successors = adjacencyList[vertex];
    auto j = successors.begin();
    while (j != successors.end()) {
        successors.erase(j++);
        edgeNumber--;
    }

    for (VertexIndex i = 0; i < size; ++i)
        removeEdge(i, vertex);
}

} // namespace BaseGraph

#endif
