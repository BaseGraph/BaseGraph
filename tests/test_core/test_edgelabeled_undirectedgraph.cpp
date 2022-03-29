#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "fixtures.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


typedef ::testing::Types<std::string, int> TestTypes;
typedef ::testing::Types<int> TestTypes_integral;


TYPED_TEST_SUITE(testEdgeLabeledUndirectedGraph, TestTypes);
TYPED_TEST_SUITE(testEdgeLabeledUndirectedGraph_integral, TestTypes_integral);



TYPED_TEST(testEdgeLabeledUndirectedGraph, getOutEdgesOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.getOutEdgesOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutEdgesOfIdx(2), std::out_of_range);
}

// When force=false in addEdgeIdx, isEdgeIdx is called.
// Both methods depend on each other so one must be tested first arbitrarily.

TYPED_TEST(testEdgeLabeledUndirectedGraph, addEdgeIdx_validEdge_successorInAdjacency) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 0, this->labels[1]);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{2, this->labels[0]}, {1, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{0, this->labels[0]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, addEdgeIdx_selfLoop_successorInAdjacency) {
    this->graph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{1, this->labels[0]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, addEdgeIdx_multiedge_successorInAdjacencyOnce) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(2, 1, this->labels[0]);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{2, this->labels[0]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, addEdgeIdx_multiedgeForced_successorInAdjacencyTwice) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(2, 1, this->labels[0], true);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{2, this->labels[0]}, {2, this->labels[0]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, addEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.addEdgeIdx(0, 0, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdgeIdx(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx(2, 1, this->labels[0]), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, isEdgeIdx_existentEdge_ReturnTrue) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    for (auto edge_label: std::list<std::tuple<BaseGraph::VertexIndex, BaseGraph::VertexIndex, TypeParam>>
                            { {0, 2, this->labels[0]}, {0, 1, this->labels[1]} }) {
        BaseGraph::VertexIndex i(std::get<0>(edge_label)), j(std::get<1>(edge_label));
        auto label = std::get<2>(edge_label);

        // Check edges (i,j) and (j,i)
        for (auto v=0; v<2; v++) {
            EXPECT_TRUE(this->graph.isEdgeIdx(j, i));
            EXPECT_TRUE(this->graph.isEdgeIdx(j, i, label));
            EXPECT_TRUE(this->graph.isEdgeIdx({j, i}));
            EXPECT_TRUE(this->graph.isEdgeIdx({j, i}, label));
            std::swap(i, j);
        }
    }
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, isEdgeIdx_inexistentEdge_ReturnFalse) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_FALSE(this->graph.isEdgeIdx(2, 1));
    EXPECT_FALSE(this->graph.isEdgeIdx(1, 2));

    for (auto edge_label: std::list<std::tuple<BaseGraph::VertexIndex, BaseGraph::VertexIndex, TypeParam>>
                            { {1, 2, this->labels[0]} }) {
        BaseGraph::VertexIndex i(std::get<0>(edge_label)), j(std::get<1>(edge_label));
        auto label = std::get<2>(edge_label);

        // Check edges (i,j) and (j,i)
        for (auto v=0; v<2; v++) {
            EXPECT_FALSE(this->graph.isEdgeIdx(j, i));
            EXPECT_FALSE(this->graph.isEdgeIdx(j, i, label));
            EXPECT_FALSE(this->graph.isEdgeIdx({j, i}));
            EXPECT_FALSE(this->graph.isEdgeIdx({j, i}, label));
            std::swap(i, j);
        }
    }
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, isEdgeIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.isEdgeIdx(0, 0), std::out_of_range);
    EXPECT_THROW(graph.isEdgeIdx(0, 0, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.isEdgeIdx({0, 0}), std::out_of_range);
    EXPECT_THROW(graph.isEdgeIdx({0, 0}, this->labels[0]), std::out_of_range);
    graph.resize(2);
    for (auto edge: std::list<BaseGraph::Edge>{{1, 2}, {2, 1}}) {
        EXPECT_THROW(graph.isEdgeIdx(edge.first, edge.second), std::out_of_range);
        EXPECT_THROW(graph.isEdgeIdx(edge.first, edge.second, this->labels[0]), std::out_of_range);
        EXPECT_THROW(graph.isEdgeIdx(edge), std::out_of_range);
        EXPECT_THROW(graph.isEdgeIdx(edge, this->labels[0]), std::out_of_range);
    }
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, getEdgeLabelOf_existentEdge_correctLabel) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->graph.getEdgeLabelOf(0, 2), this->labels[0]);
    EXPECT_EQ(this->graph.getEdgeLabelOf(2, 0), this->labels[0]);
    EXPECT_EQ(this->graph.getEdgeLabelOf(0, 1), this->labels[1]);
    EXPECT_EQ(this->graph.getEdgeLabelOf(1, 0), this->labels[1]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, getEdgeLabelOf_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->graph.getEdgeLabelOf(0, 2), std::invalid_argument);
    EXPECT_THROW(this->graph.getEdgeLabelOf(2, 0), std::invalid_argument);
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->graph.getEdgeLabelOf(0, 2), std::invalid_argument);
    EXPECT_THROW(this->graph.getEdgeLabelOf(2, 0), std::invalid_argument);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, getEdgeLabelOf_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.getEdgeLabelOf(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getEdgeLabelOf(1, 2), std::out_of_range);
    EXPECT_THROW(graph.getEdgeLabelOf(2, 1), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, setEdgeLabelTo_existentEdge_labelChanged) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    this->graph.setEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{2, this->labels[0]}, {1, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{0, this->labels[0]}});

    this->graph.setEdgeLabelTo(0, 2, this->labels[1]);
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{2, this->labels[1]}, {1, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{0, this->labels[1]}});
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, setEdgeLabelTo_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->graph.setEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->graph.setEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, setEdgeLabelTo_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.setEdgeLabelTo(0, 0, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.setEdgeLabelTo(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.setEdgeLabelTo(2, 1, this->labels[0]), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, removeEdgeIdx_existentEdge_edgeDoesntExist) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.removeEdgeIdx(0, 2);


    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, removeEdgeIdx_existentSelfLoop_edgeDoesntExist) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.removeEdgeIdx(0, 0);

    EXPECT_TRUE (this->graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->graph.isEdgeIdx(0, 0));
    EXPECT_EQ   (this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, removeEdgeIdx_inexistentEdge_edgeDoesntExist) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (this->graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->graph.isEdgeIdx(0, 2));
    EXPECT_EQ   (this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, removeEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.removeEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdgeIdx(2, 1), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, removeMultiedges_noMultiedge_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}, {1, this->labels[2]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{0, this->labels[1]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, removeMultiedges_multiedge_keepOneEdge) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 1, this->labels[1], true);
    this->graph.addEdgeIdx(0, 1, this->labels[2], true);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}, {1, this->labels[2]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{0, this->labels[1]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, removeMultiedges_multiSelfLoop_keepOnlyOneSelfLoop) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(1, 1, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[0], true);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 1, this->labels[2], true);

    this->graph.removeMultiedges();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}, {1, this->labels[1]}, {2, this->labels[2]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{1, this->labels[2]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 3);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);

    this->graph.removeSelfLoops();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{0, this->labels[1]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 0, this->labels[2]);

    this->graph.removeSelfLoops();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{0, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{0, this->labels[1]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, removeVertexFromEdgeListIdx_vertexInEdges_vertexNotInEdges) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 0, this->labels[3], true);
    this->graph.addEdgeIdx(1, 3, this->labels[4]);

    this->graph.removeVertexFromEdgeListIdx(0);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{2, this->labels[2]}, {3, this->labels[4]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {{1, this->labels[2]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(3), {{1, this->labels[4]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, removeVertexFromEdgeListIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(2), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, clearEdges_anyGraph_graphHasNoEdge) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 0, this->labels[3]);

    this->graph.clearEdges();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(2), {});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 0);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, getSubgraphOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    // Outside subgraph
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(2, 1, this->labels[1]);
    // Inside subgraph
    this->graph.addEdgeIdx(2, 3, this->labels[2]);
    this->graph.addEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph = this->graph.getSubgraphOfIdx({0, 2, 3});

    EXPECT_FALSE(subgraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(2, 1));
    EXPECT_TRUE (subgraph.isEdgeIdx(2, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 0));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 3));
    EXPECT_EQ   (subgraph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, getSubgraphOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(2);

    EXPECT_THROW(graph.getSubgraphOfIdx({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, getSubgraphWithRemapOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    // Outside subgraph
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(2, 1, this->labels[1]);
    // Inside subgraph
    this->graph.addEdgeIdx(2, 3, this->labels[2]);
    this->graph.addEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph_remap = this->graph.getSubgraphWithRemapOfIdx({0, 2, 3});
    auto& subgraph = subgraph_remap.first;
    auto& remap = subgraph_remap.second;

    EXPECT_EQ  (subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[3]));
    EXPECT_EQ  (subgraph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, getSubgraphWithRemapOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(2);

    EXPECT_THROW(graph.getSubgraphWithRemapOfIdx({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(2, 1, this->labels[3], true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {{2, 1, 0, 0},
                                                          {1, 0, 2, 0},
                                                          {0, 2, 0, 0},
                                                          {0, 0, 0, 0}};
    EXPECT_EQ(this->graph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, getDegrees_anyGraph_returnCorrectDegrees) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(0, 1, this->labels[2], true);

    EXPECT_EQ(this->graph.getDegrees(), std::vector<size_t>({4, 2, 0, 0}) );
    EXPECT_EQ(this->graph.getDegreeOfIdx(0), 4);
    EXPECT_EQ(this->graph.getDegreeOfIdx(1), 2);
    EXPECT_EQ(this->graph.getDegreeOfIdx(2), 0);
    EXPECT_EQ(this->graph.getDegreeOfIdx(3), 0);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, getDegreeOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.getDegreeOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getDegreeOfIdx(2), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, iterator_anyGraph_returnEachVertex) {
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(auto it=this->graph.begin(); it!=this->graph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for(auto it=this->graph.begin(); it!=this->graph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, rangedBasedFor_anyGraph_returnEachVertex) {
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::VertexIndex& vertex: this->graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph, equalityOperator_twoEmptyGraphs_returnTrue) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(2);
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph2(2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, equalityOperator_differentSize_returnFalse) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph2(2);

    EXPECT_FALSE(this->graph == graph2);
    EXPECT_FALSE(graph2 == this->graph);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, equalityOperator_sameEdgesAndSize_returnTrue) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, equalityOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, equalityOperator_differentLabels_returnFalse) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, equalityOperator_missingEdge_returnFalse) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph, equalityOperator_differentEdges_returnFalse) {
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);
    graph2.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}


// Test integral types only

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, addEdgeIdx_validEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, addEdgeIdx_selfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, addEdgeIdx_multiedge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, addEdgeIdx_multiedgeForced_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 2, this->labels[0], true);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, setEdgeLabelTo_existentEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    this->graph.setEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeEdgeIdx_existentEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeEdgeIdx_existentSelfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.removeEdgeIdx(0, 0);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeEdgeIdx_inexistentEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeMultiedges_noMultiedge_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeMultiedges_multiedge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 1, this->labels[0], true);
    this->graph.addEdgeIdx(0, 1, this->labels[0], true);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeMultiedges_multiSelfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(1, 1, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[1], true);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 1, this->labels[1], true);

    this->graph.removeMultiedges();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeSelfLoops_noSelfLoop_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);

    this->graph.removeSelfLoops();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeSelfLoops_existentSelfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 0, this->labels[2]);

    this->graph.removeSelfLoops();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, removeVertexFromEdgeListIdx_vertexInEdges_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 0, this->labels[3]);
    this->graph.addEdgeIdx(1, 0, this->labels[3], true);
    this->graph.addEdgeIdx(1, 3, this->labels[4]);

    this->graph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[2]+this->labels[4]);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, clearEdges_anyGraph_graphHasNoEdge) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 0, this->labels[3]);

    this->graph.clearEdges();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), 0);
}


TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, getSubgraphOfIdx_validVertexSubset_correctTotalEdgeNumber) {
    // Outside subgraph
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(2, 1, this->labels[1]);
    // Inside subgraph
    this->graph.addEdgeIdx(2, 3, this->labels[2]);
    this->graph.addEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph = this->graph.getSubgraphOfIdx({0, 2, 3});

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[4]);
}

TYPED_TEST(testEdgeLabeledUndirectedGraph_integral, getSubgraphWithRemapOfIdx_validVertexSubset_correctTotalEdgeNumber) {
    // Outside subgraph
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(2, 1, this->labels[1]);
    // Inside subgraph
    this->graph.addEdgeIdx(2, 3, this->labels[2]);
    this->graph.addEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph_remap = this->graph.getSubgraphWithRemapOfIdx({0, 2, 3});
    auto& subgraph = subgraph_remap.first;

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[4]);
}
