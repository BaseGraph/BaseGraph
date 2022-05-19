#ifndef BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H


#include "BaseGraph/edgelabeled_undirectedgraph.hpp"
#include "BaseGraph/types.h"


namespace BaseGraph {

class UndirectedMultigraph: public EdgeLabeledUndirectedGraph<EdgeMultiplicity> {

    public:
        using BaseClass = EdgeLabeledUndirectedGraph<EdgeMultiplicity>;
        using LabeledSuccessors = std::list<std::pair<VertexIndex, EdgeMultiplicity>>;

        using BaseClass::BaseClass;

        void addEdgeIdx        (VertexIndex vertex1, VertexIndex vertex2, bool force=false) { addMultiedgeIdx(vertex1, vertex2, 1, force); }
        void addEdgeIdx        (Edge edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addMultiedgeIdx   (VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity, bool force=false);
        void addMultiedgeIdx   (Edge edge, EdgeMultiplicity,  EdgeMultiplicity multiplicity, bool force=false) { addMultiedgeIdx( edge.first, edge.second, multiplicity, force); }

        void removeEdgeIdx     (VertexIndex vertex1, VertexIndex vertex2) override { removeMultiedgeIdx(vertex1, vertex2, 1); }
        void removeEdgeIdx     (const Edge& edge) override { removeEdgeIdx(edge.first, edge.second); }
        void removeMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity);
        void removeMultiedgeIdx(Edge edge, EdgeMultiplicity multiplicity) { removeMultiedgeIdx(edge.first, edge.second, multiplicity); }

        EdgeMultiplicity getEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2) const;
        EdgeMultiplicity getEdgeMultiplicityIdx(Edge edge) const { return getEdgeMultiplicityIdx(edge.first, edge.second); }
        void setEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity);
        void setEdgeMultiplicityIdx(Edge edge, EdgeMultiplicity multiplicity) { setEdgeMultiplicityIdx(edge.first, edge.second, multiplicity); }

        AdjacencyMatrix getAdjacencyMatrix() const override;
        size_t getDegreeOfIdx(VertexIndex vertex, bool dummyFlag=true) const override;
        std::vector<size_t> getDegrees(bool dummyFlag=true) const override;
};



} // namespace BaseGraph

#endif
