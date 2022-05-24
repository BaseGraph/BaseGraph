#include <stdexcept>
#include <list>
#include <deque>

#include "gtest/gtest.h"
#include "BaseGraph/undirectedgraph.h"


TEST(UndirectedGraph, getNeighboursOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.getNeighboursOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getNeighboursOfIdx(2), std::out_of_range);
}

// When force=false in addEdgeIdx, hasEdgeIdx is called.
// Both methods depend on each other so one must be tested first arbitrarily.

TEST(UndirectedGraph, addEdgeIdx_validEdge_successorInAdjacency) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({2,1}) );
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({0}) );
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({0}) );
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, addEdgeIdx_selfLoop_successorInAdjacency) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(1, 1);

    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({1}) );
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, addEdgeIdx_multiedge_successorInAdjacencyOnce) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2);

    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({2}) );
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({1}) );
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

// Tests that force correctly bypasses hasEdgeIdx
TEST(UndirectedGraph, addEdgeIdx_multiedgeForced_successorInAdjacencyTwice) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2, true);

    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({2,2}) );
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({1,1}) );
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, addEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.addEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.addEdgeIdx(2, 1), std::out_of_range);
}


TEST(UndirectedGraph, hasEdgeIdx_existentEdge_ReturnTrue) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);

    EXPECT_TRUE(graph.hasEdgeIdx(0, 2));
    EXPECT_TRUE(graph.hasEdgeIdx(2, 0));
    EXPECT_TRUE(graph.hasEdgeIdx(0, 1));
    EXPECT_TRUE(graph.hasEdgeIdx(1, 0));
}

TEST(UndirectedGraph, hasEdgeIdx_inexistentEdge_ReturnFalse) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);

    EXPECT_FALSE(graph.hasEdgeIdx(2, 1));
    EXPECT_FALSE(graph.hasEdgeIdx(1, 2));
}

TEST(UndirectedGraph, hasEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.hasEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.hasEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.hasEdgeIdx(2, 1), std::out_of_range);
}

template<template<class ...> class Container, class ...Args>
static void testAllEdgesExistForContainer() {
    Container<BaseGraph::Edge> edges = {{0, 2}, {0, 1}, {0, 0}, {5, 10}};
    BaseGraph::UndirectedGraph graph(edges);

    for (auto edge: edges) {
        EXPECT_TRUE(graph.hasEdgeIdx(edge.first, edge.second));
        EXPECT_TRUE(graph.hasEdgeIdx(edge.second, edge.first));
    }
    EXPECT_EQ(graph.getEdgeNumber(), 4);
    EXPECT_EQ(graph.getSize(), 11);
}

TEST(UndirectedGraph, edgeListConstructor_anyContainer_allEdgesExist) {
    testAllEdgesExistForContainer<std::vector>();
    testAllEdgesExistForContainer<std::list>();
    testAllEdgesExistForContainer<std::set>();
    testAllEdgesExistForContainer<std::deque>();
}


TEST(UndirectedGraph, removeEdgeIdx_existentEdge_edgeDoesntExist) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (graph.hasEdgeIdx(0, 1));
    EXPECT_FALSE(graph.hasEdgeIdx(0, 2));
    EXPECT_EQ   (graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, removeEdgeIdx_existentSelfLoop_edgeDoesntExist) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.removeEdgeIdx(0, 0);

    EXPECT_TRUE (graph.hasEdgeIdx(0, 1));
    EXPECT_FALSE(graph.hasEdgeIdx(0, 0));
    EXPECT_EQ   (graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, removeEdgeIdx_inexistentEdge_edgeDoesntExist) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.removeEdgeIdx(0, 2);

    EXPECT_TRUE (graph.hasEdgeIdx(0, 1));
    EXPECT_FALSE(graph.hasEdgeIdx(0, 2));
    EXPECT_EQ   (graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, removeEdgeIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.removeEdgeIdx(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdgeIdx(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdgeIdx(2, 1), std::out_of_range);
}


TEST(UndirectedGraph, removeDuplicateEdges_noMultiedge_doNothing) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(1, 1);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({0, 1}));
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, removeDuplicateEdges_multiedge_removeMultiedge) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1, true);
    graph.addEdgeIdx(0, 1, true);
    graph.addEdgeIdx(1, 1);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({0, 1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, removeDuplicateEdges_multiSelfLoop_keepOnlyOneSelfLoop) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(1, 1);
    graph.addEdgeIdx(1, 1, true);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 1, true);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({0, 1, 2}));
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}


TEST(UndirectedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 0);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}


TEST(UndirectedGraph, removeVertexFromEdgeListIdx_vertexInEdes_vertexNotInEdges) {
    BaseGraph::UndirectedGraph graph(4);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(1, 0, true);
    graph.addEdgeIdx(1, 3);

    graph.removeVertexFromEdgeListIdx(0);

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({2, 3}));
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getNeighboursOfIdx(3), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, removeVertexFromEdgeListIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeListIdx(2), std::out_of_range);
}


TEST(UndirectedGraph, clearEdges_anyGraph_graphHasNoEdge) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 0);

    graph.clearEdges();

    EXPECT_EQ(graph.getNeighboursOfIdx(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getNeighboursOfIdx(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getNeighboursOfIdx(2), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeNumber(), 0);
}


TEST(UndirectedGraph, getSubgraphOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::UndirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 1);
    graph.addEdgeIdx(2, 3);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(3, 3);

    auto subgraph = graph.getSubgraphOfIdx({0, 2, 3});

    EXPECT_FALSE(subgraph.hasEdgeIdx(0, 1));
    EXPECT_FALSE(subgraph.hasEdgeIdx(2, 1));
    EXPECT_TRUE (subgraph.hasEdgeIdx(2, 3));
    EXPECT_TRUE (subgraph.hasEdgeIdx(0, 3));
    EXPECT_TRUE (subgraph.hasEdgeIdx(3, 3));
    EXPECT_EQ   (subgraph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, getSubgraphOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(3);

    EXPECT_THROW(graph.getSubgraphOfIdx({0, 2, 3}), std::out_of_range);
}


TEST(UndirectedGraph, getSubgraphWithRemapOfIdx_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::UndirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 1);
    graph.addEdgeIdx(2, 3);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(3, 3);


    auto subgraph_remap = graph.getSubgraphWithRemapOfIdx({0, 2, 3});
    auto& subgraph = subgraph_remap.first;
    auto& remap = subgraph_remap.second;

    EXPECT_EQ  (subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.hasEdgeIdx(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.hasEdgeIdx(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.hasEdgeIdx(remap[3], remap[3]));
    EXPECT_EQ  (subgraph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, getSubgraphWithRemapOfIdx_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(3);

    EXPECT_THROW(graph.getSubgraphWithRemapOfIdx({0, 2, 3}), std::out_of_range);
}


TEST(UndirectedGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 1, true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {{2, 1, 0},
                                                          {1, 0, 2},
                                                          {0, 2, 0}};
    EXPECT_EQ(graph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}


TEST(UndirectedGraph, getDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);
    graph.addEdgeIdx(0, 1, true);
    graph.addEdgeIdx(1, 0, true);

    EXPECT_EQ(graph.getDegrees(), std::vector<size_t>({5, 3, 0}) );
    EXPECT_EQ(graph.getDegreeOfIdx(0), 5);
    EXPECT_EQ(graph.getDegreeOfIdx(1), 3);
    EXPECT_EQ(graph.getDegreeOfIdx(2), 0);
}

TEST(UndirectedGraph, getDegreeOfIdx_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.getDegreeOfIdx(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getDegreeOfIdx(2), std::out_of_range);
}


TEST(UndirectedGraph, iterator_anyGraph_returnEachVertex) {
    BaseGraph::UndirectedGraph graph(10);
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::UndirectedGraph::iterator it=graph.begin(); it!=graph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for(BaseGraph::UndirectedGraph::iterator it=graph.begin(); it!=graph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TEST(UndirectedGraph, rangedBasedFor_anyGraph_returnEachVertex) {
    BaseGraph::UndirectedGraph graph(10);
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for(BaseGraph::VertexIndex& vertex: graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}


TEST(UndirectedGraph, equalityOperator_twoEmptyGraphs_returnTrue) {
    BaseGraph::UndirectedGraph graph(2);
    BaseGraph::UndirectedGraph graph2(2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_differentSize_returnFalse) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_sameEdgesAndSize_returnTrue) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);
    graph2.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(0, 1);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);
    graph2.addEdgeIdx(0, 1);
    graph2.addEdgeIdx(0, 2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_missingEdge_returnFalse) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(0, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_differentEdges_returnFalse) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(0, 2);
    graph2.addEdgeIdx(1, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}


TEST(UndirectedGraph, getDirectedGraph_anyUndirectedGraph_directedEdgesExistInBothDirections) {
    BaseGraph::UndirectedGraph undirectedGraph(3);
    undirectedGraph.addEdgeIdx(0, 1);
    undirectedGraph.addEdgeIdx(0, 2);
    undirectedGraph.addEdgeIdx(1, 1);

    auto directedGraph = undirectedGraph.getDirectedGraph();

    EXPECT_EQ  (directedGraph.getSize(), 3);
    EXPECT_EQ  (directedGraph.getEdgeNumber(), 5);
    EXPECT_TRUE(directedGraph.hasEdgeIdx(0, 1));
    EXPECT_TRUE(directedGraph.hasEdgeIdx(1, 0));
    EXPECT_TRUE(directedGraph.hasEdgeIdx(0, 2));
    EXPECT_TRUE(directedGraph.hasEdgeIdx(2, 0));
    EXPECT_TRUE(directedGraph.hasEdgeIdx(1, 1));
}

TEST(UndirectedGraph, constructFromDirected_anyDirectedGraph_everyEdgeExistOnce) {
    BaseGraph::DirectedGraph directedGraph(3);
    directedGraph.addEdgeIdx(0, 1);
    directedGraph.addReciprocalEdgeIdx(0, 2);
    directedGraph.addEdgeIdx(1, 1);

    BaseGraph::UndirectedGraph undirectedGraph(directedGraph);

    EXPECT_EQ  (undirectedGraph.getSize(), 3);
    EXPECT_EQ  (undirectedGraph.getEdgeNumber(), 3);
    EXPECT_TRUE(undirectedGraph.hasEdgeIdx(0, 1));
    EXPECT_TRUE(undirectedGraph.hasEdgeIdx(0, 2));
    EXPECT_TRUE(undirectedGraph.hasEdgeIdx(1, 1));
}
