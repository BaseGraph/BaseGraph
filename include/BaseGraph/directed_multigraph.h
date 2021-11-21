#ifndef BASE_GRAPH_DIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_DIRECTED_MULTIGRAPH_H


#include "BaseGraph/directedgraph.h"
#include "BaseGraph/edgelabeled_directedgraph.hpp"


namespace BaseGraph {

class DirectedMultigraph: public EdgeLabeledDirectedGraph<EdgeMultiplicity> {
    using BaseClass = EdgeLabeledDirectedGraph<EdgeMultiplicity>;

    public:
        using BaseClass::BaseClass;

        void addEdgeIdx               (VertexIndex source, VertexIndex destination, bool force=false) { addMultiedgeIdx(source, destination, 1, force); }
        void addEdgeIdx               (Edge edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addReciprocalEdgeIdx     (VertexIndex source, VertexIndex destination, bool force=false) { addMultiedgeIdx(source, destination, 1, force); }
        void addReciprocalEdgeIdx     (Edge edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addMultiedgeIdx          (VertexIndex source, VertexIndex destination, EdgeMultiplicity, bool force=false);
        void addMultiedgeIdx          (Edge edge, EdgeMultiplicity multiplicity, bool force=false) { addMultiedgeIdx( edge.first, edge.second, multiplicity, force); }
        void addReciprocalMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity, bool force=false);
        void addReciprocalMultiedgeIdx(Edge edge, EdgeMultiplicity multiplicity, bool force=false) { addMultiedgeIdx( edge.first, edge.second, multiplicity, force); }

        void removeEdgeIdx      (VertexIndex source, VertexIndex destination) override { removeMultiedgeIdx(source, destination, 1); }
        void removeEdgeIdx      (const Edge& edge) override { BaseClass::removeEdgeIdx(edge.first, edge.second); }
        void removeMultiedgeIdx (VertexIndex source, VertexIndex destination, EdgeMultiplicity);
        void removeMultiedgeIdx (Edge edge, EdgeMultiplicity multiplicity) { removeMultiedgeIdx(edge.first, edge.second, multiplicity); }

        EdgeMultiplicity getEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination) const;
        EdgeMultiplicity getEdgeMultiplicityIdx(Edge edge) const { return getEdgeMultiplicityIdx(edge.first, edge.second); }
        void setEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity);
        void setEdgeMultiplicityIdx(Edge edge, EdgeMultiplicity multiplicity) { setEdgeMultiplicityIdx(edge.first, edge.second, multiplicity); }

        size_t getOutDegreeOfIdx(VertexIndex vertex) const override;
        size_t getInDegreeOfIdx(VertexIndex vertex) const override;
        std::vector<size_t> getOutDegrees() const override;
        std::vector<size_t> getInDegrees() const override;
};



} // namespace BaseGraph

#endif
