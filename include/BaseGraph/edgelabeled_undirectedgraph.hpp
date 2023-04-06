#ifndef BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H
#define BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H

#include <set>
#include <unordered_map>

#include "BaseGraph/boost_hash.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirectedgraph.h"

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
class EdgeLabeledUndirectedGraph : public UndirectedGraph {

  public:
    /// Construct EdgeLabeledUndirectedGraph with \p size vertices.
    /// @param size Number of vertices.
    explicit EdgeLabeledUndirectedGraph<EdgeLabel>(size_t size = 0)
        : UndirectedGraph(size), totalEdgeNumber(0) {}

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
    template <template <class...> class Container, class... Args>
    explicit EdgeLabeledUndirectedGraph<EdgeLabel>(
        const Container<LabeledEdge<EdgeLabel>, Args...> &edgeList)
        : EdgeLabeledUndirectedGraph(0) {

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

    /// Return sum of edge labels. Only defined if \p EdgeLabel is an
    /// integer type.
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_integral<U>::value, long long int>::type
    getTotalEdgeNumber() const {
        return totalEdgeNumber;
    }

    /**
     * Return if graph instance and \p other have the same size, edges and/or
     * edge labels.
     * @param other Graph to compare to.
     * @return If graph instance is equal to \p other.
     */
    bool operator==(const EdgeLabeledUndirectedGraph<EdgeLabel> &other) const {
        return UndirectedGraph::operator==(other) &&
               edgeLabels == other.edgeLabels;
    }
    /**
     * Return if graph instance and \p other have different sizes, edges
     * and/or edge labels.
     * @param other Graph to compare to.
     * @return If graph instance is different from \p other.
     */
    bool operator!=(const EdgeLabeledUndirectedGraph<EdgeLabel> &other) const {
        return !(this->operator==(other));
    }

    void addEdge(VertexIndex vertex1, VertexIndex vertex2,
                         bool force = false) {
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
    using DirectedGraph::hasEdge;
    /// Return if an edge of label \p label connects \p vertex1 and \p vertex2.
    bool hasEdge(VertexIndex vertex1, VertexIndex vertex2,
                 const EdgeLabel &label) const {
        return UndirectedGraph::hasEdge(vertex1, vertex2) &&
               (getEdgeLabelOf(vertex1, vertex2, false) == label);
    }

    /// Remove labeled edge (including duplicates) between \p vertex1 and
    /// \p vertex2. Edge label is deleted.
    void removeEdge(VertexIndex vertex1, VertexIndex vertex2) {
        _removeEdge(vertex1, vertex2);
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
                             bool throwIfInexistent = true) const;
    /**
     * Change the label of edge connecting \p vertex1 and \p vertex2.
     * @param vertex1, vertex2 Index of the vertices of the edge.
     * @param label New label value for the edge.
     * @param force If `true`, the method will not check if the edge exists.
     * This may create a label to an inexistent edge. If `false`, the method
     * throws `std::invalid_argument` if the edge doesn't exist.
     */
    void setEdgeLabel(VertexIndex vertex1, VertexIndex vertex2,
                      const EdgeLabel &label, bool force = false);

    void removeDuplicateEdges();
    void removeSelfLoops();
    void removeVertexFromEdgeList(VertexIndex vertex);
    void clearEdges() {
        UndirectedGraph::clearEdges();
        totalEdgeNumber = 0;
    }

  protected:
    long long int totalEdgeNumber; // Used only when EdgeLabel is integer
    std::unordered_map<Edge, EdgeLabel, hashEdge> edgeLabels;

    static Edge orderedEdge(VertexIndex i, VertexIndex j) {
        return i < j ? Edge{i, j} : Edge{j, i};
    }
    void setLabel(VertexIndex i, VertexIndex j, const EdgeLabel &label) {
        edgeLabels[orderedEdge(i, j)] = label;
    }

  private:
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<std::is_integral<U>::value>::type
    _removeEdge(VertexIndex vertex1, VertexIndex vertex2);
    template <typename... Dummy, typename U = EdgeLabel>
    typename std::enable_if<!std::is_integral<U>::value>::type
    _removeEdge(VertexIndex vertex1, VertexIndex vertex2);

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

template <typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::addEdge(VertexIndex vertex1,
                                                    VertexIndex vertex2,
                                                    const EdgeLabel &label,
                                                    bool force) {
    if (force || !hasEdge(vertex1, vertex2)) {
        UndirectedGraph::addEdge(vertex1, vertex2, true);
        setLabel(vertex1, vertex2, label);
        addToTotalEdgeNumber(label);
    }
}

template <typename EdgeLabel>
template <typename... Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
EdgeLabeledUndirectedGraph<EdgeLabel>::_removeEdge(VertexIndex vertex1,
                                                   VertexIndex vertex2) {
    static_assert(sizeof...(Dummy) == 0,
                  "Do not specify template arguments to call _removeEdge");

    size_t neighbourNumber = getOutEdgesOf(vertex1).size();
    UndirectedGraph::removeEdge(vertex1, vertex2);
    totalEdgeNumber -= getEdgeLabelOf(vertex1, vertex2, false) *
                       (neighbourNumber - getOutEdgesOf(vertex1).size());
    edgeLabels.erase(orderedEdge(vertex1, vertex2));
}

template <typename EdgeLabel>
template <typename... Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
EdgeLabeledUndirectedGraph<EdgeLabel>::_removeEdge(VertexIndex vertex1,
                                                   VertexIndex vertex2) {
    static_assert(sizeof...(Dummy) == 0,
                  "Do not specify template arguments to call _removeEdge");
    UndirectedGraph::removeEdge(vertex1, vertex2);
    edgeLabels.erase(orderedEdge(vertex1, vertex2));
}

template <typename EdgeLabel>
EdgeLabel EdgeLabeledUndirectedGraph<EdgeLabel>::getEdgeLabelOf(
    VertexIndex vertex1, VertexIndex vertex2, bool throwIfInexistent) const {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    Edge edge = orderedEdge(vertex1, vertex2);
    if (edgeLabels.count(edge) == 0) {
        if (throwIfInexistent)
            throw std::invalid_argument("Edge label does not exist.");
        return EdgeLabel();
    }
    return edgeLabels.at(edge);
}

template <typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::setEdgeLabel(VertexIndex vertex1,
                                                         VertexIndex vertex2,
                                                         const EdgeLabel &label,
                                                         bool force) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    if (!force && !hasEdge(vertex1, vertex2))
        throw std::invalid_argument("Cannot set label of inexistent edge.");

    auto &edgeLabel = edgeLabels[orderedEdge(vertex1, vertex2)];
    subtractFromTotalEdgeNumber(edgeLabel);
    edgeLabel = label;
    addToTotalEdgeNumber(label);
}

template <typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::removeDuplicateEdges() {
    std::set<VertexIndex> seenVertices;
    Successors::iterator j;

    for (VertexIndex i : *this) {
        j = adjacencyList[i].begin();

        while (j != adjacencyList[i].end()) {
            if (!seenVertices.count(*j)) {
                seenVertices.insert(*j);
                j++;
            } else {
                if (i <= *j) {
                    subtractFromTotalEdgeNumber(getEdgeLabelOf(i, *j, false));
                    edgeNumber--;
                }
                adjacencyList[i].erase(j++);
            }
        }
        seenVertices.clear();
    }
}

template <typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::removeSelfLoops() {
    for (VertexIndex &i : *this)
        removeEdge(i, i);
}

template <typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::removeVertexFromEdgeList(
    VertexIndex vertex) {
    assertVertexInRange(vertex);

    Successors::iterator j;
    for (VertexIndex i : *this) {
        j = adjacencyList[i].begin();
        while (j != adjacencyList[i].end())
            if (i == vertex || *j == vertex) {
                if (i <= *j) {
                    subtractFromTotalEdgeNumber(getEdgeLabelOf(i, *j, false));
                    edgeNumber--;
                }
                adjacencyList[i].erase(j++);
            } else {
                j++;
            }
    }
}

} // namespace BaseGraph

#endif
