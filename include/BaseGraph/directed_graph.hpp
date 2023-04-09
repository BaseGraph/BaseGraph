#ifndef BASE_GRAPH_DIRECTED_GRAPH_HPP
#define BASE_GRAPH_DIRECTED_GRAPH_HPP

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <set>

#include "BaseGraph/boost_hash.hpp"
#include "BaseGraph/types.h"

namespace BaseGraph {

/**
 * BaseGraph::DirectedGraph in which each edge has a label.
 *
 * Inherits from BaseGraph::DirectedGraph publicly. Since this allows the
 * creation of edges without labels, a missing edge label is built with its
 * default constructor.
 *
 * @tparam EdgeLabel Container of edge information. Requires a default
 * constructor.
 */
template <typename EdgeLabel> class LabeledDirectedGraph {
  protected:
    AdjacencyLists adjacencyList;
    size_t size = 0;
    size_t edgeNumber = 0;

    long long int totalEdgeNumber = 0; // Used only when EdgeLabel is integer
    std::unordered_map<Edge, EdgeLabel, hashEdge> edgeLabels;

  public:
    /// Construct LabeledDirectedGraph with \p size vertices.
    /// @param size Number of vertices.
    explicit LabeledDirectedGraph<EdgeLabel>(size_t _size = 0) {
        resize(_size);
    }

    /**
     * Construct _DirectedGraph containing every vertex in \p edges. Graph
     * size is adjusted to the largest index in \p edges.
     *
     * @tparam Container Any template container that accepts type
     * BaseGraph::Edge and that supports range-based loops. Most <a
     * href="https://en.cppreference.com/w/cpp/container">STL containers</a> are
     * accepted.
     *
     * @param edges Edges to add into the graph.
     */
    template <template <class...> class Container, class... Args,
              typename U = EdgeLabel>
    explicit LabeledDirectedGraph<EdgeLabel>(
        const Container<Edge, Args...> &edges,
        typename std::enable_if<std::is_same<U, NoLabel>::value,
                                long long int>::type * = 0)
        : LabeledDirectedGraph<EdgeLabel>(0) {
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
    explicit LabeledDirectedGraph<EdgeLabel>(
        const Container<LabeledEdge<EdgeLabel>, Args...> &edgeList)
        : LabeledDirectedGraph(0) {

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

    /// Return number of vertices.
    size_t getSize() const { return size; }

    /// Construct LabeledDirectedGraph with \p size vertices.
    /// @param size Number of vertices.
    void resize(size_t newSize);

    /// Return number of edges.
    size_t getEdgeNumber() const { return edgeNumber; }

    /**
     * Return if graph instance and \p other have the same size, edges and/or
     * edge labels.
     * @param other Graph to compare to.
     * @return If graph instance is equal to \p other.
     */
    bool operator==(const LabeledDirectedGraph<EdgeLabel> &other) const;
    /// Return `not *this==other`.
    bool operator!=(const LabeledDirectedGraph<EdgeLabel> &other) const {
        return !(this->operator==(other));
    }

    /// Return sum of edge labels. Only defined if \p EdgeLabel is an
    /// integer type.
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_integral<U>::value, long long int>::type
    getTotalEdgeNumber() const {
        static_assert(
            sizeof...(Dummy) == 0,
            "Do not specify template arguments to call getTotalEdgeNumber");
        return totalEdgeNumber;
    }

    void addEdge(VertexIndex source, VertexIndex destination,
                 bool force = false) {
        addEdge(source, destination, EdgeLabel(), force);
    }
    /**
     * Add labeled directed edge from vertex \p source to \p destination.
     * \warning
     * Use <tt>force=true</tt> with caution as it may create duplicate edges.
     * Since this class isn't designed to handle them, it might behave
     * unexpectedly in some algorithms. Remove duplicate edges with
     * LabeledDirectedGraph::removeDuplicateEdges.
     *
     * @param source, destination Index of the source and destination vertices.
     * @param label Label of the edge created.
     * @param force If \c false, the edge is not added if it already exists.
     *              If \c true, the edge is added without checking its
     *              existence (quicker).
     */
    void addEdge(VertexIndex source, VertexIndex destination,
                 const EdgeLabel &label, bool force = false);
    void addReciprocalEdge(VertexIndex source, VertexIndex destination,
                           bool force = false) {
        addReciprocalEdge(source, destination, EdgeLabel(), force);
    }
    /**
     * Add labeled reciprocal edges. Calls DirectedGraph::addEdge for both
     * edge directions.
     * @param vertex1, vertex2 Vertices of reciprocal edges.
     * @param label Label of the edges created.
     * @param force See `force` of addEdge.
     */
    void addReciprocalEdge(VertexIndex vertex1, VertexIndex vertex2,
                           const EdgeLabel &label, bool force = false) {
        addEdge(vertex1, vertex2, label, force);
        addEdge(vertex2, vertex1, label, force);
    }
    /// Return if a directed edge connects \p source to \p destination.
    bool hasEdge(VertexIndex source, VertexIndex destination) const;

    /// Return if a directed edge of label \p label connects \p source
    /// to \p destination.
    bool hasEdge(VertexIndex source, VertexIndex destination,
                 const EdgeLabel &label) const {
        return hasEdge(source, destination) &&
               (getEdgeLabelOf(source, destination, false) == label);
    }
    /// Return vertices to which \p vertex is connected.
    const Successors &getOutEdgesOf(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        return adjacencyList[vertex];
    }

    /// Return in edges of each vertex.
    AdjacencyLists getInEdges() const {
        AdjacencyLists inEdges(size);
        for (auto edge : edges())
            inEdges[edge.second].push_back(edge.first);
        return inEdges;
    }

    /// Remove labeled directed edge (including duplicates) from \p source to
    /// \p destination. Edge label is deleted.
    void removeEdge(VertexIndex source, VertexIndex destination) {
        _removeEdge(source, destination);
    }
    /**
     * Return label of directed edge connecting \p source to \p destination.
     * @param source, destination Index of the source and destination vertices.
     * @param throwIfInexistent If `true`, the method throws
     *            `std::invalid_argument` if the directed edge doesn't exist.
     *            If `false`, the method returns an EdgeLabeled constructed
     *            with its default constructor.
     * @return Label of edge.
     */
    EdgeLabel getEdgeLabelOf(VertexIndex source, VertexIndex destination,
                             bool throwIfInexistent = true) const;
    /**
     * Change the label of directed edge connecting \p source to \p destination.
     * @param source, destination Index of the source and destination vertices.
     * @param label New label value for the edge.
     * @param force If `true`, the method will not check if the edge exists.
     * This may create a label to an inexistent edge. If `false`, the method
     * throws `std::invalid_argument` if the directed edge doesn't exist for
     * inexistent edges.
     */
    void setEdgeLabel(VertexIndex source, VertexIndex destination,
                      const EdgeLabel &label, bool force = false);

    /// Count the number of in edges of \p vertex. Use
    /// _DirectedGraph::getInDegrees if more than one in degree is needed.
    size_t getInDegreeOf(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        size_t inDegree = 0;
        for (auto edge : edges())
            if (edge.second == vertex)
                inDegree++;
        return inDegree;
    }

    /// Count the number of in edges of each vertex.
    std::vector<size_t> getInDegrees() const {
        std::vector<size_t> inDegrees(size, 0);
        for (auto edge : edges())
            inDegrees[edge.second]++;
        return inDegrees;
    }

    /// Count the number of out edges starting from \p vertex.
    size_t getOutDegreeOf(VertexIndex vertex) const {
        assertVertexInRange(vertex);
        return adjacencyList[vertex].size();
    }

    /// Count the number of out edges of each vertex.
    std::vector<size_t> getOutDegrees() const {
        std::vector<size_t> outDegrees(size, 0);
        for (VertexIndex i = 0; i < size; i++)
            outDegrees[i] += getOutDegreeOf(i);
        return outDegrees;
    }

    /// Construct the adjacency matrix.
    AdjacencyMatrix getAdjacencyMatrix() const {
        AdjacencyMatrix adjacencyMatrix(size, std::vector<size_t>(size, 0));
        for (auto edge : edges())
            adjacencyMatrix[edge.first][edge.second]++;
        return adjacencyMatrix;
    }

    /// Construct a _DirectedGraph where each directed edge is reversed.
    LabeledDirectedGraph<EdgeLabel> getReversedGraph() const {
        LabeledDirectedGraph<EdgeLabel> reversedGraph(size);
        for (auto edge : edges())
            reversedGraph.addEdge(edge.second, edge.first,
                                  getEdgeLabelOf(edge.first, edge.second));
        return reversedGraph;
    }

    void removeDuplicateEdges();
    void removeSelfLoops() {
        for (VertexIndex &i : *this)
            removeEdge(i, i);
    }

    void removeVertexFromEdgeList(VertexIndex vertex);
    void clearEdges() {
        for (VertexIndex i : *this)
            adjacencyList[i].clear();
        edgeNumber = 0;
        totalEdgeNumber = 0;
    }

    /// Output graph's size and edges in text to a given `std::stream` object.
    friend std::ostream &
    operator<<(std::ostream &stream,
               const LabeledDirectedGraph<EdgeLabel> &graph) {
        stream << "Directed graph of size: " << graph.getSize() << "\n"
               << "Neighbours of:\n";

        for (VertexIndex i : graph) {
            stream << i << ": ";
            for (auto &neighbour : graph.getOutEdgesOf(i))
                stream << neighbour << ", ";
            stream << "\n";
        }
        return stream;
    }

    /// Return VertexIterator of first vertex.
    VertexIterator begin() const { return VertexIterator(0); }
    /// Return VertexIterator of last vertex.
    VertexIterator end() const { return VertexIterator(size); }

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
                neighbour++;
                while (neighbour == graph.getOutEdgesOf(vertex).end() &&
                       vertex != endVertex)
                    neighbour = graph.getOutEdgesOf(++vertex).begin();

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
                graph.getOutEdgesOf(0).begin();

            while (neighbour ==
                       graph.getOutEdgesOf(vertexWithFirstEdge).end() &&
                   vertexWithFirstEdge != endVertex)
                neighbour = graph.getOutEdgesOf(++vertexWithFirstEdge).begin();

            return constEdgeIterator(graph, vertexWithFirstEdge, neighbour);
        }
        constEdgeIterator end() const {
            VertexIndex endVertex = getEndVertex(graph);
            return constEdgeIterator(graph, endVertex,
                                     graph.getOutEdgesOf(endVertex).end());
        }
        static VertexIndex
        getEndVertex(const LabeledDirectedGraph<EdgeLabel> &graph) {
            auto vertexNumber = graph.getSize();
            if (vertexNumber == 0)
                return 0;
            return vertexNumber - 1;
        }
    };
    /// Add support for range-based for looping on edges with `for (const Edge&
    /// edge: graph.edges)`.
    Edges edges() const { return Edges(*this); }

    void assertVertexInRange(VertexIndex vertex) const {
        if (vertex >= size)
            throw std::out_of_range("Vertex index (" + std::to_string(vertex) +
                                    ") greater than the graph's size(" +
                                    std::to_string(size) + ").");
    }

  private:
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_integral<U>::value>::type
    _removeEdge(VertexIndex source, VertexIndex destination);
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<!std::is_integral<U>::value>::type
    _removeEdge(VertexIndex source, VertexIndex destination);

    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_integral<U>::value>::type
    addToTotalEdgeNumber(EdgeLabel value) {
        totalEdgeNumber += value;
    }
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<!std::is_integral<U>::value>::type
    addToTotalEdgeNumber(EdgeLabel value) {}

    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_integral<U>::value>::type
    subtractFromTotalEdgeNumber(EdgeLabel value) {
        totalEdgeNumber -= value;
    }
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<!std::is_integral<U>::value>::type
    subtractFromTotalEdgeNumber(EdgeLabel value) {}
};

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
        edgeNumber++;
        edgeLabels[{source, destination}] = label;
        addToTotalEdgeNumber(label);
    }
}

template <typename EdgeLabel>
bool LabeledDirectedGraph<EdgeLabel>::hasEdge(VertexIndex source,
                                              VertexIndex destination) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    const auto &outEdges = getOutEdgesOf(source);
    return find(outEdges.begin(), outEdges.end(), destination) !=
           outEdges.end();
}

template <typename EdgeLabel>
EdgeLabel LabeledDirectedGraph<EdgeLabel>::getEdgeLabelOf(
    VertexIndex source, VertexIndex destination, bool throwIfInexistent) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    Edge edge = {source, destination};
    if (edgeLabels.count(edge) == 0) {
        if (throwIfInexistent)
            throw std::invalid_argument("Edge label does not exist.");
        return EdgeLabel();
    }
    return edgeLabels.at(edge);
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

    subtractFromTotalEdgeNumber(getEdgeLabelOf(source, destination, false));
    edgeLabels[{source, destination}] = label;
    addToTotalEdgeNumber(label);
}

template <typename EdgeLabel>
template <typename... Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
LabeledDirectedGraph<EdgeLabel>::_removeEdge(VertexIndex source,
                                             VertexIndex destination) {
    static_assert(sizeof...(Dummy) == 0,
                  "Do not specify template arguments to call _removeEdge");
    assertVertexInRange(source);
    assertVertexInRange(destination);

    size_t sizeBefore = adjacencyList[source].size();
    adjacencyList[source].remove(destination);
    size_t sizeAfter = sizeBefore - adjacencyList[source].size();

    edgeNumber -= sizeAfter;
    totalEdgeNumber -= getEdgeLabelOf(source, destination, false) * sizeAfter;
    edgeLabels.erase({source, destination});
}

template <typename EdgeLabel>
template <typename... Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
LabeledDirectedGraph<EdgeLabel>::_removeEdge(VertexIndex source,
                                             VertexIndex destination) {
    static_assert(sizeof...(Dummy) == 0,
                  "Do not specify template arguments to call _removeEdge");
    assertVertexInRange(source);
    assertVertexInRange(destination);

    size_t sizeBefore = adjacencyList[source].size();
    adjacencyList[source].remove(destination);
    edgeNumber -= sizeBefore - adjacencyList[source].size();

    edgeLabels.erase({source, destination});
}

template <typename EdgeLabel>
void LabeledDirectedGraph<EdgeLabel>::removeDuplicateEdges() {
    std::set<VertexIndex> seenVertices;
    Successors::iterator j;

    for (VertexIndex i : *this) {
        j = adjacencyList[i].begin();

        while (j != adjacencyList[i].end()) {
            if (!seenVertices.count(*j)) {
                seenVertices.insert(*j);
                j++;
            } else {
                subtractFromTotalEdgeNumber(getEdgeLabelOf(i, *j, false));
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
        subtractFromTotalEdgeNumber(getEdgeLabelOf(vertex, *j, false));
        successors.erase(j++);
        edgeNumber--;
    }

    for (VertexIndex i = 0; i < size; ++i)
        removeEdge(i, vertex);
}

} // namespace BaseGraph

#endif
