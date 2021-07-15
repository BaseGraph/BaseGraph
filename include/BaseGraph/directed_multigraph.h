#ifndef BASE_GRAPH_DIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_DIRECTED_MULTIGRAPH_H


#include "BaseGraph/edgelabeled_directedgraph.hpp"


namespace BaseGraph {

class DirectedMultigraph: public EdgeLabeledDirectedGraph<EdgeMultiplicity> {
    using BaseClass = EdgeLabeledDirectedGraph<EdgeMultiplicity>;

    public:
        DirectedMultigraph(size_t size=0): BaseClass(size) {}


        void addEdgeIdx        (VertexIndex source, VertexIndex destination, bool force=false) { addMultiedgeIdx(source, destination, 1, force); }
        void addEdgeIdx        (Edge edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addMultiedgeIdx   (VertexIndex source, VertexIndex destination, EdgeMultiplicity, bool force=false);
        void addMultiedgeIdx   (Edge edge, EdgeMultiplicity,  EdgeMultiplicity multiplicity, bool force=false) { addMultiedgeIdx( edge.first, edge.second, multiplicity, force); }

        void removeEdgeIdx     (VertexIndex source, VertexIndex destination) { removeMultiedgeIdx(source, destination, 1); }
        void removeEdgeIdx     (Edge edge) { removeEdgeIdx(edge.first, edge.second); }
        void removeMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity);
        void removeMultiedgeIdx(Edge edge, EdgeMultiplicity multiplicity) { removeMultiedgeIdx(edge.first, edge.second, multiplicity); }

        EdgeMultiplicity getEdgeMultiplicity(VertexIndex source, VertexIndex destination) const { return getEdgeLabelOf(source, destination); }
        EdgeMultiplicity getEdgeMultiplicity(Edge edge) const { return getEdgeLabelOf(edge); }
        void setEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity);
        void setEdgeMultiplicityIdx(Edge edge, EdgeMultiplicity multiplicity) { setEdgeMultiplicityIdx(edge.first, edge.second, multiplicity); }
};



} // namespace BaseGraph

#endif
