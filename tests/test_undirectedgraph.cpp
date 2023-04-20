#include <algorithm>
#include <deque>
#include <list>
#include <stdexcept>

#include "BaseGraph/undirected_graph.hpp"
#include "gtest/gtest.h"

TEST(UndirectedGraph, getNeighbours_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.getNeighbours(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getNeighbours(2), std::out_of_range);
}

// When force=false in addEdge, hasEdge is called.
// Both methods depend on each other so one must be tested first arbitrarily.

TEST(UndirectedGraph, addEdge_validEdge_successorInAdjacency) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({2, 1}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, addEdge_selfLoop_successorInAdjacency) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(1, 1);

    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, addEdge_multiedge_successorInAdjacencyOnce) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 2);

    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({2}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

// Tests that force correctly bypasses hasEdge
TEST(UndirectedGraph, addEdge_multiedgeForced_successorInAdjacencyTwice) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 2, true);

    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({2, 2}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({1, 1}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, addEdge_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.addEdge(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.addEdge(1, 2), std::out_of_range);
    EXPECT_THROW(graph.addEdge(2, 1), std::out_of_range);
}

TEST(UndirectedGraph, hasEdge_existentEdge_ReturnTrue) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);

    EXPECT_TRUE(graph.hasEdge(0, 2));
    EXPECT_TRUE(graph.hasEdge(2, 0));
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 0));
}

TEST(UndirectedGraph, hasEdge_inexistentEdge_ReturnFalse) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);

    EXPECT_FALSE(graph.hasEdge(2, 1));
    EXPECT_FALSE(graph.hasEdge(1, 2));
}

TEST(UndirectedGraph, hasEdge_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.hasEdge(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.hasEdge(1, 2), std::out_of_range);
    EXPECT_THROW(graph.hasEdge(2, 1), std::out_of_range);
}

template <template <class...> class Container, class... Args>
static void testAllEdgesExistForContainer() {
    Container<BaseGraph::Edge> edges = {{0, 2}, {0, 1}, {0, 0}, {5, 10}};
    BaseGraph::UndirectedGraph graph(edges);

    for (auto edge : edges) {
        EXPECT_TRUE(graph.hasEdge(edge.first, edge.second));
        EXPECT_TRUE(graph.hasEdge(edge.second, edge.first));
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

TEST(UndirectedGraph, removeEdge_existentEdge_edgeDoesntExist) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.removeEdge(0, 2);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, removeEdge_existentSelfLoop_edgeDoesntExist) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.removeEdge(0, 0);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, removeEdge_inexistentEdge_edgeDoesntExist) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.removeEdge(0, 2);

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(UndirectedGraph, removeEdge_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.removeEdge(0, 0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeEdge(1, 2), std::out_of_range);
    EXPECT_THROW(graph.removeEdge(2, 1), std::out_of_range);
}

TEST(UndirectedGraph, removeDuplicateEdges_noMultiedge_doNothing) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 1);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({0, 1}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, removeDuplicateEdges_multiedge_removeMultiedge) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1, true);
    graph.addEdge(0, 1, true);
    graph.addEdge(1, 1);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({0, 1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, removeDuplicateEdges_multiSelfLoop_keepOnlyOneSelfLoop) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(1, 1);
    graph.addEdge(1, 1, true);
    graph.addEdge(1, 2);
    graph.addEdge(1, 1, true);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({0, 1, 2}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, removeSelfLoops_existentSelfLoop_removeSelfLoop) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 0);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph, removeVertexFromEdgeList_vertexInEdes_vertexNotInEdges) {
    BaseGraph::UndirectedGraph graph(4);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(1, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 0, true);
    graph.addEdge(1, 3);

    graph.removeVertexFromEdgeList(0);

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({2, 3}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getNeighbours(3), BaseGraph::Successors({1}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(UndirectedGraph,
     removeVertexFromEdgeList_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.removeVertexFromEdgeList(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.removeVertexFromEdgeList(2), std::out_of_range);
}

TEST(UndirectedGraph, clearEdges_anyGraph_graphHasNoEdge) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(1, 2);
    graph.addEdge(1, 0);

    graph.clearEdges();

    EXPECT_EQ(graph.getNeighbours(0), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getNeighbours(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getNeighbours(2), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeNumber(), 0);
}

TEST(UndirectedGraph, getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(1, 2);
    graph.addEdge(2, 1, true);

    BaseGraph::AdjacencyMatrix expectedAdjacencyMatrix = {
        {2, 1, 0}, {1, 0, 2}, {0, 2, 0}};
    EXPECT_EQ(graph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}

TEST(UndirectedGraph, getDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(0, 1, true);
    graph.addEdge(1, 0, true);

    EXPECT_EQ(graph.getDegrees(), std::vector<size_t>({5, 3, 0}));
    EXPECT_EQ(graph.getDegree(0), 5);
    EXPECT_EQ(graph.getDegree(1), 3);
    EXPECT_EQ(graph.getDegree(2), 0);
}

TEST(UndirectedGraph, getDegrees_selfLoopsCountedTwice_returnCorrectDegrees) {
    BaseGraph::UndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(0, 1, true);
    graph.addEdge(1, 0, true);

    EXPECT_EQ(graph.getDegrees(false), std::vector<size_t>({4, 3, 0}));
    EXPECT_EQ(graph.getDegree(0, false), 4);
    EXPECT_EQ(graph.getDegree(1, false), 3);
    EXPECT_EQ(graph.getDegree(2, false), 0);
}

TEST(UndirectedGraph, getDegree_vertexOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(0);

    EXPECT_THROW(graph.getDegree(0), std::out_of_range);
    graph.resize(2);
    EXPECT_THROW(graph.getDegree(2), std::out_of_range);
}

TEST(UndirectedGraph, iterator_anyGraph_returnEachVertex) {
    BaseGraph::UndirectedGraph graph(10);
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

TEST(UndirectedGraph, rangeBasedFor_anyGraph_returnEachVertex) {
    BaseGraph::UndirectedGraph graph(10);
    std::list<BaseGraph::VertexIndex> expectedVertices = {0, 1, 2, 3, 4,
                                                          5, 6, 7, 8, 9};
    std::list<BaseGraph::VertexIndex> loopVertices;

    for (BaseGraph::VertexIndex &vertex : graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}

TEST(UndirectedGraph, edgeRangeFor_firstVertexHasNoNeighbour_returnEachEdge) {
    // Order of the follow list must be kept for the lists to match
    std::list<BaseGraph::Edge> edges = {{1, 2}, {1, 1}, {1, 5}};
    BaseGraph::UndirectedGraph graph(edges);

    std::list<BaseGraph::Edge> loopEdges;
    for (const BaseGraph::Edge &edge : graph.edges())
        loopEdges.push_back(edge);
    EXPECT_EQ(loopEdges, edges);
}

TEST(UndirectedGraph, edgeRangeFor_lastVertexHasNoNeighbour_returnEachEdge) {
    // Order of the follow list must be kept for the lists to match
    std::list<BaseGraph::Edge> edges = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 1}};
    BaseGraph::UndirectedGraph graph(edges);
    graph.resize(5);

    std::list<BaseGraph::Edge> loopEdges;
    for (const BaseGraph::Edge &edge : graph.edges())
        loopEdges.push_back(edge);
    EXPECT_EQ(loopEdges, edges);
}

TEST(UndirectedGraph, edgeRangeFor_unorderedAdjacency_returnEachEdge) {
    // e.g. The iterator must skip (3, 1) before returning (3, 4)
    BaseGraph::UndirectedGraph graph(std::list<BaseGraph::Edge>{
        {3, 1}, {3, 4}, {3, 3}, {3, 0}, {3, 2}, {4, 4}});

    std::list<BaseGraph::Edge> loopEdges;
    for (const BaseGraph::Edge &edge : graph.edges())
        loopEdges.push_back(edge);

    std::list<BaseGraph::Edge> expectedEdges = {{0, 3}, {1, 3}, {2, 3},
                                                {3, 4}, {3, 3}, {4, 4}};
    EXPECT_EQ(loopEdges, expectedEdges);
}

TEST(UndirectedGraph, edgeRangeFor_emptyGraph_returnNoEdge) {
    BaseGraph::UndirectedGraph graph(5);

    std::list<BaseGraph::Edge> loopEdges, edges;
    for (const BaseGraph::Edge &edge : graph.edges())
        loopEdges.push_back(edge);
    EXPECT_EQ(loopEdges, edges);
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
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);
    graph2.addEdge(0, 2);
    graph2.addEdge(0, 1);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_differentEdgeOrder_returnTrue) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);
    graph2.addEdge(0, 1);
    graph2.addEdge(0, 2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_missingEdge_returnFalse) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph2.addEdge(0, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(UndirectedGraph, equalityOperator_differentEdges_returnFalse) {
    BaseGraph::UndirectedGraph graph(3);
    BaseGraph::UndirectedGraph graph2(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph2.addEdge(0, 2);
    graph2.addEdge(1, 2);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(UndirectedGraph,
     getDirectedGraph_anyUndirectedGraph_directedEdgesExistInBothDirections) {
    BaseGraph::UndirectedGraph undirectedGraph(3);
    undirectedGraph.addEdge(0, 1);
    undirectedGraph.addEdge(0, 2);
    undirectedGraph.addEdge(1, 1);

    auto directedGraph = undirectedGraph.getDirectedGraph();

    EXPECT_EQ(directedGraph.getSize(), 3);
    EXPECT_EQ(directedGraph.getEdgeNumber(), 5);
    EXPECT_TRUE(directedGraph.hasEdge(0, 1));
    EXPECT_TRUE(directedGraph.hasEdge(1, 0));
    EXPECT_TRUE(directedGraph.hasEdge(0, 2));
    EXPECT_TRUE(directedGraph.hasEdge(2, 0));
    EXPECT_TRUE(directedGraph.hasEdge(1, 1));
}

TEST(UndirectedGraph,
     constructFromDirected_anyDirectedGraph_everyEdgeExistOnce) {
    BaseGraph::DirectedGraph directedGraph(3);
    directedGraph.addEdge(0, 1);
    directedGraph.addReciprocalEdge(0, 2);
    directedGraph.addEdge(1, 1);

    BaseGraph::UndirectedGraph undirectedGraph(directedGraph);

    EXPECT_EQ(undirectedGraph.getSize(), 3);
    EXPECT_EQ(undirectedGraph.getEdgeNumber(), 3);
    EXPECT_TRUE(undirectedGraph.hasEdge(0, 1));
    EXPECT_TRUE(undirectedGraph.hasEdge(0, 2));
    EXPECT_TRUE(undirectedGraph.hasEdge(1, 1));
}
