#ifndef BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H


#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


namespace BaseGraph {

class UndirectedMultigraph: public EdgeLabeledUndirectedGraph<EdgeMultiplicity> {
    std::vector<LabeledSuccessors>& adjacencyList = BaseClass::EdgeLabeledDirectedGraph<EdgeMultiplicity>::adjacencyList;
    long long int& totalEdgeNumber                = BaseClass::EdgeLabeledDirectedGraph<EdgeMultiplicity>::totalEdgeNumber;
    size_t& distinctEdgeNumber                    = BaseClass::EdgeLabeledDirectedGraph<EdgeMultiplicity>::distinctEdgeNumber;

    public:
        using BaseClass = EdgeLabeledUndirectedGraph<EdgeMultiplicity>;
        using LabeledSuccessors=  std::list<std::pair<VertexIndex, EdgeMultiplicity>>;

        UndirectedMultigraph(size_t size=0): BaseClass(size) {}

        void addEdgeIdx        (VertexIndex vertex1, VertexIndex vertex2, bool force=false) { addMultiedgeIdx(vertex1, vertex2, 1, force); }
        void addEdgeIdx        (Edge edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addMultiedgeIdx   (VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity, bool force=false);
        void addMultiedgeIdx   (Edge edge, EdgeMultiplicity,  EdgeMultiplicity multiplicity, bool force=false) { addMultiedgeIdx( edge.first, edge.second, multiplicity, force); }

        void removeEdgeIdx     (VertexIndex vertex1, VertexIndex vertex2) { removeMultiedgeIdx(vertex1, vertex2, 1); }
        void removeEdgeIdx     (Edge edge) { removeEdgeIdx(edge.first, edge.second); }
        void removeMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity);
        void removeMultiedgeIdx(Edge edge, EdgeMultiplicity multiplicity) { removeMultiedgeIdx(edge.first, edge.second, multiplicity); }

        const EdgeMultiplicity& getEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2) const { return getEdgeLabelOf(vertex1, vertex2); }
        const EdgeMultiplicity& getEdgeMultiplicityIdx(Edge edge) const { return getEdgeLabelOf(edge); }
        void setEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity);
        void setEdgeMultiplicityIdx(Edge edge, EdgeMultiplicity multiplicity) { setEdgeMultiplicityIdx(edge.first, edge.second, multiplicity); }
};



} // namespace BaseGraph

#endif
