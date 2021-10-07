#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "fixtures.hpp"
#include "BaseGraph/edgelabeled_directedgraph.hpp"


template<typename T>
using Type_isIntegral = std::pair<T, std::is_integral<T>>;

using TestTypes = ::testing::Types<
                        Type_isIntegral<std::string>,
                        Type_isIntegral<int>
                    >;
using TestTypes_integral = ::testing::Types<int>;


#define LabeledSuccessors typename BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type>::LabeledSuccessors
#define isIntegral TypeParam::second_type::value

TYPED_TEST_SUITE(EdgeLabeledGraph, TestTypes);
TYPED_TEST_SUITE(EdgeLabeledGraph_integral, TestTypes_integral);


TYPED_TEST(EdgeLabeledGraph, getOutEdgesOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.getOutEdgesOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutEdgesOfIdx(2), std::out_of_range);
}

// When force=false in addEdgeIdx, isEdgeIdx is called.
// Both methods depend on each other so one must be tested first arbitrarily.

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_validEdge_successorInAdjacency) {
    this->directedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, this->labels[0]}, {1, this->labels[1]}}) );
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_selfLoop_successorInAdjacency) {
    this->directedGraph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{1, this->labels[0]}}) );
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_multiedge_successorInAdjacencyOnce) {
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{2, this->labels[0]}}) );
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_multiedgeForced_successorInAdjacencyTwice) {
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0], true);

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{2, this->labels[0]}, {2, this->labels[0]}}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.addEdgeIdx(0, 0, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdgeIdx(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx(2, 1, this->labels[0]), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, isEdgeIdx_existentEdge_ReturnTrue) {
    this->directedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_TRUE(this->directedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(this->directedGraph.isEdgeIdx(0, 1));
}

TYPED_TEST(EdgeLabeledGraph, isEdgeIdx_inexistentEdge_ReturnFalse) {
    this->directedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_FALSE(this->directedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(this->directedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(this->directedGraph.isEdgeIdx(2, 1));
}

TYPED_TEST(EdgeLabeledGraph, isEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.isEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.isEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.isEdgeIdx(2, 1), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getEdgeLabelOf_existentEdge_correctLabel) {
    this->directedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->directedGraph.getEdgeLabelOf(0, 2), this->labels[0]);
    EXPECT_EQ(this->directedGraph.getEdgeLabelOf(0, 1), this->labels[1]);
}

TYPED_TEST(EdgeLabeledGraph, getEdgeLabelOf_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->directedGraph.getEdgeLabelOf(0, 2), std::invalid_argument);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->directedGraph.getEdgeLabelOf(0, 2), std::invalid_argument);
}

TYPED_TEST(EdgeLabeledGraph, getEdgeLabelOf_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.getEdgeLabelOf(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getEdgeLabelOf(1, 2), std::out_of_range);
    EXPECT_THROW(graph.getEdgeLabelOf(2, 1), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, setEdgeLabelTo_existentEdge_labelChanged) {
    this->directedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[1]);

    this->directedGraph.setEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, this->labels[0]}, {1, this->labels[0]}}) );
    this->directedGraph.setEdgeLabelTo(0, 2, this->labels[1]);
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, this->labels[1]}, {1, this->labels[0]}}) );
}

TYPED_TEST(EdgeLabeledGraph, setEdgeLabelTo_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->directedGraph.setEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->directedGraph.setEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
}

TYPED_TEST(EdgeLabeledGraph, setEdgeLabelTo_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.setEdgeLabelTo(0, 0, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.setEdgeLabelTo(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.setEdgeLabelTo(2, 1, this->labels[0]), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, addReciprocalEdgeIdx_validEdge_edgeExistsInBothDirections) {
    this->directedGraph.addReciprocalEdgeIdx(0, 1, this->labels[0]);

    EXPECT_TRUE(this->directedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(this->directedGraph.isEdgeIdx(1, 0));
    EXPECT_EQ  (this->directedGraph.getDistinctEdgeNumber(), 2);
}


TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_existentEdge_edgeDoesntExist) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (this->directedGraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->directedGraph.isEdgeIdx(0, 2));
    EXPECT_EQ   (this->directedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_existentSelfLoop_edgeDoesntExist) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.removeEdgeIdx(0, 0);

    EXPECT_TRUE (this->directedGraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->directedGraph.isEdgeIdx(0, 0));
    EXPECT_EQ   (this->directedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_inexistentEdge_edgeDoesntExist) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (this->directedGraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->directedGraph.isEdgeIdx(0, 2));
    EXPECT_EQ   (this->directedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.removeEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdgeIdx(2, 1), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, removeMultiedges_noMultiedge_doNothing) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->directedGraph.removeMultiedges();

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{1, this->labels[2]}}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(EdgeLabeledGraph, removeMultiedges_multiedge_keepOneEdge) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->directedGraph.removeMultiedges();

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{1, this->labels[2]}}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(EdgeLabeledGraph, removeMultiedges_multiSelfLoop_keepOnlyOneSelfLoop) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[1], true);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[1], true);

    this->directedGraph.removeMultiedges();

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{1, this->labels[1]}, {2, this->labels[2]}}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 3);
}


TYPED_TEST(EdgeLabeledGraph, removeSelfLoops_noSelfLoop_doNothing) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);

    this->directedGraph.removeSelfLoops();

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[2]);

    this->directedGraph.removeSelfLoops();

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 2);
}


TYPED_TEST(EdgeLabeledGraph, removeVertexFromEdgeListIdx_vertexInEdes_vertexNotInEdges) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3], true);
    this->directedGraph.addEdgeIdx(1, 3, this->labels[4]);

    this->directedGraph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{2, this->labels[2]}, {3, this->labels[4]}}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, removeVertexFromEdgeListIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(2), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, clearEdges_anyGraph_graphHasNoEdge) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3]);

    this->directedGraph.clearEdges();

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({}));
    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 0);
}


TYPED_TEST(EdgeLabeledGraph, getSubgraphOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    this->directedGraph.addEdgeIdx          (0, 1, this->labels[0]);
    this->directedGraph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->directedGraph.addEdgeIdx          (2, 3, this->labels[2]);
    this->directedGraph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->directedGraph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph = this->directedGraph.getSubgraphOfIdx({0, 2, 3});

    EXPECT_FALSE(subgraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(2, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(1, 2));
    EXPECT_TRUE (subgraph.isEdgeIdx(2, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(0, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 0));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 3));
    EXPECT_EQ   (subgraph.getDistinctEdgeNumber(), 4);
}

TYPED_TEST(EdgeLabeledGraph, getSubgraphOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(2);

    EXPECT_THROW(graph.getSubgraphOfIdx({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getSubgraphWithRemapOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    this->directedGraph.addEdgeIdx          (0, 1, this->labels[0]);
    this->directedGraph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->directedGraph.addEdgeIdx          (2, 3, this->labels[2]);
    this->directedGraph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->directedGraph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph_remap = this->directedGraph.getSubgraphWithRemapOfIdx({0, 2, 3});
    auto& subgraph = subgraph_remap.first;
    auto& remap = subgraph_remap.second;

    EXPECT_EQ  (subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[0]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[3]));
    EXPECT_EQ  (subgraph.getDistinctEdgeNumber(), 4);
}

TYPED_TEST(EdgeLabeledGraph, getSubgraphWithRemapOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(2);

    EXPECT_THROW(graph.getSubgraphWithRemapOfIdx({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getInEdges_anyGraph_returnAllInEdges) {
    this->directedGraph.addEdgeIdx          (0, 0, this->labels[0]);
    this->directedGraph.addEdgeIdx          (0, 1, this->labels[1]);
    this->directedGraph.addReciprocalEdgeIdx(2, 1, this->labels[2]);
    this->directedGraph.addEdgeIdx          (3, 2, this->labels[3]);
    this->directedGraph.addEdgeIdx          (3, 1, this->labels[4]);

    EXPECT_EQ(this->directedGraph.getInEdges(), std::vector<LabeledSuccessors>({
                {{0, this->labels[0]}},
                {{0, this->labels[1]}, {2, this->labels[2]}, {3, this->labels[4]}},
                {{1, this->labels[2]}, {3, this->labels[3]}},
                {}}) );
}


TYPED_TEST(EdgeLabeledGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    this->directedGraph.addEdgeIdx          (0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx          (0, 0, this->labels[1]);
    this->directedGraph.addReciprocalEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx          (2, 1, this->labels[3], true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {{1, 1, 0, 0},
                                                          {0, 0, 1, 0},
                                                          {0, 2, 0, 0},
                                                          {0, 0, 0, 0}};
    EXPECT_EQ(this->directedGraph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}


TYPED_TEST(EdgeLabeledGraph, getInDegreeOfIdx_anyGraph_returnCorrectDegrees) {
    this->directedGraph.addEdgeIdx(1, 0, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[2], true);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[3]);

    EXPECT_EQ(this->directedGraph.getInDegrees(), std::vector<size_t>({3, 1, 0, 0}) );
    EXPECT_EQ(this->directedGraph.getInDegreeOfIdx(0), 3);
    EXPECT_EQ(this->directedGraph.getInDegreeOfIdx(1), 1);
    EXPECT_EQ(this->directedGraph.getInDegreeOfIdx(2), 0);
}

TYPED_TEST(EdgeLabeledGraph, getInDegreeOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.getInDegreeOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getInDegreeOfIdx(2), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getOutDegrees_anyGraph_returnCorrectDegrees) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[2], true);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3]);

    EXPECT_EQ(this->directedGraph.getOutDegrees(), std::vector<size_t>({3, 1, 0, 0}) );
    EXPECT_EQ(this->directedGraph.getOutDegreeOfIdx(0), 3);
    EXPECT_EQ(this->directedGraph.getOutDegreeOfIdx(1), 1);
    EXPECT_EQ(this->directedGraph.getOutDegreeOfIdx(2), 0);
    EXPECT_EQ(this->directedGraph.getOutDegreeOfIdx(3), 0);
}

TYPED_TEST(EdgeLabeledGraph, getOutDegreeOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.getOutDegreeOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutDegreeOfIdx(2), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getReversedGraph_anyGraph_onlyReverseEdgesExist) {
    this->directedGraph.addEdgeIdx          (0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx          (2, 0, this->labels[1]);
    this->directedGraph.addReciprocalEdgeIdx(2, 3, this->labels[2]);

    auto reversedGraph = this->directedGraph.getReversedGraph();

    EXPECT_FALSE(reversedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(reversedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(2, 3));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(3, 2));

    EXPECT_EQ(reversedGraph.getDistinctEdgeNumber(), 4);
}


TYPED_TEST(EdgeLabeledGraph, iterator_anyGraph_returnEachVertex) {
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(auto it=this->directedGraph.begin(); it!=this->directedGraph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for(auto it=this->directedGraph.begin(); it!=this->directedGraph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TYPED_TEST(EdgeLabeledGraph, rangedBasedFor_anyGraph_returnEachVertex) {
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::VertexIndex& vertex: this->directedGraph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TYPED_TEST(EdgeLabeledGraph, comparisonOperator_twoEmptyGraphs_returnTrue) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(2);
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph2(2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_differentSize_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph2(2);

    EXPECT_FALSE(this->directedGraph == graph2);
    EXPECT_FALSE(graph2 == this->directedGraph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_sameEdgesAndSize_returnTrue) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_missingEdge_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_differentEdges_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);
    graph2.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_oppositeEdges_returnFalse) {
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledDirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph.addEdgeIdx (1, 0, this->labels[0]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}


// Test integral types only

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_validEdge_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_selfLoop_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_multiedge_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_multiedgeForced_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[0], true);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(EdgeLabeledGraph_integral, addReciprocalEdgeIdx_validEdge_totalEdgeNumberUpdated) {
    this->directedGraph.addReciprocalEdgeIdx(0, 1, this->labels[0]);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(EdgeLabeledGraph_integral, setEdgeLabelTo_existentEdge_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[1]);

    this->directedGraph.setEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeEdgeIdx_existentEdge_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeEdgeIdx_existentSelfLoop_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.removeEdgeIdx(0, 0);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeEdgeIdx_inexistentEdge_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeMultiedges_noMultiedge_doNothing) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->directedGraph.removeMultiedges();

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeMultiedges_multiedge_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->directedGraph.removeMultiedges();

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeMultiedges_multiSelfLoop_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[1], true);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx(1, 1, this->labels[1], true);

    this->directedGraph.removeMultiedges();

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeSelfLoops_noSelfLoop_doNothing) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);

    this->directedGraph.removeSelfLoops();

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeSelfLoops_existentSelfLoop_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[2]);

    this->directedGraph.removeSelfLoops();

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeVertexFromEdgeListIdx_vertexInEdes_totalEdgeNumberUpdated) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3], true);
    this->directedGraph.addEdgeIdx(1, 3, this->labels[4]);

    this->directedGraph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), this->labels[2]+this->labels[4]);
}


TYPED_TEST(EdgeLabeledGraph_integral, clearEdges_anyGraph_totalNumberEdgeNumberIs0) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3]);

    this->directedGraph.clearEdges();
    EXPECT_EQ(this->directedGraph.getTotalEdgeNumber(), 0);
}


TYPED_TEST(EdgeLabeledGraph_integral, getSubgraphOfIdx_validVertexSubset_correctTotalEdgeNumber) {
    this->directedGraph.addEdgeIdx          (0, 1, this->labels[0]);
    this->directedGraph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->directedGraph.addEdgeIdx          (2, 3, this->labels[2]);
    this->directedGraph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->directedGraph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph = this->directedGraph.getSubgraphOfIdx({0, 2, 3});

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[3]+this->labels[4]);
}

TYPED_TEST(EdgeLabeledGraph_integral, getSubgraphWithRemapOfIdx_validVertexSubset_correctTotalEdgeNumber) {
    this->directedGraph.addEdgeIdx          (0, 1, this->labels[0]);
    this->directedGraph.addReciprocalEdgeIdx(2, 1, this->labels[1]);
    this->directedGraph.addEdgeIdx          (2, 3, this->labels[2]);
    this->directedGraph.addReciprocalEdgeIdx(0, 3, this->labels[3]);
    this->directedGraph.addEdgeIdx          (3, 3, this->labels[4]);

    auto subgraph_remap = this->directedGraph.getSubgraphWithRemapOfIdx({0, 2, 3});
    auto& subgraph = subgraph_remap.first;

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[3]+this->labels[4]);
}
