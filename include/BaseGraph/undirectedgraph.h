#ifndef BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H
#define BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H

#include "BaseGraph/directedgraph.h"


namespace BaseGraph {

class UndirectedGraph: protected DirectedGraph {
    public:
        explicit UndirectedGraph(size_t size=0) : DirectedGraph(size) {}
        explicit UndirectedGraph(const DirectedGraph&);

        template<template<class ...> class Container, class ...Args>
        explicit UndirectedGraph(const Container<Edge>& edgeList): UndirectedGraph(0) {
            VertexIndex maxIndex=0;
            for (const Edge& edge: edgeList) {
                maxIndex = std::max(edge.first, edge.second);
                if (maxIndex >= getSize())
                    resize(maxIndex+1);
                addEdgeIdx(edge.first, edge.second);
            }
        }

        DirectedGraph getDirectedGraph() const;

        using DirectedGraph::resize;
        using DirectedGraph::getSize;
        using DirectedGraph::getEdgeNumber;

        bool operator==(const UndirectedGraph& other) const { return DirectedGraph::operator==(other); }
        bool operator!=(const UndirectedGraph& other) const { return DirectedGraph::operator!=(other); }

        virtual void addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false);
        virtual void addEdgeIdx(const Edge& edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        bool isEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) const;
        bool isEdgeIdx(const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }
        virtual void removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);
        virtual void removeEdgeIdx(const Edge& edge) { removeEdgeIdx(edge.first, edge.second); }
        virtual void removeVertexFromEdgeListIdx(VertexIndex vertex);
        virtual void removeDuplicateEdges();
        using DirectedGraph::removeSelfLoops;
        using DirectedGraph::clearEdges;

        template <typename Iterator>
        UndirectedGraph getSubgraphOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end));
        };
        UndirectedGraph getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        template <typename Iterator>
        std::pair<UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end));
        };
        std::pair<UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;


        using DirectedGraph::getOutEdgesOfIdx;
        const Successors& getNeighboursOfIdx(VertexIndex vertex) const { return getOutEdgesOfIdx(vertex); }

        virtual AdjacencyMatrix getAdjacencyMatrix() const;
        virtual size_t getDegreeOfIdx(VertexIndex vertex, bool withSelfLoops=true) const;
        virtual std::vector<size_t> getDegrees(bool withSelfLoops=true) const;

        friend std::ostream& operator <<(std::ostream &stream, const UndirectedGraph& graph) {
            stream << "Undirected graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << neighbour << ", ";
                stream << "\n";
            }
            return stream;
        }
        Edge getSmallestAdjacency(VertexIndex vertex1, VertexIndex vertex2) const {
            return getDegreeOfIdx(vertex1, false) < getDegreeOfIdx(vertex2, false)
                ? Edge{vertex1, vertex2} : Edge{vertex2, vertex1};
        }

        using DirectedGraph::iterator;
        using DirectedGraph::begin;
        using DirectedGraph::end;
};

} // namespace BaseGraph

#endif
