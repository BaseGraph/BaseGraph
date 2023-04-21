#include "gtest/gtest.h"
#include <deque>
#include <list>
#include <queue>
#include <stdexcept>

#include "BaseGraph/directed_weighted_graph.hpp"
#include "BaseGraph/types.h"
#include "fixtures.hpp"

TEST(DirectedWeightedGraph, addEdge_inexistent_newMultiedge) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, 3.5);
    graph.addEdge(0, 2, 1);
    graph.addEdge(0, 0, -1);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 3.5);
    EXPECT_EQ(graph.getEdgeWeight(0, 2), 1);
    EXPECT_EQ(graph.getEdgeWeight(0, 0), -1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalWeight(), 3.5);
}

TEST(DirectedWeightedGraph, addEdge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.addEdge(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.addEdge(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.addEdge(0, 1, 1), std::out_of_range);
}

TEST(DirectedWeightedGraph, removeEdge_noEdgeAndTotalWeightDecremented) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, 1.5);
    graph.addEdge(0, 2, 3);
    graph.addEdge(0, 0, -1);

    graph.removeEdge(0, 2);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 0}));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalWeight(), .5);
}

TEST(DirectedWeightedGraph, removeEdge_inexistentEdge_graphUnchanged) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 0, -1);

    graph.removeEdge(0, 2);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalWeight(), 0);
}

TEST(DirectedWeightedGraph, removeEdge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.removeEdge(0, 0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.removeEdge(1, 0), std::out_of_range);
    EXPECT_THROW(graph.removeEdge(0, 1), std::out_of_range);
}

TEST(DirectedWeightedGraph, setEdgeWeight_inexistentEdge_addEdge) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 2, 1);
    graph.setEdgeWeight(0, 1, 2.5);
    graph.addEdge(0, 0, -1);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 1, 0}));
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 2.5);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalWeight(), 2.5);
}

TEST(DirectedWeightedGraph,
     setEdgeWeight_existentEdge_weightAndTotalWeightUpdated) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 2, 1);
    graph.addEdge(0, 1, 0);
    graph.addEdge(0, 0, 1);

    graph.setEdgeWeight(0, 1, 1.5);
    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 1, 0}));
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 1.5);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalWeight(), 3.5);

    graph.setEdgeWeight(0, 1, -2);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), -2);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalWeight(), 0);
}

TEST(DirectedWeightedGraph, setEdgeWeight_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.setEdgeWeight(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.setEdgeWeight(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.setEdgeWeight(0, 1, 1), std::out_of_range);
}

TEST(DirectedWeightedGraph, getEdgeWeight_existentEdge_retrunEdgeMultiplicity) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 0, 1);
    graph.addEdge(0, 1, 2.5);

    EXPECT_EQ(graph.getEdgeWeight(0, 1), 2.5);
}

TEST(DirectedWeightedGraph, getEdgeWeight_inexistentEdge_throwInvalidArgument) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 0, 1);
    graph.addEdge(0, 1, 2);

    EXPECT_THROW(graph.getEdgeWeight(0, 2, true), std::invalid_argument);
    EXPECT_THROW(graph.getEdgeWeight(1, 0, true), std::invalid_argument);
}

TEST(DirectedWeightedGraph, getEdgeWeight_inexistentEdgeNoThrow_return0) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 0, 1);
    graph.addEdge(0, 1, 2);

    EXPECT_EQ(graph.getEdgeWeight(0, 2, false), 0);
    EXPECT_EQ(graph.getEdgeWeight(1, 0, false), 0);
}

TEST(DirectedWeightedGraph, getEdgeWeight_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.getEdgeWeight(0, 0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getEdgeWeight(1, 0), std::out_of_range);
    EXPECT_THROW(graph.getEdgeWeight(0, 1), std::out_of_range);
}

TEST(DirectedWeightedGraph, getEdgeMatrix_anyGraph_returnCorrectWeights) {
    BaseGraph::DirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, -2);
    graph.addEdge(0, 0, 1);
    graph.addEdge(1, 0, 1.75);

    EXPECT_EQ(graph.getWeightMatrix(),
              BaseGraph::WeightMatrix({{1, -2, 0}, {1.75, 0, 0}, {0, 0, 0}}));
}

const std::vector<BaseGraph::EdgeWeight> weights = {-10, -3, 2, 50, 100};

TEST(DirectedWeightedGraph, removeDuplicateEdges_noDuplicateEdge_doNothing) {
    BaseGraph::DirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);
    graph.addEdge(1, 1, weights[2]);

    graph.removeDuplicateEdges();

    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(0, 2));
    EXPECT_TRUE(graph.hasEdge(1, 1));
    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1] + weights[2]);
}

TEST(DirectedWeightedGraph, removeDuplicateEdges_multiedge_totalWeightUpdated) {
    BaseGraph::DirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);
    graph.addEdge(0, 1, weights[0], true);
    graph.addEdge(0, 1, weights[0], true);
    graph.addEdge(1, 1, weights[2]);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1] + weights[2]);
}

TEST(DirectedWeightedGraph,
     removeDuplicateEdges_multiSelfLoop_totalWeightUpdated) {
    BaseGraph::DirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(1, 1, weights[1]);
    graph.addEdge(1, 1, weights[1], true);
    graph.addEdge(1, 2, weights[2]);
    graph.addEdge(1, 1, weights[1], true);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1] + weights[2]);
}

TEST(DirectedWeightedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    BaseGraph::DirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1]);
}

TEST(DirectedWeightedGraph,
     removeSelfLoops_existentSelfLoop_loopRemovedAndTotalWeightUpdated) {
    BaseGraph::DirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);
    graph.addEdge(0, 0, weights[2]);

    graph.removeSelfLoops();

    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1]);
}

TEST(
    DirectedWeightedGraph,
    removeVertexFromEdgeList_vertexInEdges_edgesWithVertexRemovedAndTotalWeightUpdated) {
    BaseGraph::DirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 0, weights[1]);
    graph.addEdge(1, 2, weights[2]);
    graph.addEdge(1, 0, weights[3]);
    graph.addEdge(1, 0, weights[3], true);
    graph.addEdge(1, 3, weights[4]);

    graph.removeVertexFromEdgeList(0);

    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_FALSE(graph.hasEdge(1, 0));
    EXPECT_EQ(graph.getTotalWeight(), weights[2] + weights[4]);
}

TEST(DirectedWeightedGraph, clearEdges_anyGraph_totalNumberEdgeNumberIs0) {
    BaseGraph::DirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 0, weights[1]);
    graph.addEdge(1, 2, weights[2]);
    graph.addEdge(1, 0, weights[3]);

    graph.clearEdges();
    EXPECT_EQ(graph.getTotalWeight(), 0);
}
