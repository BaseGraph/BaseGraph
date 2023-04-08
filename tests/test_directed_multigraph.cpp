#include <list>
#include <stdexcept>

#include "BaseGraph/directed_multigraph.hpp"
#include "BaseGraph/types.h"
#include "fixtures.hpp"
#include "gtest/gtest.h"

TEST(DirectedMultigraph, addMultiedge_inexistent_newMultiedge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addMultiedge(0, 1, 3);
    graph.addMultiedge(0, 2, 1);
    graph.addEdge(0, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 3);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 5);
}

TEST(DirectedMultigraph, addMultiedge_existent_multiplicityIncremented) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 2, 1);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 2);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 0), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);
}

TEST(DirectedMultigraph,
     addMultiedge_existentMultiedgeAndForce_duplicateMultiedge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 2, 1, true);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0, 2}));
    EXPECT_EQ(graph.getEdgeNumber(), 4);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);
}

TEST(DirectedMultigraph, addMultiedge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.addMultiedge(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.addMultiedge(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.addMultiedge(0, 1, 1), std::out_of_range);
}

TEST(
    DirectedMultigraph,
    removeMultiedge_existentEdgeWithHigherMultiplicity_multiplicityDecremented) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 2, 3);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 2);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 2, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 1);
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 1);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 3);
}

TEST(DirectedMultigraph,
     removeMultiedge_existentEdgeWithEqualMultiplicity_noEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 2, 3);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 3);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(DirectedMultigraph,
     removeMultiedge_existentEdgeWithLowerMultiplicity_noEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addMultiedge(0, 2, 3);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 4);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(DirectedMultigraph, removeMultiedge_inexistentEdge_graphUnchanged) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 4);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(DirectedMultigraph, removeMultiedge_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.removeMultiedge(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.removeMultiedge(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.removeMultiedge(0, 1, 1), std::out_of_range);
}

TEST(DirectedMultigraph,
     setEdgeMultiplicity_inexistentEdgeToPositiveMultiplicity_addEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 2);
    graph.setEdgeMultiplicity(0, 1, 2);
    graph.addEdge(0, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({2, 1, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 2);
    EXPECT_EQ(graph.getEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 4);
}

TEST(DirectedMultigraph,
     setEdgeMultiplicity_inexistentEdgeToMultiplicity0_doNothing) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 2);
    graph.setEdgeMultiplicity(0, 1, 0);
    graph.addEdge(0, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({2, 0}));
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(DirectedMultigraph,
     setEdgeMultiplicity_existentEdgeToMultiplicity0_removeEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 2, 1);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 0);
    graph.setEdgeMultiplicity(0, 1, 0);

    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({2, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(
    DirectedMultigraph,
    setEdgeMultiplicity_existentEdgeToNonZeroMultiplicity_multiplicityAndEdgeNumberUpdated) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 2);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);

    graph.setEdgeMultiplicity(0, 1, 1);
    EXPECT_EQ(graph.getOutEdgesOf(0), BaseGraph::Successors({2, 1, 0}));
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

TEST(DirectedMultigraph, setEdgeMultiplicity_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.setEdgeMultiplicity(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.setEdgeMultiplicity(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.setEdgeMultiplicity(0, 1, 1), std::out_of_range);
}

/* Assumed to work
TEST(DirectedMultigraph, getEdgeMultiplicity_edgeOfMultiplicity2_return2) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 1, 2);

    EXPECT_EQ(graph.getEdgeMultiplicity(0, 1), 2);
}

TEST(DirectedMultigraph, getEdgeMultiplicity_inexistentEdge_return0) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 0);
    graph.addMultiedge(0, 1, 2);

    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 0);
    EXPECT_EQ(graph.getEdgeMultiplicity({0, 2}), 0);
    EXPECT_EQ(graph.getEdgeMultiplicity(1, 0), 0);
    EXPECT_EQ(graph.getEdgeMultiplicity({1, 0}), 0);
}
*/

TEST(DirectedMultigraph, getEdgeMultiplicity_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.getEdgeMultiplicity(0, 0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getEdgeMultiplicity(1, 0), std::out_of_range);
    EXPECT_THROW(graph.getEdgeMultiplicity(0, 1), std::out_of_range);
}

TEST(DirectedMultigraph,
     getAdjacencyMatrix_anyGraph_returnCorrectMultiplicities) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addMultiedge(0, 1, 2);
    graph.addEdge(0, 0);
    graph.addEdge(1, 0);

    EXPECT_EQ(graph.getAdjacencyMatrix(),
              BaseGraph::AdjacencyMatrix({{1, 2, 0}, {1, 0, 0}, {0, 0, 0}}));
}

TEST(DirectedMultigraph, getOutDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addMultiedge(0, 1, 2);
    graph.addEdge(0, 0);
    graph.addEdge(1, 0);

    EXPECT_EQ(graph.getOutDegrees(), std::vector<size_t>({3, 1, 0}));
    EXPECT_EQ(graph.getOutDegreeOf(0), 3);
    EXPECT_EQ(graph.getOutDegreeOf(1), 1);
    EXPECT_EQ(graph.getOutDegreeOf(2), 0);
}

TEST(DirectedMultigraph, getOutDegree_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.getOutDegreeOf(0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getOutDegreeOf(1), std::out_of_range);
}

TEST(DirectedMultigraph, getInDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(1, 0);
    graph.addEdge(0, 0);
    graph.addMultiedge(1, 0, 2);
    graph.addEdge(0, 1);

    EXPECT_EQ(graph.getInDegrees(), std::vector<size_t>({4, 1, 0}));
    EXPECT_EQ(graph.getInDegreeOf(0), 4);
    EXPECT_EQ(graph.getInDegreeOf(1), 1);
    EXPECT_EQ(graph.getInDegreeOf(2), 0);
}

TEST(DirectedMultigraph, getDegree_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);

    EXPECT_THROW(graph.getOutDegreeOf(1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getOutDegreeOf(1), std::out_of_range);
}
