#ifndef BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H
#define BASE_GRAPH_UNDIRECTED_MULTIGRAPH_H


#include "BaseGraph/edgelabeled_undirectedgraph.hpp"
#include "BaseGraph/types.h"


namespace BaseGraph {

class UndirectedMultigraph: public EdgeLabeledUndirectedGraph<EdgeMultiplicity> {

    public:
        using BaseClass = EdgeLabeledUndirectedGraph<EdgeMultiplicity>;

        using BaseClass::BaseClass;

        void addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false) override {
            addMultiedgeIdx(vertex1, vertex2, 1, force);
        }
        void addMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity, bool force=false);

        void removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) override {
            removeMultiedgeIdx(vertex1, vertex2, 1);
        }
        void removeMultiedgeIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity);

        EdgeMultiplicity getEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2) const;
        void setEdgeMultiplicityIdx(VertexIndex vertex1, VertexIndex vertex2, EdgeMultiplicity);

        AdjacencyMatrix getAdjacencyMatrix() const override;
        size_t getDegreeOfIdx(VertexIndex vertex, bool countSelfLoopsTwice=true) const override;
        std::vector<size_t> getDegrees(bool countSelfLoopsTwice=true) const override;
};



} // namespace BaseGraph

#endif
