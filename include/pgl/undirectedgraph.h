#ifndef PGL_UNDIRECTED_GRAPH_BASE_H
#define PGL_UNDIRECTED_GRAPH_BASE_H

#include "pgl/directedgraph.h"


namespace PGL{

class UndirectedGraph: protected DirectedGraph{
    public:
        explicit UndirectedGraph(size_t graphSize): DirectedGraph(graphSize) {}
        explicit UndirectedGraph(const DirectedGraph&);
        void resize(size_t size) { DirectedGraph::resize(size); }
        size_t getSize() const { return DirectedGraph::getSize(); }
        size_t getEdgeNumber() const { return edgeNumber; }

        DirectedGraph getDirectedGraph() const;

        bool operator==(const UndirectedGraph& other) const;
        bool operator!=(const UndirectedGraph& other) const { return !(this->operator==(other)); }

        void addEdgeIdx(size_t vertex1, size_t vertex2, bool force=false);
        void addEdgeIdx(const std::pair<size_t, size_t>& edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        bool isEdgeIdx(size_t vertex1, size_t vertex2) const;
        bool isEdgeIdx(const std::pair<size_t, size_t>& edge) const { return isEdgeIdx(edge.first, edge.second); }
        void removeEdgeIdx(size_t vertex1, size_t vertex2);
        void removeEdgeIdx(const std::pair<size_t, size_t>& edge) { removeEdgeIdx(edge.first, edge.second); }
        void removeVertexFromEdgeListIdx(size_t vertex);
        void removeMultiedges();
        void removeSelfLoops() { DirectedGraph::removeSelfLoops(); }
        void clear() { DirectedGraph::clear(); }

        template <typename Iterator>
        UndirectedGraph getSubgraph(Iterator begin, Iterator end) const { return getSubgraph(std::unordered_set<size_t>(begin, end)); };
        UndirectedGraph getSubgraph(const std::unordered_set<size_t>& vertices) const;
        template <typename Iterator>
        std::pair<UndirectedGraph, std::unordered_map<size_t, size_t>> getSubgraphWithRemap(Iterator begin, Iterator end) const {
            return getSubgraphWithRemap(std::unordered_set<size_t>(begin, end)); };
        std::pair<UndirectedGraph, std::unordered_map<size_t, size_t>> getSubgraphWithRemap(const std::unordered_set<size_t>& vertices) const;


        const std::list<size_t>& getNeighboursOfIdx(size_t vertex) const { return DirectedGraph::getOutEdgesOfIdx(vertex); }
        const std::list<size_t>& getOutEdgesOfIdx(size_t vertex) const { return getNeighboursOfIdx(vertex); }
        std::vector<std::vector<size_t> > getAdjacencyMatrix() const { return DirectedGraph::getAdjacencyMatrix(); }

        size_t getDegreeIdx(size_t vertex) const { return DirectedGraph::getOutDegreeIdx(vertex); }
        std::vector<size_t> getDegrees() const;

        friend std::ostream& operator <<(std::ostream &stream, const UndirectedGraph& graph) {
            stream << "Undirected graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (size_t i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << neighbour << ", ";
                stream << "\n";
            }
            return stream;
        }

        iterator begin() const {return iterator(0);}
        iterator end() const {return iterator(size);}
};

} // namespace PGL

#endif
