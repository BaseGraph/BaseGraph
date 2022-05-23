#ifndef BASE_GRAPH_EDGE_LABELED_DIRECTED_GRAPH_H
#define BASE_GRAPH_EDGE_LABELED_DIRECTED_GRAPH_H

#include <algorithm>
#include <fstream>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "BaseGraph/boost_hash.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/directedgraph.h"


namespace BaseGraph{


template<typename EdgeLabel>
class EdgeLabeledDirectedGraph: public DirectedGraph {
    public:
        explicit EdgeLabeledDirectedGraph<EdgeLabel>(size_t size=0): DirectedGraph(size), totalEdgeNumber(0) {}

        template<template<class ...> class Container, class ...Args>
        explicit EdgeLabeledDirectedGraph<EdgeLabel>(const Container<LabeledEdge<EdgeLabel>>& edgeList):
            EdgeLabeledDirectedGraph(0)
        {
            VertexIndex maxIndex=0;
            for (const LabeledEdge<EdgeLabel>& labeledEdge: edgeList) {
                maxIndex = std::max(std::get<0>(labeledEdge), std::get<1>(labeledEdge));
                if (maxIndex >= getSize())
                    resize(maxIndex+1);
                addEdgeIdx(std::get<0>(labeledEdge), std::get<1>(labeledEdge), std::get<2>(labeledEdge));
            }
        }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value, long long int>::type
            getTotalEdgeNumber() const { return totalEdgeNumber; }

        bool operator==(const EdgeLabeledDirectedGraph<EdgeLabel>& other) const {
            return DirectedGraph::operator==(other) && edgeLabels == other.edgeLabels;
        }
        bool operator!=(const EdgeLabeledDirectedGraph<EdgeLabel>& other) const {
            return !(this->operator==(other));
        }

        using DirectedGraph::addEdgeIdx;
        void addEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force=false);
        void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false) {
            addEdgeIdx(vertex1, vertex2, label, force);
            addEdgeIdx(vertex2, vertex1, label, force);
        }
        using DirectedGraph::isEdgeIdx;
        bool isEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label) const {
            return isEdgeIdx(source, destination)
                && (getEdgeLabelOfIdx(source, destination) == label);
        }

        virtual void removeEdgeIdx(VertexIndex source, VertexIndex destination) override {
            _removeEdgeIdx(source, destination);
        }
        EdgeLabel getEdgeLabelOfIdx(VertexIndex source, VertexIndex destination, bool throwIfInexistent=false) const;
        void setEdgeLabelIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force=false);

        virtual void removeDuplicateEdges() override;
        virtual void removeSelfLoops() override;

        virtual void removeVertexFromEdgeListIdx(VertexIndex vertex) override;
        virtual void clearEdges() override;

        friend std::ostream& operator <<(std::ostream& stream, const EdgeLabeledDirectedGraph<EdgeLabel>& graph) {
            stream << "Directed graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex vertex: graph) {
                stream << vertex << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(vertex))
                    stream << "(" << neighbour << ", " << graph.getEdgeLabelOfIdx(vertex, neighbour) << ")";
                stream << "\n";
            }
            return stream;
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
    if (force || !isEdgeIdx(source, destination)) {
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

    if (!force && !isEdgeIdx(source, destination))
        throw std::invalid_argument("Cannot set label of inexistent edge.");

    subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(source, destination));
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
    totalEdgeNumber -= getEdgeLabelOfIdx(source, destination)*(neighbourNumber - getOutEdgesOfIdx(source).size());
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
                subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(i, *j));
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
        subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(vertex, *j));
        successors.erase(j++);
        edgeNumber--;
    }

    for (VertexIndex i=0; i<size; ++i)
        removeEdgeIdx(i, vertex);
}

template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::clearEdges() {
    DirectedGraph::clearEdges();
    totalEdgeNumber = 0;
}

} // namespace BaseGraph

#endif
