#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "BaseGraph/types.h"
#include "fixtures.hpp"
#include "BaseGraph/edgelabeled_directedgraph.hpp"


typedef ::testing::Types<std::string, int> TestTypes;
typedef ::testing::Types<int> TestTypes_integral;


TYPED_TEST_SUITE(testEdgeLabeledDirectedGraph, TestTypes);
TYPED_TEST_SUITE(testEdgeLabeledDirectedGraph_integral, TestTypes_integral);


TYPED_TEST(testEdgeLabeledDirectedGraph, getOutEdgesOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.getOutEdgesOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutEdgesOfIdx(2), std::out_of_range);
}

// When force=false in addEdgeIdx, isEdgeIdx is called.
// Both methods depend on each other so one must be tested first arbitrarily.

TYPED_TEST(testEdgeLabeledDirectedGraph, addEdgeIdx_validEdge_successorInAdjacency) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{2, this->labels[0]}, {1, this->labels[1]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, addEdgeIdx_selfLoop_successorInAdjacency) {
    this->graph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{1, this->labels[0]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, addEdgeIdx_multiedge_successorInAdjacencyOnce) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{2, this->labels[0]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, addEdgeIdx_multiedgeForced_successorInAdjacencyTwice) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 2, this->labels[0], true);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{2, this->labels[0]}, {2, this->labels[0]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, addEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.addEdgeIdx(0, 0, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx({0, 0}, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdgeIdx(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx({1, 2}, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx(2, 1, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx({2, 1}, this->labels[0]), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, isEdgeIdx_existentEdge_ReturnTrue) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);


    for (auto edge_label: std::list<std::tuple<BaseGraph::VertexIndex, BaseGraph::VertexIndex, TypeParam>>
                            { {0, 2, this->labels[0]}, {0, 1, this->labels[1]} }) {
        BaseGraph::VertexIndex i(std::get<0>(edge_label)), j(std::get<1>(edge_label));
        auto label = std::get<2>(edge_label);
        EXPECT_TRUE(this->graph.isEdgeIdx(i, j));
        EXPECT_TRUE(this->graph.isEdgeIdx(i, j, label));
        EXPECT_TRUE(this->graph.isEdgeIdx({i, j}));
        EXPECT_TRUE(this->graph.isEdgeIdx({i, j}, label));
    }
}

TYPED_TEST(testEdgeLabeledDirectedGraph, isEdgeIdx_inexistentEdge_ReturnFalse) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    for (auto edge_label: std::list<std::tuple<BaseGraph::VertexIndex, BaseGraph::VertexIndex, TypeParam>>
                            { {0, 2, this->labels[0]}, {0, 1, this->labels[1]} }) {
        BaseGraph::VertexIndex i(std::get<0>(edge_label)), j(std::get<1>(edge_label));
        auto label = std::get<2>(edge_label);
        EXPECT_FALSE(this->graph.isEdgeIdx(j, i));
        EXPECT_FALSE(this->graph.isEdgeIdx(j, i, label));
        EXPECT_FALSE(this->graph.isEdgeIdx({j, i}));
        EXPECT_FALSE(this->graph.isEdgeIdx({j, i}, label));
    }
}

TYPED_TEST(testEdgeLabeledDirectedGraph, isEdgeIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

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


TYPED_TEST(testEdgeLabeledDirectedGraph, getEdgeLabelOf_existentEdge_correctLabel) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->graph.getEdgeLabelOf(0, 2), this->labels[0]);
    EXPECT_EQ(this->graph.getEdgeLabelOf(0, 1), this->labels[1]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, getEdgeLabelOf_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->graph.getEdgeLabelOf(0, 2), std::invalid_argument);
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->graph.getEdgeLabelOf(0, 2), std::invalid_argument);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, getEdgeLabelOf_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.getEdgeLabelOf(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getEdgeLabelOf(1, 2), std::out_of_range);
    EXPECT_THROW(graph.getEdgeLabelOf(2, 1), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, setEdgeLabelTo_existentEdge_labelChanged) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    this->graph.setEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{2, this->labels[0]}, {1, this->labels[0]}});
    this->graph.setEdgeLabelTo(0, 2, this->labels[1]);
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{2, this->labels[1]}, {1, this->labels[0]}});
}

TYPED_TEST(testEdgeLabeledDirectedGraph, setEdgeLabelTo_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->graph.setEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->graph.setEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, setEdgeLabelTo_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.setEdgeLabelTo(0, 0, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.setEdgeLabelTo(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.setEdgeLabelTo(2, 1, this->labels[0]), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, addReciprocalEdgeIdx_validEdge_edgeExistsInBothDirections) {
    this->graph.addReciprocalEdgeIdx(0, 1, this->labels[0]);

    EXPECT_TRUE(this->graph.isEdgeIdx(0, 1));
    EXPECT_TRUE(this->graph.isEdgeIdx(1, 0));
    EXPECT_EQ  (this->graph.getDistinctEdgeNumber(), 2);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, removeEdgeIdx_existentEdge_edgeDoesntExist) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (this->graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->graph.isEdgeIdx(0, 2));
    EXPECT_EQ   (this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, removeEdgeIdx_existentSelfLoop_edgeDoesntExist) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.removeEdgeIdx(0, 0);

    EXPECT_TRUE (this->graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->graph.isEdgeIdx(0, 0));
    EXPECT_EQ   (this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, removeEdgeIdx_inexistentEdge_edgeDoesntExist) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (this->graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->graph.isEdgeIdx(0, 2));
    EXPECT_EQ   (this->graph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, removeEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.removeEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdgeIdx(2, 1), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, removeMultiedges_noMultiedge_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{1, this->labels[2]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, removeMultiedges_multiedge_keepOneEdge) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 1, this->labels[0], true);
    this->graph.addEdgeIdx(0, 1, this->labels[0], true);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{1, this->labels[2]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, removeMultiedges_multiSelfLoop_keepOnlyOneSelfLoop) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(1, 1, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[1], true);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 1, this->labels[1], true);

    this->graph.removeMultiedges();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{1, this->labels[1]}, {2, this->labels[2]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 3);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);

    this->graph.removeSelfLoops();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 0, this->labels[2]);

    this->graph.removeSelfLoops();

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {{1, this->labels[0]}, {2, this->labels[1]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, removeVertexFromEdgeListIdx_vertexInEdges_vertexNotInEdges) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 0, this->labels[3]);
    this->graph.addEdgeIdx(1, 0, this->labels[3], true);
    this->graph.addEdgeIdx(1, 3, this->labels[4]);

    this->graph.removeVertexFromEdgeListIdx(0);

    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(0), {});
    EXPECT_NEIGHBOURS_EQ(this->graph.getOutEdgesOfIdx(1), {{2, this->labels[2]}, {3, this->labels[4]}});
    EXPECT_EQ(this->graph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, removeVertexFromEdgeListIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(2), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, clearEdges_anyGraph_graphHasNoEdge) {
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


TYPED_TEST(testEdgeLabeledDirectedGraph, getSubgraphOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    this->graph.addEdgeIdx          (0, 1, this->labels[0]);
    this->graph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->graph.addEdgeIdx          (2, 3, this->labels[2]);
    this->graph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph = this->graph.getSubgraphOfIdx({0, 2, 3});

    EXPECT_FALSE(subgraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(2, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(1, 2));
    EXPECT_TRUE (subgraph.isEdgeIdx(2, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(0, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 0));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 3));
    EXPECT_EQ   (subgraph.getDistinctEdgeNumber(), 4);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, getSubgraphOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(2);

    EXPECT_THROW(graph.getSubgraphOfIdx({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, getSubgraphWithRemapOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    this->graph.addEdgeIdx          (0, 1, this->labels[0]);
    this->graph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->graph.addEdgeIdx          (2, 3, this->labels[2]);
    this->graph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph_remap = this->graph.getSubgraphWithRemapOfIdx({0, 2, 3});
    auto& subgraph = subgraph_remap.first;
    auto& remap = subgraph_remap.second;

    EXPECT_EQ  (subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[0]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[3]));
    EXPECT_EQ  (subgraph.getDistinctEdgeNumber(), 4);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, getSubgraphWithRemapOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(2);

    EXPECT_THROW(graph.getSubgraphWithRemapOfIdx({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, getInEdges_anyGraph_returnAllInEdges) {
    this->graph.addEdgeIdx          (0, 0, this->labels[0]);
    this->graph.addEdgeIdx          (0, 1, this->labels[1]);
    this->graph.addReciprocalEdgeIdx(2, 1, this->labels[2]);
    this->graph.addEdgeIdx          (3, 2, this->labels[3]);
    this->graph.addEdgeIdx          (3, 1, this->labels[4]);

    EXPECT_NEIGHBOURS_EQ(this->graph.getInEdges(), {
                {{0, this->labels[0]}},
                {{0, this->labels[1]}, {2, this->labels[2]}, {3, this->labels[4]}},
                {{1, this->labels[2]}, {3, this->labels[3]}},
                {}});
}


TYPED_TEST(testEdgeLabeledDirectedGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    this->graph.addEdgeIdx          (0, 1, this->labels[0]);
    this->graph.addEdgeIdx          (0, 0, this->labels[1]);
    this->graph.addReciprocalEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx          (2, 1, this->labels[3], true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {{1, 1, 0, 0},
                                                          {0, 0, 1, 0},
                                                          {0, 2, 0, 0},
                                                          {0, 0, 0, 0}};
    EXPECT_EQ(this->graph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, getInDegreeOfIdx_anyGraph_returnCorrectDegrees) {
    this->graph.addEdgeIdx(1, 0, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 0, this->labels[2], true);
    this->graph.addEdgeIdx(0, 1, this->labels[3]);

    EXPECT_EQ(this->graph.getInDegrees(), std::vector<size_t>({3, 1, 0, 0}) );
    EXPECT_EQ(this->graph.getInDegreeOfIdx(0), 3);
    EXPECT_EQ(this->graph.getInDegreeOfIdx(1), 1);
    EXPECT_EQ(this->graph.getInDegreeOfIdx(2), 0);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, getInDegreeOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.getInDegreeOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getInDegreeOfIdx(2), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, getOutDegrees_anyGraph_returnCorrectDegrees) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(0, 1, this->labels[2], true);
    this->graph.addEdgeIdx(1, 0, this->labels[3]);

    EXPECT_EQ(this->graph.getOutDegrees(), std::vector<size_t>({3, 1, 0, 0}) );
    EXPECT_EQ(this->graph.getOutDegreeOfIdx(0), 3);
    EXPECT_EQ(this->graph.getOutDegreeOfIdx(1), 1);
    EXPECT_EQ(this->graph.getOutDegreeOfIdx(2), 0);
    EXPECT_EQ(this->graph.getOutDegreeOfIdx(3), 0);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, getOutDegreeOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(0);

    EXPECT_THROW(graph.getOutDegreeOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutDegreeOfIdx(2), std::out_of_range);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, getReversedGraph_anyGraph_onlyReverseEdgesExist) {
    this->graph.addEdgeIdx          (0, 1, this->labels[0]);
    this->graph.addEdgeIdx          (2, 0, this->labels[1]);
    this->graph.addReciprocalEdgeIdx(2, 3, this->labels[2]);

    auto reversedGraph = this->graph.getReversedGraph();

    EXPECT_FALSE(reversedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(reversedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(2, 3));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(3, 2));

    EXPECT_EQ(reversedGraph.getDistinctEdgeNumber(), 4);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, iterator_anyGraph_returnEachVertex) {
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


TYPED_TEST(testEdgeLabeledDirectedGraph, rangedBasedFor_anyGraph_returnEachVertex) {
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::VertexIndex& vertex: this->graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_twoEmptyGraphs_returnTrue) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(2);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_differentSize_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(2);

    EXPECT_FALSE(this->graph == graph2);
    EXPECT_FALSE(graph2 == this->graph);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_sameEdgesAndSize_returnTrue) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_differentLabels_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_missingEdge_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_differentEdges_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);
    graph2.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(testEdgeLabeledDirectedGraph, equalityOperator_oppositeEdges_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<TypeParam> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph.addEdgeIdx (1, 0, this->labels[0]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}


// Test integral types only

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, addEdgeIdx_validEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, addEdgeIdx_selfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, addEdgeIdx_multiedge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, addEdgeIdx_multiedgeForced_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(1, 2, this->labels[0]);
    this->graph.addEdgeIdx(1, 2, this->labels[0], true);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, addReciprocalEdgeIdx_validEdge_totalEdgeNumberUpdated) {
    this->graph.addReciprocalEdgeIdx(0, 1, this->labels[0]);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, setEdgeLabelTo_existentEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 2, this->labels[0]);
    this->graph.addEdgeIdx(0, 1, this->labels[1]);

    this->graph.setEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeEdgeIdx_existentEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeEdgeIdx_existentSelfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.removeEdgeIdx(0, 0);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeEdgeIdx_inexistentEdge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeMultiedges_noMultiedge_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeMultiedges_multiedge_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 1, this->labels[0], true);
    this->graph.addEdgeIdx(0, 1, this->labels[0], true);
    this->graph.addEdgeIdx(1, 1, this->labels[2]);

    this->graph.removeMultiedges();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeMultiedges_multiSelfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(1, 1, this->labels[1]);
    this->graph.addEdgeIdx(1, 1, this->labels[1], true);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 1, this->labels[1], true);

    this->graph.removeMultiedges();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeSelfLoops_noSelfLoop_doNothing) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);

    this->graph.removeSelfLoops();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeSelfLoops_existentSelfLoop_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 2, this->labels[1]);
    this->graph.addEdgeIdx(0, 0, this->labels[2]);

    this->graph.removeSelfLoops();

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, removeVertexFromEdgeListIdx_vertexInEdges_totalEdgeNumberUpdated) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 0, this->labels[3]);
    this->graph.addEdgeIdx(1, 0, this->labels[3], true);
    this->graph.addEdgeIdx(1, 3, this->labels[4]);

    this->graph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(this->graph.getTotalEdgeNumber(), this->labels[2]+this->labels[4]);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, clearEdges_anyGraph_totalNumberEdgeNumberIs0) {
    this->graph.addEdgeIdx(0, 1, this->labels[0]);
    this->graph.addEdgeIdx(0, 0, this->labels[1]);
    this->graph.addEdgeIdx(1, 2, this->labels[2]);
    this->graph.addEdgeIdx(1, 0, this->labels[3]);

    this->graph.clearEdges();
    EXPECT_EQ(this->graph.getTotalEdgeNumber(), 0);
}


TYPED_TEST(testEdgeLabeledDirectedGraph_integral, getSubgraphOfIdx_validVertexSubset_correctTotalEdgeNumber) {
    this->graph.addEdgeIdx          (0, 1, this->labels[0]);
    this->graph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->graph.addEdgeIdx          (2, 3, this->labels[2]);
    this->graph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph = this->graph.getSubgraphOfIdx({0, 2, 3});

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[3]+this->labels[4]);
}

TYPED_TEST(testEdgeLabeledDirectedGraph_integral, getSubgraphWithRemapOfIdx_validVertexSubset_correctTotalEdgeNumber) {
    this->graph.addEdgeIdx          (0, 1, this->labels[0]);
    this->graph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->graph.addEdgeIdx          (2, 3, this->labels[2]);
    this->graph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->graph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph_remap = this->graph.getSubgraphWithRemapOfIdx({0, 2, 3});
    auto& subgraph = subgraph_remap.first;

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[3]+this->labels[4]);
}
