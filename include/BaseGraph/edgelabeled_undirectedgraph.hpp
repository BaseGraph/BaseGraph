#ifndef BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H
#define BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H

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
#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph{


template<typename EdgeLabel>
class EdgeLabeledUndirectedGraph: public UndirectedGraph {

    public:
        explicit EdgeLabeledUndirectedGraph<EdgeLabel>(size_t size=0): UndirectedGraph(size), totalEdgeNumber(0) {}

        template<template<class ...> class Container, class ...Args>
        explicit EdgeLabeledUndirectedGraph<EdgeLabel>(const Container<LabeledEdge<EdgeLabel>>& edgeList): EdgeLabeledUndirectedGraph(0) {
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

        bool operator==(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const {
            return UndirectedGraph::operator==(other) && edgeLabels == other.edgeLabels;
        }
        bool operator!=(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const {
            return !(this->operator==(other));
        }

        using UndirectedGraph::addEdgeIdx;
        void addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false);
        using UndirectedGraph::hasEdgeIdx;
        bool hasEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label) const {
            return UndirectedGraph::hasEdgeIdx(vertex1, vertex2)
                && (getEdgeLabelOfIdx(vertex1, vertex2) == label);
        }

        virtual void removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) override {
            _removeEdgeIdx(vertex1, vertex2);
        }
        EdgeLabel getEdgeLabelOfIdx(VertexIndex vertex1, VertexIndex vertex2, bool throwIfInexistent=false) const;
        void setEdgeLabelIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false);

        void removeDuplicateEdges() override;
        void removeSelfLoops() override;
        void removeVertexFromEdgeListIdx(VertexIndex vertex) override;
        void clearEdges() override {
            UndirectedGraph::clearEdges(); totalEdgeNumber = 0;
        }

        friend std::ostream& operator <<(std::ostream& stream, const EdgeLabeledUndirectedGraph<EdgeLabel>& graph) {
            stream << "Undirected graph of size: " << graph.getSize() << "\n"
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

        static Edge orderedEdge(VertexIndex i, VertexIndex j) {
            return i<j ? Edge{i, j} : Edge{j, i};
        }
        void setLabel(VertexIndex i, VertexIndex j, const EdgeLabel& label) {
            edgeLabels[orderedEdge(i, j)] = label;
        }


    private:

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            _removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            _removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);


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
void EdgeLabeledUndirectedGraph<EdgeLabel>::addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force) {
    if (force || !hasEdgeIdx(vertex1, vertex2)) {
        UndirectedGraph::addEdgeIdx(vertex1, vertex2, true);
        setLabel(vertex1, vertex2, label);
        addToTotalEdgeNumber(label);
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::_removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _removeEdgeIdx");

    size_t neighbourNumber = getOutEdgesOfIdx(vertex1).size();
    UndirectedGraph::removeEdgeIdx(vertex1, vertex2);
    totalEdgeNumber -= getEdgeLabelOfIdx(vertex1, vertex2)*(neighbourNumber - getOutEdgesOfIdx(vertex1).size());
    edgeLabels.erase(orderedEdge(vertex1, vertex2));
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::_removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _removeEdgeIdx");
    UndirectedGraph::removeEdgeIdx(vertex1, vertex2);
    edgeLabels.erase(orderedEdge(vertex1, vertex2));
}


template<typename EdgeLabel>
EdgeLabel EdgeLabeledUndirectedGraph<EdgeLabel>::getEdgeLabelOfIdx(VertexIndex vertex1, VertexIndex vertex2, bool throwIfInexistent) const {
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


template<typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::setEdgeLabelIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force) {
    assertVertexInRange(vertex1);
    assertVertexInRange(vertex2);

    if (!force && !hasEdgeIdx(vertex1, vertex2))
        throw std::invalid_argument("Cannot set label of inexistent edge.");

    auto& edgeLabel = edgeLabels[orderedEdge(vertex1, vertex2)];
    subtractFromTotalEdgeNumber(edgeLabel);
    edgeLabel = label;
    addToTotalEdgeNumber(label);
}


template<typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::removeDuplicateEdges(){
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
                if (i <= *j) {
                    subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(i, *j));
                    edgeNumber--;
                }
                adjacencyList[i].erase(j++);
            }
        }
        seenVertices.clear();
    }
}

template<typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::removeSelfLoops() {
    for (VertexIndex& i: *this)
        removeEdgeIdx(i, i);
}


template<typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::removeVertexFromEdgeListIdx(VertexIndex vertex){
    assertVertexInRange(vertex);

    Successors::iterator j;
    for (VertexIndex i: *this) {
        j = adjacencyList[i].begin();
        while (j!=adjacencyList[i].end())
            if ( i==vertex || *j==vertex ) {
                if (i<=*j) {
                    subtractFromTotalEdgeNumber(getEdgeLabelOfIdx(i, *j));
                    edgeNumber--;
                }
                adjacencyList[i].erase(j++);
            }
            else {
                j++;
            }
    }
}


} // namespace BaseGraph

#endif
