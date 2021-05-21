#ifndef PGL_UNDIRECTED_GRAPH_BASE_H
#define PGL_UNDIRECTED_GRAPH_BASE_H

#include "pgl/directedgraph.h"


namespace PGL{

class UndirectedGraph: public DirectedGraph{
    public:
        explicit UndirectedGraph(size_t graphSize=10): DirectedGraph(graphSize) {}

        bool operator==(const UndirectedGraph& other) const;
        bool operator!=(const UndirectedGraph& other) const { return !(this->operator==(other)); }

        void addEdgeIdx(size_t source, size_t destination, bool force=false);
        void addEdgeIdx(const std::pair<size_t, size_t>& edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addReciprocalEdgeIdx(size_t vertex1, size_t vertex2, bool force=false) { addEdgeIdx(vertex1, vertex2, force); }
        void addReciprocalEdgeIdx(const std::pair<size_t, size_t>& edge, bool force=false) { addEdgeIdx(edge, force); }
        void removeEdgeIdx(size_t source, size_t destination);
        void removeEdgeIdx(const std::pair<size_t, size_t>& edge) { removeEdgeIdx(edge.first, edge.second); }
        void removeVertexFromEdgeListIdx(size_t vertex);
        void removeMultiedges();

        const std::list<size_t>& getNeighboursOfIdx(size_t vertex) const { return getOutEdgesOfIdx(vertex); }
        size_t getDegreeIdx(size_t vertex) const { return DirectedGraph::getOutDegreeIdx(vertex); }
        std::vector<size_t> getDegrees() const;
};

} // namespace PGL

#endif
