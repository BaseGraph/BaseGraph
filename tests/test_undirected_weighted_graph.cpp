#include <deque>
#include <list>
#include <set>
#include <stdexcept>
#include <vector>

#include "BaseGraph/types.h"
#include "BaseGraph/undirected_weighted_graph.hpp"
#include "fixtures.hpp"
#include "gtest/gtest.h"

TEST(UndirectedWeightedGraph, addEdge_inexistent_newMultiedge) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, 3);
    graph.addEdge(0, 2, -1.5);
    graph.addEdge(0, 0, 1);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getOutNeighbours(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 3);
    EXPECT_EQ(graph.getEdgeWeight(2, 0), -1.5);
    EXPECT_EQ(graph.getEdgeWeight(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalWeight(), 2.5);
}

TEST(UndirectedWeightedGraph, addEdge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.addEdge(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.addEdge(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.addEdge(0, 1, 1), std::out_of_range);
}

TEST(UndirectedWeightedGraph,
     removeEdge_existentEdge_noEdgeAndTotalWeightUpdated) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 2, 3);
    graph.addEdge(0, 0, -3.5);

    graph.removeEdge(0, 2);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getOutNeighbours(2), BaseGraph::Successors({}));
    EXPECT_FALSE(graph.hasEdge(0, 2));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalWeight(), -2.5);
}

TEST(UndirectedWeightedGraph, removeEgde_inexistentEdge_graphUnchanged) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 0, -.5);

    graph.removeEdge(0, 2);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 1);
    EXPECT_EQ(graph.getEdgeWeight(0, 0), -.5);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalWeight(), .5);
}

TEST(UndirectedWeightedGraph, removeEdge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.removeEdge(0, 0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.removeEdge(1, 0), std::out_of_range);
    EXPECT_THROW(graph.removeEdge(0, 1), std::out_of_range);
}

TEST(UndirectedWeightedGraph, setEdgeWeight_inexistentEdge_addEdge) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 2, 1.5);
    graph.setEdgeWeight(0, 1, -2);
    graph.addEdge(0, 0, 1);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 1, 0}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getOutNeighbours(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeWeight(0, 2), 1.5);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), -2);
    EXPECT_EQ(graph.getEdgeWeight(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalWeight(), .5);
}

TEST(UndirectedWeightedGraph, setEdgeWeight_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.setEdgeWeight(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.setEdgeWeight(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.setEdgeWeight(0, 1, 1), std::out_of_range);
}

TEST(UndirectedWeightedGraph, getEdgeWeight_existentEdge_returnCorrectWeight) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 0, -1.5);
    graph.addEdge(0, 1, 2);
    graph.addEdge(1, 2, 1);

    EXPECT_EQ(graph.getEdgeWeight(0, 1), 2);
    EXPECT_EQ(graph.getEdgeWeight(1, 0), 2);
    EXPECT_EQ(graph.getEdgeWeight(0, 0), -1.5);
}

TEST(UndirectedWeightedGraph, getEdgeWeight_inexistentEdge_throwInvalidArgument) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 0, 1);
    graph.addEdge(0, 1, 2);

    EXPECT_THROW(graph.getEdgeWeight(0, 2, true), std::invalid_argument);
}

TEST(UndirectedWeightedGraph, getEdgeWeight_inexistentEdgeNoThrow_return0) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 0, 1);
    graph.addEdge(0, 1, 2);

    EXPECT_EQ(graph.getEdgeWeight(0, 2, false), 0);
}

TEST(UndirectedWeightedGraph, getEdgeWeight_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedWeightedGraph graph(0);
    EXPECT_THROW(graph.getEdgeWeight(0, 0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getEdgeWeight(1, 0), std::out_of_range);
    EXPECT_THROW(graph.getEdgeWeight(0, 1), std::out_of_range);
}

TEST(UndirectedWeightedGraph,
     getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    BaseGraph::UndirectedWeightedGraph graph(3);
    graph.addEdge(0, 1, -2);
    graph.addEdge(0, 0, .5);
    graph.addEdge(2, 0, 2);

    EXPECT_EQ(graph.getWeightMatrix(),
              BaseGraph::WeightMatrix({{.5, -2, 2}, {-2, 0, 0}, {2, 0, 0}}));
}

const std::vector<BaseGraph::EdgeWeight> weights = {-10, -3.5, 2.75, 50, 100};

TEST(UndirectedWeightedGraph, removeDuplicateEdges_noMultiedge_doNothing) {
    BaseGraph::UndirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);
    graph.addEdge(1, 1, weights[2]);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1] + weights[2]);
}

TEST(UndirectedWeightedGraph,
     removeDuplicateEdges_multiedge_totalWeightUpdated) {
    BaseGraph::UndirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);
    graph.addEdge(0, 1, weights[0], true);
    graph.addEdge(0, 1, weights[0], true);
    graph.addEdge(1, 1, weights[2]);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1] + weights[2]);
}

TEST(UndirectedWeightedGraph,
     removeDuplicateEdges_multiSelfLoop_totalWeightUpdated) {
    BaseGraph::UndirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(1, 1, weights[1]);
    graph.addEdge(1, 1, weights[1], true);
    graph.addEdge(1, 2, weights[2]);
    graph.addEdge(1, 1, weights[1], true);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1] + weights[2]);
}

TEST(UndirectedWeightedGraph, removeSelfLoops_noSelfLoop_doNothing) {
    BaseGraph::UndirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1]);
}

TEST(UndirectedWeightedGraph,
     removeSelfLoops_existentSelfLoop_totalWeightUpdated) {
    BaseGraph::UndirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 2, weights[1]);
    graph.addEdge(0, 0, weights[2]);

    graph.removeSelfLoops();

    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_EQ(graph.getTotalWeight(), weights[0] + weights[1]);
}

TEST(UndirectedWeightedGraph,
     removeVertexFromEdgeList_vertexInEdges_totalWeightUpdated) {
    BaseGraph::UndirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 0, weights[1]);
    graph.addEdge(1, 2, weights[2]);
    graph.addEdge(1, 0, weights[3]);
    graph.addEdge(1, 3, weights[4]);

    graph.removeVertexFromEdgeList(0);

    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(0, 0));
    EXPECT_EQ(graph.getTotalWeight(), weights[2] + weights[4]);
}

TEST(UndirectedWeightedGraph, clearEdges_anyGraph_graphHasNoEdge) {
    BaseGraph::UndirectedWeightedGraph graph(weights.size());
    graph.addEdge(0, 1, weights[0]);
    graph.addEdge(0, 0, weights[1]);
    graph.addEdge(1, 2, weights[2]);
    graph.addEdge(1, 0, weights[3]);

    graph.clearEdges();

    EXPECT_EQ(graph.getTotalWeight(), 0);
}
