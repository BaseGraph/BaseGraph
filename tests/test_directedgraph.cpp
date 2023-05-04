#include <algorithm>
#include <deque>
#include <list>
#include <stdexcept>

#include "BaseGraph/directed_graph.hpp"
#include "gtest/gtest.h"

TEST(DirectedGraph, getEdgesFrom_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.getOutNeighbours(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutNeighbours(2), std::out_of_range);
}

// When force=false in addEdge, hasEdge is called.
// Both methods depend on each other so one must be tested first arbitrarily.

TEST(DirectedGraph, addEdge_validEdge_successorInAdjacency) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 1}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, addEdge_selfLoop_successorInAdjacency) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(1, 1);

    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, addEdge_multiedge_successorInAdjacencyOnce) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 2);

    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({2}));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, addEdge_multiedgeForced_successorInAdjacencyTwice) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 2, true);

    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({2, 2}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, addEdge_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.addEdge(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdge(1, 2), std::out_of_range);
    EXPECT_THROW(graph.addEdge(2, 1), std::out_of_range);
}

TEST(DirectedGraph, hasEdge_existentEdge_ReturnTrue) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);

    EXPECT_TRUE(graph.hasEdge(0, 2));
    EXPECT_TRUE(graph.hasEdge(0, 1));
}

TEST(DirectedGraph, hasEdge_inexistentEdge_ReturnFalse) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);

    EXPECT_FALSE(graph.hasEdge(2, 0));
    EXPECT_FALSE(graph.hasEdge(1, 0));
    EXPECT_FALSE(graph.hasEdge(2, 1));
}

TEST(DirectedGraph, hasEdge_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.hasEdge(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.hasEdge(1, 2), std::out_of_range);
    EXPECT_THROW(graph.hasEdge(2, 1), std::out_of_range);
}

template <template <class...> class Container, class... Args>
static void testAllEdgesExistForContainer() {
    Container<BaseGraph::Edge> edges = {{0, 2}, {0, 1}, {0, 0}, {10, 5}};
    BaseGraph::DirectedGraph graph(edges);

    for (auto edge : edges)
        EXPECT_TRUE(graph.hasEdge(edge.first, edge.second));
    EXPECT_EQ(graph.getEdgeNumber(), 4);
    EXPECT_EQ(graph.getSize(), 11);
}

TEST(DirectedGraph, edgeListConstructor_anyContainer_allEdgesExist) {
    testAllEdgesExistForContainer<std::vector>();
    testAllEdgesExistForContainer<std::list>();
    testAllEdgesExistForContainer<std::set>();
    testAllEdgesExistForContainer<std::deque>();
}

TEST(DirectedGraph, edgeListConstructor_list_allEdgesExist) {
    std::list<BaseGraph::Edge> edges = {{0, 2}, {0, 1}, {3, 0}, {5, 10}};
    BaseGraph::DirectedGraph graph(edges);

    for (auto edge : edges)
        EXPECT_TRUE(graph.hasEdge(edge.first, edge.second));
    EXPECT_EQ(graph.getEdgeNumber(), 4);
    EXPECT_EQ(graph.getSize(), 11);
}

TEST(DirectedGraph, addReciprocalEdge_validEdge_edgeExistsInBothDirections) {
    BaseGraph::DirectedGraph graph(3);
    graph.addReciprocalEdge(0, 1);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 0));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, removeEdge_existentEdge_edgeDoesntExist) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.removeEdge(0, 2);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, removeEdge_existentSelfLoop_edgeDoesntExist) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.removeEdge(0, 0);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, removeEdge_inexistentEdge_edgeDoesntExist) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.removeEdge(0, 2);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(DirectedGraph, removeEdge_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.removeEdge(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdge(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdge(2, 1), std::out_of_range);
}

TEST(DirectedGraph, removeDuplicateEdges_noMultiedge_doNothing) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 1);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(DirectedGraph, removeDuplicateEdges_multiedge_removeMultiedge) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1, true);
    graph.addEdge(0, 1, true);
    graph.addEdge(1, 1);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(DirectedGraph, removeDuplicateEdges_multiSelfLoop_keepOnlyOneSelfLoop) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(1, 1);
    graph.addEdge(1, 1, true);
    graph.addEdge(1, 2);
    graph.addEdge(1, 1, true);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(DirectedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 0);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph, removeVertexFromEdgeList_vertexInEdges_vertexNotInEdges) {
    BaseGraph::DirectedGraph graph(4);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(1, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 0, true);
    graph.addEdge(1, 3);

    graph.removeVertexFromEdgeList(0);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({2, 3}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(DirectedGraph,
     removeVertexFromEdgeList_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeList(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeList(2), std::out_of_range);
}

TEST(DirectedGraph, clearEdges_anyGraph_graphHasNoEdge) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(1, 2);
    graph.addEdge(1, 0);

    graph.clearEdges();

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutNeighbours(2), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeNumber(), 0);
}

TEST(DirectedGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addReciprocalEdge(1, 2);
    graph.addEdge(2, 1, true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {
        {1, 1, 0}, {0, 0, 1}, {0, 2, 0}};
    EXPECT_EQ(graph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}

TEST(DirectedGraph, getInDegree_anyGraph_returnCorrectDegrees) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(1, 0);
    graph.addEdge(0, 0);
    graph.addEdge(1, 0, true);
    graph.addEdge(0, 1);

    EXPECT_EQ(graph.getInDegrees(), std::vector<size_t>({3, 1, 0}));
    EXPECT_EQ(graph.getInDegree(0), 3);
    EXPECT_EQ(graph.getInDegree(1), 1);
    EXPECT_EQ(graph.getInDegree(2), 0);
}

TEST(DirectedGraph, getInDegree_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.getInDegree(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getInDegree(2), std::out_of_range);
}

TEST(DirectedGraph, getOutDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::DirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(0, 1, true);
    graph.addEdge(1, 0);

    EXPECT_EQ(graph.getOutDegrees(), std::vector<size_t>({3, 1, 0}));
    EXPECT_EQ(graph.getOutDegree(0), 3);
    EXPECT_EQ(graph.getOutDegree(1), 1);
    EXPECT_EQ(graph.getOutDegree(2), 0);
}

TEST(DirectedGraph, getOutDegree_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(0);

    EXPECT_THROW(graph.getOutDegree(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getOutDegree(2), std::out_of_range);
}

TEST(DirectedGraph, getReversedGraph_anyGraph_onlyReverseEdgesExist) {
    BaseGraph::DirectedGraph graph(5);

    graph.addEdge(1, 2);
    graph.addEdge(3, 1);
    graph.addReciprocalEdge(3, 4);

    auto reversedGraph = graph.getReversedGraph();

    EXPECT_FALSE(reversedGraph.hasEdge(1, 2));
    EXPECT_TRUE(reversedGraph.hasEdge(2, 1));
    EXPECT_FALSE(reversedGraph.hasEdge(3, 1));
    EXPECT_TRUE(reversedGraph.hasEdge(1, 3));
    EXPECT_TRUE(reversedGraph.hasEdge(3, 4));
    EXPECT_TRUE(reversedGraph.hasEdge(4, 3));

    EXPECT_EQ(reversedGraph.getEdgeNumber(), 4);
}

TEST(DirectedGraph, iterator_anyGraph_returnEachVertex) {
    BaseGraph::DirectedGraph graph(10);
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3, 4,
                                                          5, 6, 7, 8, 9};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for (auto it = graph.begin(); it != graph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for (auto it = graph.begin(); it != graph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}

TEST(DirectedGraph, rangeBasedFor_anyGraph_returnEachVertex) {
    BaseGraph::DirectedGraph graph(10);
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3, 4,
                                                          5, 6, 7, 8, 9};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for (BaseGraph::VertexIndex &vertex : graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}

TEST(DirectedGraph, edgeRangeFor_firstVertexHasNoNeighbour_returnEachEdge) {
    // Order of the follow list must be kepts for the lists to match
    std::list<BaseGraph::Edge> edges = {{1, 2}, {1, 0}, {1, 1}, {3, 0}};
    BaseGraph::DirectedGraph graph(edges);

    std::list<BaseGraph::Edge> loopEdges;
    for (const BaseGraph::Edge &edge : graph.edges())
        loopEdges.push_back(edge);
    EXPECT_EQ(loopEdges, edges);
}

TEST(DirectedGraph, edgeRangeFor_lastVertexHasNoNeighbour_returnEachEdge) {
    // Order of the follow list must be kepts for the lists to match
    std::list<BaseGraph::Edge> edges = {{0, 2}, {0, 0}, {0, 1}, {1, 1}, {3, 0}};
    BaseGraph::DirectedGraph graph(edges);
    graph.resize(5);

    std::list<BaseGraph::Edge> loopEdges;
    for (const BaseGraph::Edge &edge : graph.edges())
        loopEdges.push_back(edge);
    EXPECT_EQ(loopEdges, edges);
}

TEST(DirectedGraph, edgeRangeFor_emptyGraph_returnNoEdge) {
    BaseGraph::DirectedGraph graph(5);

    std::list<BaseGraph::Edge> loopEdges, edges;
    for (const BaseGraph::Edge &edge : graph.edges())
        loopEdges.push_back(edge);
    EXPECT_EQ(loopEdges, edges);
}

TEST(DirectedGraph, equalityOperator_twoEmptyGraphs_returnTrue) {
    BaseGraph::DirectedGraph graph(2);
    BaseGraph::DirectedGraph graph2(2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(DirectedGraph, equalityOperator_differentSize_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(DirectedGraph, equalityOperator_sameEdgesAndSize_returnTrue) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);
    graph2.addEdge(0, 2);
    graph2.addEdge(0, 1);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(DirectedGraph, equalityOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);
    graph2.addEdge(0, 1);
    graph2.addEdge(0, 2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(DirectedGraph, equalityOperator_missingEdge_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph2.addEdge(0, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(DirectedGraph, equalityOperator_differentEdges_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph2.addEdge(0, 2);
    graph2.addEdge(1, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(DirectedGraph, equalityOperator_oppositeEdges_returnFalse) {
    BaseGraph::DirectedGraph graph(3);
    BaseGraph::DirectedGraph graph2(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph2.addEdge(0, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}
