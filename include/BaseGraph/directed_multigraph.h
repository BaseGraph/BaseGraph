#ifndef BASE_GRAPH_DIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_DIRECTED_MULTIGRAPH_H

#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/types.h"


namespace BaseGraph {

class DirectedMultigraph: public EdgeLabeledDirectedGraph<EdgeMultiplicity> {
    using BaseClass = EdgeLabeledDirectedGraph<EdgeMultiplicity>;

    public:
        using BaseClass::BaseClass;

        void addEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false) override {
            addMultiedgeIdx(source, destination, 1, force);
        }
        void addReciprocalEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false) override {
            addEdgeIdx(source, destination, force);
            addEdgeIdx(destination, source, force);
        }
        void addMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity, bool force=false);
        void addReciprocalMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity, bool force=false);

        void removeEdgeIdx(VertexIndex source, VertexIndex destination) override {
            removeMultiedgeIdx(source, destination, 1);
        }
        void removeMultiedgeIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity);

        EdgeMultiplicity getEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination) const;
        void setEdgeMultiplicityIdx(VertexIndex source, VertexIndex destination, EdgeMultiplicity);

        AdjacencyMatrix getAdjacencyMatrix() const override;
        size_t getOutDegreeOfIdx(VertexIndex vertex) const override;
        size_t getInDegreeOfIdx(VertexIndex vertex) const override;
        std::vector<size_t> getOutDegrees() const override;
        std::vector<size_t> getInDegrees() const override;
};



} // namespace BaseGraph

#endif
