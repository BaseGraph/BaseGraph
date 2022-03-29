#ifndef BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H
#define BASE_GRAPH_UNDIRECTED_GRAPH_BASE_H

#include "BaseGraph/directedgraph.h"


namespace BaseGraph{

class UndirectedGraph: protected DirectedGraph{
    public:
        explicit UndirectedGraph(size_t graphSize=0): DirectedGraph(graphSize) {}
        explicit UndirectedGraph(const DirectedGraph&);

        void resize(size_t size) { DirectedGraph::resize(size); }
        size_t getSize() const { return DirectedGraph::getSize(); }
        size_t getEdgeNumber() const { return edgeNumber; }

        DirectedGraph getDirectedGraph() const;

        bool operator==(const UndirectedGraph& other) const { return DirectedGraph::operator==(other); }
        bool operator!=(const UndirectedGraph& other) const { return DirectedGraph::operator!=(other); }

        void addEdgeIdx      (VertexIndex vertex1, VertexIndex vertex2, bool force=false);
        void addEdgeIdx      (const Edge& edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        bool isEdgeIdx       (VertexIndex vertex1, VertexIndex vertex2) const;
        bool isEdgeIdx       (const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }
        void removeEdgeIdx   (VertexIndex vertex1, VertexIndex vertex2);
        void removeEdgeIdx   (const Edge& edge) { removeEdgeIdx(edge.first, edge.second); }
        void removeVertexFromEdgeListIdx(VertexIndex vertex);
        void removeMultiedges();
        void removeSelfLoops () { DirectedGraph::removeSelfLoops(); }
        void clearEdges      () { DirectedGraph::clearEdges(); }

        template <typename Iterator>
        UndirectedGraph getSubgraphOfIdx(Iterator begin, Iterator end) const { return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        UndirectedGraph getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        template <typename Iterator>
        std::pair<UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        std::pair<UndirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;


        const Successors& getNeighboursOfIdx(VertexIndex vertex) const { return DirectedGraph::getOutEdgesOfIdx(vertex); }
        const Successors& getOutEdgesOfIdx(VertexIndex vertex) const { return getNeighboursOfIdx(vertex); }
        AdjacencyMatrix   getAdjacencyMatrix() const;

        size_t getDegreeOfIdx(VertexIndex vertex, bool withSelfLoops=true) const;
        std::vector<size_t> getDegrees(bool withSelfLoops=true) const;

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
        struct iterator: DirectedGraph::iterator {
            iterator(const DirectedGraph::iterator& it): DirectedGraph::iterator(it) {}
        };

        iterator begin() const {return DirectedGraph::iterator(0);}
        iterator end() const {return DirectedGraph::iterator(size);}
};

} // namespace BaseGraph

#endif
