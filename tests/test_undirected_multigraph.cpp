#include <list>
#include <stdexcept>

#include "BaseGraph/undirected_multigraph.hpp"
#include "fixtures.hpp"
#include "gtest/gtest.h"

TEST(UndirectedMultigraph, addMultiedge_inexistent_newMultiedge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addMultiedge(0, 1, 3);
    graph.addMultiedge(0, 2, 1);
    graph.addEdge(0, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 3);
    EXPECT_EQ(graph.getEdgeMultiplicity(2, 0), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 5);
}

TEST(UndirectedMultigraph, addMultiedge_existent_multiplicityIncremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 2, 1);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(2, 0), 2);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);
}

TEST(UndirectedMultigraph,
     addMultiedge_existentSelfLoop_multiplicityIncremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);
    graph.addEdge(0, 2);
    graph.addMultiedge(0, 0, 1);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0, 2}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 2);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);
}

TEST(UndirectedMultigraph,
     addMultiedge_existentMultiedgeAndForce_newMultiedge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 2, 1, true);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0, 2}));
    EXPECT_EQ(graph.getEdgeNumber(), 4);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);
}

TEST(UndirectedMultigraph, addMultiedge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.addMultiedge(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.addMultiedge(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.addMultiedge(0, 1, 1), std::out_of_range);
}

TEST(
    UndirectedMultigraph,
    removeMultiedge_existentEdgeWithHigherMultiplicity_multiplicityDecremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 2, 3);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 2);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 3);
}

TEST(UndirectedMultigraph,
     removeMultiedge_existentEdgeWithEqualMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 2, 3);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 3);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(UndirectedMultigraph,
     removeMultiedge_existentEdgeWithLowerMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 2, 3);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 4);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(
    UndirectedMultigraph,
    removeMultiedge_existentSelfLoopWithHigherMultiplicity_multiplicityDecremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 0, 3);
    graph.addEdge(0, 2);

    graph.removeMultiedge(0, 0, 2);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0, 2}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 3);
}

TEST(UndirectedMultigraph,
     removeMultiedge_existentSelfLoopWithEqualMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 0, 3);
    graph.addEdge(0, 2);

    graph.removeMultiedge(0, 0, 3);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(UndirectedMultigraph,
     removeMultiedge_existentSelfLoopWithLowerMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 0, 3);
    graph.addEdge(0, 2);

    graph.removeMultiedge(0, 0, 4);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(UndirectedMultigraph, removeMultiedge_inexistentEdge_graphUnchanged) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 0, 2);

    graph.removeMultiedge(0, 2, 4);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 2);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 3);
}

TEST(UndirectedMultigraph, removeMultiedge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.removeMultiedge(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.removeMultiedge(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.removeMultiedge(0, 1, 1), std::out_of_range);
}

TEST(UndirectedMultigraph,
     setEdgeMultiplicity_inexistentEdgeToPositiveMultiplicity_addEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 2);
    graph.setEdgeMultiplicity(0, 1, 2);
    graph.addEdge(0, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({2, 1, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getOutEdgesOf(1), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 2);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);
}

TEST(UndirectedMultigraph,
     setEdgeMultiplicity_inexistentEdgeToMultiplicity0_doNothing) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 2);
    graph.setEdgeMultiplicity(0, 1, 0);
    graph.addEdge(0, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({2, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 0);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(UndirectedMultigraph,
     setEdgeMultiplicity_existentEdgeToMultiplicity0_removeEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 2, 1);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 0);
    graph.setEdgeMultiplicity(0, 1, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({2, 0}));
    EXPECT_EQ(graph.getOutEdgesOf(1), BaseGraph::Successors({}));
    EXPECT_EQ(graph.getOutEdgesOf(2), BaseGraph::Successors({0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 0);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(
    UndirectedMultigraph,
    setEdgeMultiplicity_existentEdgeToNonZeroMultiplicity_multiplicityAndEdgeNumberUpdated) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);

    graph.setEdgeMultiplicity(0, 1, 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 3);

    graph.setEdgeMultiplicity(0, 1, 2);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 2);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);

    graph.setEdgeMultiplicity(0, 1, 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 3);
}

TEST(UndirectedMultigraph,
     setEdgeMultiplicity_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.setEdgeMultiplicity(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.setEdgeMultiplicity(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.setEdgeMultiplicity(0, 1, 1), std::out_of_range);
}

/* Assumed to work
TEST(UndirectedMultigraph, getEdgeMultiplicity_edgeOfMultiplicity2_return2) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 1, 2);
    graph.addEdge(1, 2);

    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 2);
    EXPECT_EQ(graph.getEdgeMultiplicity(1, 0), 2);
}

TEST(UndirectedMultigraph, getEdgeMultiplicity_inexistentEdge_return0) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 1, 2);

    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 0);
    EXPECT_EQ(graph.getEdgeMultiplicity({0, 2}), 0);
}
*/

TEST(UndirectedMultigraph,
     getEdgeMultiplicity_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.getEdgeMultiplicity(0, 0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getEdgeMultiplicity(1, 0), std::out_of_range);
    EXPECT_THROW(graph.getEdgeMultiplicity(0, 1), std::out_of_range);
}

TEST(UndirectedMultigraph,
     getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addMultiedge(0, 1, 2);
    graph.addEdge(0, 0);
    graph.addEdge(1, 0);

    EXPECT_EQ(graph.getAdjacencyMatrix(),
              BaseGraph::AdjacencyMatrix({{2, 3, 0}, {3, 0, 0}, {0, 0, 0}}));
}

TEST(UndirectedMultigraph, getDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addMultiedge(0, 1, 2);
    graph.addMultiedge(0, 0, 2);
    graph.addEdge(1, 0);

    EXPECT_EQ(graph.getDegrees(), std::vector<size_t>({7, 3, 0}));
    EXPECT_EQ(graph.getDegreeOf(0), 7);
    EXPECT_EQ(graph.getDegreeOf(1), 3);
    EXPECT_EQ(graph.getDegreeOf(2), 0);
}

TEST(UndirectedMultigraph, getDegrees_countSelfLoopsOnce_returnCorrectDegrees) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addMultiedge(0, 1, 2);
    graph.addMultiedge(0, 0, 2);
    graph.addEdge(1, 0);

    EXPECT_EQ(graph.getDegrees(false), std::vector<size_t>({5, 3, 0}));
    EXPECT_EQ(graph.getDegreeOf(0, false), 5);
    EXPECT_EQ(graph.getDegreeOf(1, false), 3);
    EXPECT_EQ(graph.getDegreeOf(2, false), 0);
}

TEST(UndirectedMultigraph, getDegree_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.getDegreeOf(0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getDegreeOf(1), std::out_of_range);
}
