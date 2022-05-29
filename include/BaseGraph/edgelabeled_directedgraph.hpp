#ifndef BASE_GRAPH_EDGE_LABELED_DIRECTED_GRAPH_H
#define BASE_GRAPH_EDGE_LABELED_DIRECTED_GRAPH_H

#include <set>
#include <unordered_map>

#include "BaseGraph/boost_hash.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/directedgraph.h"


namespace BaseGraph{

/**
 * BaseGraph::DirectedGraph in which each edge has a label.
 *
 * Inherits from BaseGraph::DirectedGraph publicly. Since this allows the
 * creation of edges without labels, a missing edge label is built with its
 * default constructor.
 *
 * @tparam EdgeLabel Container of edge information. Requires a default constructor.
 */
template<typename EdgeLabel>
class EdgeLabeledDirectedGraph: public DirectedGraph {
    public:
        /// Construct EdgeLabeledDirectedGraph with \p size vertices.
        /// @param size Number of vertices.
        explicit EdgeLabeledDirectedGraph<EdgeLabel>(size_t size=0):
            DirectedGraph(size), totalEdgeNumber(0) {}

        /**
         * Construct EdgeLabeledDirectedGraph containing every vertex in \p edges.
         * Graph size is adjusted to the largest index in \p edges.
         *
         * @tparam Container Any template container that accepts BaseGraph::LabeledEdge
         *         and that supports range-based loops. Most
         *         <a href="https://en.cppreference.com/w/cpp/container">STL
         *         containers</a> are accepted.
         *
         * @param edges Edges to add into the graph.
         */
        template<template<class ...> class Container, class ...Args>
        explicit EdgeLabeledDirectedGraph<EdgeLabel>(const Container<LabeledEdge<EdgeLabel>>& edgeList):
            EdgeLabeledDirectedGraph(0) {

            VertexIndex maxIndex=0;
            for (const LabeledEdge<EdgeLabel>& labeledEdge: edgeList) {
                maxIndex = std::max(std::get<0>(labeledEdge), std::get<1>(labeledEdge));
                if (maxIndex >= getSize())
                    resize(maxIndex+1);
                addEdgeIdx(std::get<0>(labeledEdge), std::get<1>(labeledEdge), std::get<2>(labeledEdge));
            }
        }

        /// Return sum of edge labels. Only defined if \p EdgeLabel is an
        /// integer type.
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value, long long int>::type
            getTotalEdgeNumber() const {
                static_assert(sizeof...(Dummy)==0,
                        "Do not specify template arguments to call getTotalEdgeNumber");
                return totalEdgeNumber;
            }

        /**
         * Return if graph instance and \p other have the same size, edges and/or
         * edge labels.
         * @param other Graph to compare to.
         * @return If graph instance is equal to \p other.
         */
        bool operator==(const EdgeLabeledDirectedGraph<EdgeLabel>& other) const {
            return DirectedGraph::operator==(other) && edgeLabels == other.edgeLabels;
        }
        /**
         * Return if graph instance and \p other have different sizes, edges
         * and/or edge labels.
         * @param other Graph to compare to.
         * @return If graph instance is different from \p other.
         */
        bool operator!=(const EdgeLabeledDirectedGraph<EdgeLabel>& other) const {
            return !(this->operator==(other));
        }

        virtual void addEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false) override {
            addEdgeIdx(source, destination, EdgeLabel(), force);
        }
        /**
         * Add labeled directed edge from vertex \p source to \p destination.
         * \warning
         * Use <tt>force=true</tt> with caution as it may create duplicate edges.
         * Since this class isn't designed to handle them, it might behave
         * unexpectedly in some algorithms. Remove duplicate edges with
         * EdgeLabeledDirectedGraph::removeDuplicateEdges.
         *
         * @param source, destination Index of the source and destination vertices.
         * @param label Label of the edge created.
         * @param force If \c false, the edge is not added if it already exists.
         *              If \c true, the edge is added without checking its
         *              existence (quicker).
         */
        void addEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force=false);
        virtual void addReciprocalEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false) override {
            addReciprocalEdgeIdx(source, destination, EdgeLabel(), force);
        }
        /**
         * Add labeled reciprocal edges. Calls DirectedGraph::addEdgeIdx for both
         * edge directions.
         * @param vertex1, vertex2 Vertices of reciprocal edges.
         * @param label Label of the edges created.
         * @param force See `force` of addEdgeIdx.
         */
        void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false) {
            addEdgeIdx(vertex1, vertex2, label, force);
            addEdgeIdx(vertex2, vertex1, label, force);
        }
        using DirectedGraph::hasEdgeIdx;
        /// Return if a directed edge of label \p label connects \p source
        /// to \p destination.
        bool hasEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label) const {
            return hasEdgeIdx(source, destination)
                && (getEdgeLabelOfIdx(source, destination, false) == label);
        }
        /// Remove labeled directed edge (including duplicates) from \p source to
        /// \p destination. Edge label is deleted.
        virtual void removeEdgeIdx(VertexIndex source, VertexIndex destination) override {
            _removeEdgeIdx(source, destination);
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
        EdgeLabel getEdgeLabelOfIdx(VertexIndex source, VertexIndex destination, bool throwIfInexistent=true) const;
        /**
         * Change the label of directed edge connecting \p source to \p destination.
         * @param source, destination Index of the source and destination vertices.
         * @param label New label value for the edge.
         * @param force If `true`, the method will not check if the edge exists. This
         *            may create a label to an inexistent edge. If `false`, the
         *            method throws `std::invalid_argument` if the directed edge
         *            doesn't exist for inexistent edges.
         */
        void setEdgeLabelIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force=false);

        virtual void removeDuplicateEdges() override;
        virtual void removeSelfLoops() override;

        virtual void removeVertexFromEdgeListIdx(VertexIndex vertex) override;
        virtual void clearEdges() override {
            DirectedGraph::clearEdges(); totalEdgeNumber = 0;
        }

    protected:
        long long int totalEdgeNumber; // Used only when EdgeLabel is integer
        std::unordered_map<Edge, EdgeLabel, hashEdge> edgeLabels;

    private:
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            _removeEdgeIdx(VertexIndex source, VertexIndex destination);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            _removeEdgeIdx(VertexIndex source, VertexIndex destination);

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            addToTotalEdgeNumber(EdgeLabel value) {
                totalEdgeNumber += value;
            }
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            addToTotalEdgeNumber(EdgeLabel value) {}

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            subtractFromTotalEdgeNumber(EdgeLabel value) {
                totalEdgeNumber -= value;
            }
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            subtractFromTotalEdgeNumber(EdgeLabel value) {}
};


template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::addEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force) {
    if (force || !hasEdgeIdx(source, destination)) {
        DirectedGraph::addEdgeIdx(source, destination, true);
        edgeLabels[{source, destination}] = label;
        addToTotalEdgeNumber(label);
    }
}

template<typename EdgeLabel>
EdgeLabel EdgeLabeledDirectedGraph<EdgeLabel>::getEdgeLabelOfIdx(VertexIndex source, VertexIndex destination, bool throwIfInexistent) const {
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

template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::setEdgeLabelIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (!force && !hasEdgeIdx(source, destination))
        throw std::invalid_argument("Cannot set label of inexistent edge.");

    subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(source, destination, false));
    edgeLabels[{source, destination}] = label;
    addToTotalEdgeNumber(label);
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::_removeEdgeIdx(VertexIndex source, VertexIndex destination) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _removeEdgeIdx");

    size_t neighbourNumber = getOutEdgesOfIdx(source).size();
    DirectedGraph::removeEdgeIdx(source, destination);
    totalEdgeNumber -= getEdgeLabelOfIdx(source, destination, false)
                        *(neighbourNumber - getOutEdgesOfIdx(source).size());
    edgeLabels.erase({source, destination});
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::_removeEdgeIdx(VertexIndex source, VertexIndex destination) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _removeEdgeIdx");
    DirectedGraph::removeEdgeIdx(source, destination);
    edgeLabels.erase({source, destination});
}

template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::removeDuplicateEdges(){
    std::set<VertexIndex> seenVertices;
    Successors::iterator j;

    for (VertexIndex i: *this) {
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (!seenVertices.count(*j)) {
                seenVertices.insert(*j);
                j++;
            }
            else {
                subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(i, *j, false));
                adjacencyList[i].erase(j++);
                edgeNumber--;
            }
        }
        seenVertices.clear();
    }
}

template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::removeSelfLoops() {
    for (VertexIndex& i: *this)
        removeEdgeIdx(i, i);
}


template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::removeVertexFromEdgeListIdx(VertexIndex vertex){
    assertVertexInRange(vertex);

    auto& successors = adjacencyList[vertex];
    auto j = successors.begin();
    while (j != successors.end()) {
        subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(vertex, *j, false));
        successors.erase(j++);
        edgeNumber--;
    }

    for (VertexIndex i=0; i<size; ++i)
        removeEdgeIdx(i, vertex);
}

} // namespace BaseGraph

#endif
