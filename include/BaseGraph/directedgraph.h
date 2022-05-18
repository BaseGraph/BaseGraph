#ifndef BASE_GRAPH_DIRECTED_GRAPH_H
#define BASE_GRAPH_DIRECTED_GRAPH_H

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include "BaseGraph/types.h"


namespace BaseGraph{


class DirectedGraph{

    public:
        explicit DirectedGraph(size_t size=0): size(0), edgeNumber(0) {resize(size);}

        template<template<class ...> class Container, class ...Args>
        explicit DirectedGraph(const Container<Edge>& edgeList): DirectedGraph(0) {
            VertexIndex maxIndex=0;
            for (const Edge& edge: edgeList) {
                maxIndex = std::max(edge.first, edge.second);
                if (maxIndex >= getSize())
                    resize(maxIndex+1);
                addEdgeIdx(edge);
            }
        }

        void resize(size_t size);
        size_t getSize() const { return size; }
        size_t getEdgeNumber() const { return edgeNumber; }

        bool operator==(const DirectedGraph& other) const;
        bool operator!=(const DirectedGraph& other) const { return !(this->operator==(other)); }

        void addEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false);
        void addEdgeIdx(const Edge& edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false) { addEdgeIdx(vertex1, vertex2, force); addEdgeIdx(vertex2, vertex1, force); }
        void addReciprocalEdgeIdx(const Edge& edge, bool force=false) {addReciprocalEdgeIdx(edge, force);}
        bool isEdgeIdx(VertexIndex source, VertexIndex destination) const;
        bool isEdgeIdx(const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }
        void removeEdgeIdx(VertexIndex source, VertexIndex destination);
        void removeEdgeIdx(const Edge& edge) {removeEdgeIdx(edge.first, edge.second);}
        void removeMultiedges();
        void removeSelfLoops();
        void removeVertexFromEdgeListIdx(VertexIndex vertex);
        void clearEdges();

        template <typename Iterator>
        DirectedGraph getSubgraphOfIdx(Iterator begin, Iterator end) const { return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        DirectedGraph getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        template <typename Iterator>
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;

        const Successors& getOutEdgesOfIdx(VertexIndex vertex) const { assertVertexInRange(vertex); return adjacencyList[vertex]; }
        AdjacencyLists getInEdges() const;
        AdjacencyMatrix getAdjacencyMatrix() const;

        size_t getInDegreeOfIdx(VertexIndex vertex) const;
        std::vector<size_t> getInDegrees() const;
        size_t getOutDegreeOfIdx(VertexIndex vertex) const;
        std::vector<size_t> getOutDegrees() const;

        DirectedGraph getReversedGraph() const;

        friend std::ostream& operator <<(std::ostream &stream, const DirectedGraph& graph) {
            stream << "Directed graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << neighbour << ", ";
                stream << "\n";
            }
            return stream;
        }

        struct iterator {
            VertexIndex position;
            iterator(VertexIndex position) : position(position) {}
            bool operator ==(iterator rhs) {return position == rhs.position;}
            bool operator!=(iterator rhs) {return position != rhs.position;}
            VertexIndex& operator*() {return position;}
            iterator operator++() {++position; return *this;}
            iterator operator++(int) {iterator tmp=iterator(position); operator++(); return tmp;}
        };

        iterator begin() const {return iterator(0);}
        iterator end() const {return iterator(size);}

    protected:
        AdjacencyLists adjacencyList;
        size_t size;
        size_t edgeNumber;

        void assertVertexInRange(VertexIndex vertex) const{
            if (vertex >= size)
                throw std::out_of_range("Vertex index (" + std::to_string(vertex) +
                        ") greater than the graph's size("+ std::to_string(size) +").");
        }
};

} // namespace BaseGraph

#endif
