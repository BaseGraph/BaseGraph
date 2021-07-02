#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "BaseGraph/directedgraph.h"


TEST(DirectedGraph, getOutEdgesOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.getOutEdgesOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutEdgesOfIdx(2), std::out_of_range);
}

// When force=false in addEdgeIdx, isEdgeIdx is called. 
// Both methods depend on each other so one must be tested first arbitrarily.

TEST(DirectedGraph, addEdgeIdx_validEdge_successorInAdjacency) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({2,1}) );
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, addEdgeIdx_selfLoop_successorInAdjacency) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(1, 1);

    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({1}) );
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, addEdgeIdx_multiedge_successorInAdjacencyOnce) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2);

    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({2}) );
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, addEdgeIdx_multiedgeForced_successorInAdjacencyTwice) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2, true);

    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({2,2}) );
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, addEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.addEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx(2, 1), std::out_of_range);
}


TEST(DirectedGraph, isEdgeIdx_existentEdge_ReturnTrue) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);

    EXPECT_TRUE(graph.isEdgeIdx(0, 2));
    EXPECT_TRUE(graph.isEdgeIdx(0, 1));
}

TEST(DirectedGraph, isEdgeIdx_inexistentEdge_ReturnFalse) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);

    EXPECT_FALSE(graph.isEdgeIdx(2, 0));
    EXPECT_FALSE(graph.isEdgeIdx(1, 0));
    EXPECT_FALSE(graph.isEdgeIdx(2, 1));
}

TEST(DirectedGraph, isEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.isEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.isEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.isEdgeIdx(2, 1), std::out_of_range);
}


TEST(DirectedGraph, addReciprocalEdgeIdx_validEdge_edgeExistsInBothDirections) {
    BaseGraph::DirectedGraph graph(3);
    graph.addReciprocalEdgeIdx(0, 1);

    EXPECT_TRUE(graph.isEdgeIdx(0, 1));
    EXPECT_TRUE(graph.isEdgeIdx(1, 0));
    EXPECT_EQ  (graph.getEdgeNumber(), 2);
}


TEST(DirectedGraph, removeEdgeIdx_existentEdge_edgeDoesntExist) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(graph.isEdgeIdx(0, 2));
    EXPECT_EQ   (graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, removeEdgeIdx_existentSelfLoop_edgeDoesntExist) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.removeEdgeIdx(0, 0);

    EXPECT_TRUE (graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(graph.isEdgeIdx(0, 0));
    EXPECT_EQ   (graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, removeEdgeIdx_inexistentEdge_edgeDoesntExist) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(graph.isEdgeIdx(0, 2));
    EXPECT_EQ   (graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, removeEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.removeEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdgeIdx(2, 1), std::out_of_range);
}


TEST(DirectedGraph, removeMultiedges_noMultiedge_doNothing) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(1, 1);

    graph.removeMultiedges();
    
    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(DirectedGraph, removeMultiedges_multiedge_removeMultiedge) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1, true);
    graph.addEdgeIdx(0, 1, true);
    graph.addEdgeIdx(1, 1);

    graph.removeMultiedges();
    
    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(DirectedGraph, removeMultiedges_multiSelfLoop_keepOnlyOneSelfLoop) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(1, 1);
    graph.addEdgeIdx(1, 1, true);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 1, true);

    graph.removeMultiedges();
    
    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}


TEST(DirectedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 0);

    graph.removeSelfLoops();
    
    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}


TEST(DirectedGraph, removeVertexFromEdgeListIdx_vertexInEdes_vertexNotInEdges) {
    BaseGraph::DirectedGraph graph(4);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(1, 0, true);
    graph.addEdgeIdx(1, 3);

    graph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({2, 3}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, removeVertexFromEdgeListIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(2), std::out_of_range);
}


TEST(DirectedGraph, clearEdges_anyGraph_graphHasNoEdge) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 0);

    graph.clearEdges();

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(2), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeNumber(), 0);
}


TEST(DirectedGraph, getSubgraph_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addReciprocalEdgeIdx(2, 1);
    graph.addEdgeIdx(2, 3);
    graph.addReciprocalEdgeIdx(0, 3);
    graph.addEdgeIdx(3, 3);

    auto subgraph = graph.getSubgraph({0, 2, 3});

    EXPECT_FALSE(subgraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(2, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(1, 2));
    EXPECT_TRUE (subgraph.isEdgeIdx(2, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(0, 3));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 0));
    EXPECT_TRUE (subgraph.isEdgeIdx(3, 3));
    EXPECT_EQ   (subgraph.getEdgeNumber(), 4);
}

TEST(DirectedGraph, getSubgraph_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(2);

    EXPECT_THROW(graph.getSubgraph({0, 2, 3}), std::out_of_range);
}


TEST(DirectedGraph, getSubgraphWithRemap_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addReciprocalEdgeIdx(2, 1);
    graph.addEdgeIdx(2, 3);
    graph.addReciprocalEdgeIdx(0, 3);
    graph.addEdgeIdx(3, 3);

    auto subgraph_remap = graph.getSubgraphWithRemap({0, 2, 3});
    auto& subgraph = subgraph_remap.first;
    auto& remap = subgraph_remap.second;

    EXPECT_EQ  (subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[0]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[3]));
    EXPECT_EQ  (subgraph.getEdgeNumber(), 4);
}

TEST(DirectedGraph, getSubgraphWithRemap_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(2);

    EXPECT_THROW(graph.getSubgraphWithRemap({0, 2, 3}), std::out_of_range);
}


TEST(DirectedGraph, getInEdges_anyGraph_returnAllInEdges) {
    BaseGraph::DirectedGraph graph(4);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(0, 1);
    graph.addReciprocalEdgeIdx(2, 1);
    graph.addEdgeIdx(3, 2);
    graph.addEdgeIdx(3, 1);

    EXPECT_EQ(graph.getInEdges(), BaseGraph::AdjacencyLists({{0}, {0, 2, 3}, {1, 3}, {}}) );
}


TEST(DirectedGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addReciprocalEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 1, true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {{1, 1, 0},
                                                          {0, 0, 1},
                                                          {0, 2, 0}};
    EXPECT_EQ(graph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}


TEST(DirectedGraph, getInDegreeIdx_anyGraph_returnCorrectDegrees) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(1, 0, true);
    graph.addEdgeIdx(0, 1);

    EXPECT_EQ(graph.getInDegrees(), std::vector<size_t>({3, 1, 0}) );
    EXPECT_EQ(graph.getInDegreeIdx(0), 3);
    EXPECT_EQ(graph.getInDegreeIdx(1), 1);
    EXPECT_EQ(graph.getInDegreeIdx(2), 0);
}

TEST(DirectedGraph, getInDegreeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.getInDegreeIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getInDegreeIdx(2), std::out_of_range);
}


TEST(DirectedGraph, getOutDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(0, 1, true);
    graph.addEdgeIdx(1, 0);

    EXPECT_EQ(graph.getOutDegrees(), std::vector<size_t>({3, 1, 0}) );
    EXPECT_EQ(graph.getOutDegreeIdx(0), 3);
    EXPECT_EQ(graph.getOutDegreeIdx(1), 1);
    EXPECT_EQ(graph.getOutDegreeIdx(2), 0);
}

TEST(DirectedGraph, getOutDegreeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.getOutDegreeIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutDegreeIdx(2), std::out_of_range);
}


TEST(DirectedGraph, getReversedGraph_anyGraph_onlyReverseEdgesExist) {
    BaseGraph::DirectedGraph graph(5);

    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);
    graph.addReciprocalEdgeIdx(3, 4);

    auto reversedGraph = graph.getReversedGraph();

    EXPECT_FALSE(reversedGraph.isEdgeIdx(1, 2));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(2, 1));
    EXPECT_FALSE(reversedGraph.isEdgeIdx(3, 1));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(1, 3));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(3, 4));
    EXPECT_TRUE (reversedGraph.isEdgeIdx(4, 3));

    EXPECT_EQ(reversedGraph.getEdgeNumber(), 4);
}


TEST(DirectedGraph, iterator_anyGraph_returnEachVertex) {
    BaseGraph::DirectedGraph graph(10);
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::DirectedGraph::iterator it=graph.begin(); it!=graph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for(BaseGraph::DirectedGraph::iterator it=graph.begin(); it!=graph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TEST(DirectedGraph, rangedBasedFor_anyGraph_returnEachVertex) {
    BaseGraph::DirectedGraph graph(10);
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::VertexIndex& vertex: graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TEST(DirectedGraph, comparisonOperator_twoEmptyGraphs_returnTrue) {
    BaseGraph::DirectedGraph graph(2);
    BaseGraph::DirectedGraph graph2(2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(DirectedGraph, comparisonOperator_differentSize_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(DirectedGraph, comparisonOperator_sameEdgesAndSize_returnTrue) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);
    graph2.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(0, 1);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(DirectedGraph, comparisonOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);
    graph2.addEdgeIdx(0, 1);
    graph2.addEdgeIdx(0, 2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(DirectedGraph, comparisonOperator_missingEdge_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(0, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(DirectedGraph, comparisonOperator_differentEdges_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(1, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(DirectedGraph, comparisonOperator_oppositeEdges_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(1, 0);
    graph2.addEdgeIdx(0, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}
