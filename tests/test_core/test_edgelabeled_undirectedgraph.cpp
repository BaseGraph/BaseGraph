#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "fixtures.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


template<typename T>
using Type_isIntegral = std::pair<T, std::is_integral<T>>;

using TestTypes = ::testing::Types<
                        Type_isIntegral<std::string>,
                        Type_isIntegral<int>
                    >;
using TestTypes_integral = ::testing::Types<int>;


#define LabeledSuccessors typename BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type>::LabeledSuccessors
#define isIntegral TypeParam::second_type::value

TYPED_TEST_SUITE(EdgeLabeledGraph, TestTypes);
TYPED_TEST_SUITE(EdgeLabeledGraph_integral, TestTypes_integral);


TYPED_TEST(EdgeLabeledGraph, getOutEdgesOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.getOutEdgesOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutEdgesOfIdx(2), std::out_of_range);
}

// When force=false in addEdgeIdx, isEdgeIdx is called. 
// Both methods depend on each other so one must be tested first arbitrarily.

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_validEdge_successorInAdjacency) {
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(1, 0, this->labels[1]);

    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, this->labels[0]}, {1, this->labels[1]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[1]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{0, this->labels[0]}}) );
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_selfLoop_successorInAdjacency) {
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{1, this->labels[0]}}) );
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_multiedge_successorInAdjacencyOnce) {
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(2, 1, this->labels[0]);

    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{2, this->labels[0]}}) );
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_multiedgeForced_successorInAdjacencyTwice) {
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(2, 1, this->labels[0], true);

    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{2, this->labels[0]}, {2, this->labels[0]}}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, addEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.addEdgeIdx(0, 0, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdgeIdx(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx(2, 1, this->labels[0]), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, isEdgeIdx_existentEdge_ReturnTrue) {
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_TRUE(this->undirectedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(this->undirectedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(this->undirectedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(this->undirectedGraph.isEdgeIdx(1, 0));
}

TYPED_TEST(EdgeLabeledGraph, isEdgeIdx_inexistentEdge_ReturnFalse) {
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(0, 2));
    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(2, 1));
    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(1, 2));
}

TYPED_TEST(EdgeLabeledGraph, isEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.isEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.isEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.isEdgeIdx(2, 1), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getEdgeLabelOf_existentEdge_correctLabel) {
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->undirectedGraph.getEdgeLabelOf(0, 2), this->labels[0]);
    EXPECT_EQ(this->undirectedGraph.getEdgeLabelOf(2, 0), this->labels[0]);
    EXPECT_EQ(this->undirectedGraph.getEdgeLabelOf(0, 1), this->labels[1]);
    EXPECT_EQ(this->undirectedGraph.getEdgeLabelOf(1, 0), this->labels[1]);
}

TYPED_TEST(EdgeLabeledGraph, getEdgeLabelOf_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->undirectedGraph.getEdgeLabelOf(0, 2), std::invalid_argument);
    EXPECT_THROW(this->undirectedGraph.getEdgeLabelOf(2, 0), std::invalid_argument);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->undirectedGraph.getEdgeLabelOf(0, 2), std::invalid_argument);
    EXPECT_THROW(this->undirectedGraph.getEdgeLabelOf(2, 0), std::invalid_argument);
}

TYPED_TEST(EdgeLabeledGraph, getEdgeLabelOf_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.getEdgeLabelOf(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getEdgeLabelOf(1, 2), std::out_of_range);
    EXPECT_THROW(graph.getEdgeLabelOf(2, 1), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, changeEdgeLabelTo_existentEdge_labelChanged) {
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[1]);

    this->undirectedGraph.changeEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, this->labels[0]}, {1, this->labels[0]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[0]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{0, this->labels[0]}}) );

    this->undirectedGraph.changeEdgeLabelTo(0, 2, this->labels[1]);
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, this->labels[1]}, {1, this->labels[0]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[0]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{0, this->labels[1]}}) );
}

TYPED_TEST(EdgeLabeledGraph, changeEdgeLabelTo_inexistentEdge_throwInvalidArgument) {
    EXPECT_THROW(this->undirectedGraph.changeEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    EXPECT_THROW(this->undirectedGraph.changeEdgeLabelTo(0, 2, this->labels[0]), std::invalid_argument);
}

TYPED_TEST(EdgeLabeledGraph, changeEdgeLabelTo_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.changeEdgeLabelTo(0, 0, this->labels[0]), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.changeEdgeLabelTo(1, 2, this->labels[0]), std::out_of_range);
    EXPECT_THROW(graph.changeEdgeLabelTo(2, 1, this->labels[0]), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_existentEdge_edgeDoesntExist) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.removeEdgeIdx(0, 2);


    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[1]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[1]}}) );
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({}) );
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_existentSelfLoop_edgeDoesntExist) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->undirectedGraph.removeEdgeIdx(0, 0);

    EXPECT_TRUE (this->undirectedGraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(0, 0));
    EXPECT_EQ   (this->undirectedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_inexistentEdge_edgeDoesntExist) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (this->undirectedGraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(this->undirectedGraph.isEdgeIdx(0, 2));
    EXPECT_EQ   (this->undirectedGraph.getDistinctEdgeNumber(), 1);
}

TYPED_TEST(EdgeLabeledGraph, removeEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.removeEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdgeIdx(2, 1), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, removeMultiedges_noMultiedge_doNothing) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->undirectedGraph.removeMultiedges();
    
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[0]}, {1, this->labels[2]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{0, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(EdgeLabeledGraph, removeMultiedges_multiedge_keepOneEdge) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->undirectedGraph.removeMultiedges();
    
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[0]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{0, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 3);
}

TYPED_TEST(EdgeLabeledGraph, removeMultiedges_multiSelfLoop_keepOnlyOneSelfLoop) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[1], true);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[1], true);

    this->undirectedGraph.removeMultiedges();
    
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{1, this->labels[1]}, {2, this->labels[2]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{1, this->labels[2]}}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 3);
}


TYPED_TEST(EdgeLabeledGraph, removeSelfLoops_noSelfLoop_doNothing) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);

    this->undirectedGraph.removeSelfLoops();

    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[0]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{0, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[2]);

    this->undirectedGraph.removeSelfLoops();
    
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, this->labels[0]}, {2, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{0, this->labels[0]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{0, this->labels[1]}}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 2);
}


TYPED_TEST(EdgeLabeledGraph, removeVertexFromEdgeListIdx_vertexInEdes_vertexNotInEdges) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(1, 0, this->labels[3], true);
    this->undirectedGraph.addEdgeIdx(1, 3, this->labels[4]);

    this->undirectedGraph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({{2, this->labels[2]}, {3, this->labels[4]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({{1, this->labels[2]}}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(3), LabeledSuccessors({{1, this->labels[4]}}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 2);
}

TYPED_TEST(EdgeLabeledGraph, removeVertexFromEdgeListIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(2), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, clearEdges_anyGraph_graphHasNoEdge) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(1, 0, this->labels[3]);

    this->undirectedGraph.clearEdges();

    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(0), LabeledSuccessors({}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(1), LabeledSuccessors({}));
    EXPECT_EQ(this->undirectedGraph.getOutEdgesOfIdx(2), LabeledSuccessors({}));
    EXPECT_EQ(this->undirectedGraph.getDistinctEdgeNumber(), 0);
}


TYPED_TEST(EdgeLabeledGraph, getSubgraph_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(2, 1, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(2, 3, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(0, 3, this->labels[3]);
    this->undirectedGraph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph = this->undirectedGraph.getSubgraph({0, 2, 3});

    EXPECT_FALSE(subgraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(2, 1));
    EXPECT_TRUE (subgraph.isEdgeIdx(2, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 0));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 3));
    EXPECT_EQ   (subgraph.getDistinctEdgeNumber(), 4);
}

TYPED_TEST(EdgeLabeledGraph, getSubgraph_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(2);

    EXPECT_THROW(graph.getSubgraph({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getSubgraphWithRemap_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(2, 1, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(2, 3, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(0, 3, this->labels[3]);
    this->undirectedGraph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph_remap = this->undirectedGraph.getSubgraphWithRemap({0, 2, 3});
    auto& subgraph = subgraph_remap.first;
    auto& remap = subgraph_remap.second;

    EXPECT_EQ  (subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[3]));
    EXPECT_EQ  (subgraph.getDistinctEdgeNumber(), 4);
}

TYPED_TEST(EdgeLabeledGraph, getSubgraphWithRemap_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(2);

    EXPECT_THROW(graph.getSubgraphWithRemap({0, 2, 3}), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(2, 1, this->labels[3], true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {{1, 1, 0, 0},
                                                          {0, 0, 1, 0},
                                                          {0, 2, 0, 0},
                                                          {0, 0, 0, 0}};
    EXPECT_EQ(this->undirectedGraph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}


TYPED_TEST(EdgeLabeledGraph, getDegrees_anyGraph_returnCorrectDegrees) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[2], true);

    EXPECT_EQ(this->undirectedGraph.getDegrees(), std::vector<size_t>({3, 2, 0, 0}) );
    EXPECT_EQ(this->undirectedGraph.getDegreeIdx(0), 3);
    EXPECT_EQ(this->undirectedGraph.getDegreeIdx(1), 2);
    EXPECT_EQ(this->undirectedGraph.getDegreeIdx(2), 0);
    EXPECT_EQ(this->undirectedGraph.getDegreeIdx(3), 0);
}

TYPED_TEST(EdgeLabeledGraph, getDegreeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(0);

    EXPECT_THROW(graph.getDegreeIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getDegreeIdx(2), std::out_of_range);
}


TYPED_TEST(EdgeLabeledGraph, iterator_anyGraph_returnEachVertex) {
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(auto it=this->undirectedGraph.begin(); it!=this->undirectedGraph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for(auto it=this->undirectedGraph.begin(); it!=this->undirectedGraph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TYPED_TEST(EdgeLabeledGraph, rangedBasedFor_anyGraph_returnEachVertex) {
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::VertexIndex& vertex: this->undirectedGraph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TYPED_TEST(EdgeLabeledGraph, comparisonOperator_twoEmptyGraphs_returnTrue) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(2);
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph2(2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_differentSize_returnFalse) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph2(2);

    EXPECT_FALSE(this->undirectedGraph == graph2);
    EXPECT_FALSE(graph2 == this->undirectedGraph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_sameEdgesAndSize_returnTrue) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 2, this->labels[0]);
    graph.addEdgeIdx (0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 1, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[0]);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_missingEdge_returnFalse) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TYPED_TEST(EdgeLabeledGraph, comparisonOperator_differentEdges_returnFalse) {
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph(3);
    BaseGraph::EdgeLabeledUndirectedGraph<typename TypeParam::first_type> graph2(3);
    graph.addEdgeIdx (0, 1, this->labels[0]);
    graph.addEdgeIdx (0, 2, this->labels[1]);
    graph2.addEdgeIdx(0, 2, this->labels[1]);
    graph2.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}


// Test integral types only

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_validEdge_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[1]);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_selfLoop_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[0]);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_multiedge_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[0]);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, addEdgeIdx_multiedgeForced_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[0], true);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(EdgeLabeledGraph_integral, changeEdgeLabelTo_existentEdge_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[1]);

    this->undirectedGraph.changeEdgeLabelTo(0, 1, this->labels[0]);
    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[0]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeEdgeIdx_existentEdge_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeEdgeIdx_existentSelfLoop_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->undirectedGraph.removeEdgeIdx(0, 0);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeEdgeIdx_inexistentEdge_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.removeEdgeIdx(0, 2);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeMultiedges_noMultiedge_doNothing) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->undirectedGraph.removeMultiedges();
    
    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeMultiedges_multiedge_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0], true);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[2]);

    this->undirectedGraph.removeMultiedges();
    
    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeMultiedges_multiSelfLoop_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[1], true);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(1, 1, this->labels[1], true);

    this->undirectedGraph.removeMultiedges();
    
    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]+this->labels[2]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeSelfLoops_noSelfLoop_doNothing) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);

    this->undirectedGraph.removeSelfLoops();

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}

TYPED_TEST(EdgeLabeledGraph_integral, removeSelfLoops_existentSelfLoop_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 2, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[2]);

    this->undirectedGraph.removeSelfLoops();
    
    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[0]+this->labels[1]);
}


TYPED_TEST(EdgeLabeledGraph_integral, removeVertexFromEdgeListIdx_vertexInEdes_totalEdgeNumberUpdated) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(1, 0, this->labels[3]);
    this->undirectedGraph.addEdgeIdx(1, 0, this->labels[3], true);
    this->undirectedGraph.addEdgeIdx(1, 3, this->labels[4]);

    this->undirectedGraph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(this->undirectedGraph.getTotalEdgeNumber(), this->labels[2]+this->labels[4]);
}


TYPED_TEST(EdgeLabeledGraph_integral, clearEdges_anyGraph_graphHasNoEdge) {
    this->directedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->directedGraph.addEdgeIdx(0, 0, this->labels[1]);
    this->directedGraph.addEdgeIdx(1, 2, this->labels[2]);
    this->directedGraph.addEdgeIdx(1, 0, this->labels[3]);

    this->directedGraph.clearEdges();

    EXPECT_EQ(this->directedGraph.getDistinctEdgeNumber(), 0);
}


TYPED_TEST(EdgeLabeledGraph_integral, getSubgraph_validVertexSubset_correctTotalEdgeNumber) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(2, 1, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(2, 3, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(0, 3, this->labels[3]);
    this->undirectedGraph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph = this->undirectedGraph.getSubgraph({0, 2, 3});

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[4]);
}

TYPED_TEST(EdgeLabeledGraph_integral, getSubgraphWithRemap_validVertexSubset_correctTotalEdgeNumber) {
    this->undirectedGraph.addEdgeIdx(0, 1, this->labels[0]);
    this->undirectedGraph.addEdgeIdx(2, 1, this->labels[1]);
    this->undirectedGraph.addEdgeIdx(2, 3, this->labels[2]);
    this->undirectedGraph.addEdgeIdx(0, 3, this->labels[3]);
    this->undirectedGraph.addEdgeIdx(3, 3, this->labels[4]);

    auto subgraph_remap = this->undirectedGraph.getSubgraphWithRemap({0, 2, 3});
    auto& subgraph = subgraph_remap.first;

    EXPECT_EQ(subgraph.getTotalEdgeNumber(), this->labels[2]+this->labels[3]+this->labels[4]);
}

