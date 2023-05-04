#include <deque>
#include <list>
#include <queue>
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2, 0}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2, 0}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2, 0, 2}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 2, 0}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 0}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 0}));
    EXPECT_EQ(graph.getEdgeMultiplicity(0, 2), 0);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(), 2);
}

TEST(DirectedMultigraph, removeMultiedge_inexistentEdge_graphUnchanged) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(0, 0);

    graph.removeMultiedge(0, 2, 4);

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({1, 0}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 1, 0}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 0}));
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

    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 0}));
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
    EXPECT_EQ(graph.getOutNeighbours(0), BaseGraph::Successors({2, 1, 0}));
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
    EXPECT_EQ(graph.getEdgeMultiplicity(1, 0), 0);
}

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
    EXPECT_EQ(graph.getOutDegree(0), 3);
    EXPECT_EQ(graph.getOutDegree(1), 1);
    EXPECT_EQ(graph.getOutDegree(2), 0);
}

TEST(DirectedMultigraph, getOutDegree_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.getOutDegree(0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getOutDegree(1), std::out_of_range);
}

TEST(DirectedMultigraph, getInDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdge(1, 0);
    graph.addEdge(0, 0);
    graph.addMultiedge(1, 0, 2);
    graph.addEdge(0, 1);

    EXPECT_EQ(graph.getInDegrees(), std::vector<size_t>({4, 1, 0}));
    EXPECT_EQ(graph.getInDegree(0), 4);
    EXPECT_EQ(graph.getInDegree(1), 1);
    EXPECT_EQ(graph.getInDegree(2), 0);
}

TEST(DirectedMultigraph, getDegree_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);

    EXPECT_THROW(graph.getOutDegree(1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getOutDegree(1), std::out_of_range);
}

const std::vector<int> multiplicities = {1, 2, 3, 10, 100};

TEST(DirectedMultigraph, addReciprocalEdge_validEdge_totalEdgeNumberUpdated) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addReciprocalMultiedge(0, 1, multiplicities[0]);

    EXPECT_EQ(graph.getTotalEdgeNumber(),
              multiplicities[0] + multiplicities[0]);
}

template <template <class...> class Container, class... Args, typename Labels>
static void testCorrectTotalEdgeNumberForContainer(Labels &labels) {
    Container<BaseGraph::LabeledEdge<BaseGraph::EdgeMultiplicity>> edges = {
        {0, 2, labels[0]},
        {0, 1, labels[1]},
        {0, 0, labels[2]},
        {10, 5, labels[3]}};
    BaseGraph::DirectedMultigraph graph(edges);
    EXPECT_EQ(graph.getTotalEdgeNumber(),
              labels[0] + labels[1] + labels[2] + labels[3]);
}

TEST(DirectedMultigraph, edgeListConstructor_anyContainer_allEdgesExist) {
    testCorrectTotalEdgeNumberForContainer<std::vector>(multiplicities);
    testCorrectTotalEdgeNumberForContainer<std::list>(multiplicities);
    testCorrectTotalEdgeNumberForContainer<std::set>(multiplicities);
    testCorrectTotalEdgeNumberForContainer<std::deque>(multiplicities);
}

TEST(DirectedMultigraph, removeDuplicateEdges_noMultiedge_doNothing) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addMultiedge(0, 1, multiplicities[0]);
    graph.addMultiedge(0, 2, multiplicities[1]);
    graph.addMultiedge(1, 1, multiplicities[2]);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalEdgeNumber(),
              multiplicities[0] + multiplicities[1] + multiplicities[2]);
}

TEST(DirectedMultigraph,
     removeDuplicateEdges_multiedge_totalEdgeNumberUpdated) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addMultiedge(0, 1, multiplicities[0]);
    graph.addMultiedge(0, 2, multiplicities[1]);
    graph.addMultiedge(0, 1, multiplicities[0], true);
    graph.addMultiedge(0, 1, multiplicities[0], true);
    graph.addMultiedge(1, 1, multiplicities[2]);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalEdgeNumber(),
              multiplicities[0] + multiplicities[1] + multiplicities[2]);
}

TEST(DirectedMultigraph,
     removeDuplicateEdges_multiSelfLoop_totalEdgeNumberUpdated) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addMultiedge(0, 1, multiplicities[0]);
    graph.addMultiedge(1, 1, multiplicities[1]);
    graph.addMultiedge(1, 1, multiplicities[1], true);
    graph.addMultiedge(1, 2, multiplicities[2]);
    graph.addMultiedge(1, 1, multiplicities[1], true);

    graph.removeDuplicateEdges();

    EXPECT_EQ(graph.getTotalEdgeNumber(),
              multiplicities[0] + multiplicities[1] + multiplicities[2]);
}

TEST(DirectedMultigraph, removeSelfLoops_noSelfLoop_doNothing) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addMultiedge(0, 1, multiplicities[0]);
    graph.addMultiedge(0, 2, multiplicities[1]);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getTotalEdgeNumber(),
              multiplicities[0] + multiplicities[1]);
}

TEST(DirectedMultigraph,
     removeSelfLoops_existentSelfLoop_totalEdgeNumberUpdated) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addMultiedge(0, 1, multiplicities[0]);
    graph.addMultiedge(0, 2, multiplicities[1]);
    graph.addMultiedge(0, 0, multiplicities[2]);

    graph.removeSelfLoops();

    EXPECT_EQ(graph.getTotalEdgeNumber(),
              multiplicities[0] + multiplicities[1]);
}

TEST(DirectedMultigraph,
     removeVertexFromEdgeList_vertexInEdges_totalEdgeNumberUpdated) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addMultiedge(0, 1, multiplicities[0]);
    graph.addMultiedge(0, 0, multiplicities[1]);
    graph.addMultiedge(1, 2, multiplicities[2]);
    graph.addMultiedge(1, 0, multiplicities[3]);
    graph.addMultiedge(1, 0, multiplicities[3], true);
    graph.addMultiedge(1, 3, multiplicities[4]);

    graph.removeVertexFromEdgeList(0);

    EXPECT_EQ(graph.getTotalEdgeNumber(),
              multiplicities[2] + multiplicities[4]);
}

TEST(DirectedMultigraph, clearEdges_anyGraph_totalNumberEdgeNumberIs0) {
    BaseGraph::DirectedMultigraph graph(multiplicities.size());
    graph.addMultiedge(0, 1, multiplicities[0]);
    graph.addMultiedge(0, 0, multiplicities[1]);
    graph.addMultiedge(1, 2, multiplicities[2]);
    graph.addMultiedge(1, 0, multiplicities[3]);

    graph.clearEdges();
    EXPECT_EQ(graph.getTotalEdgeNumber(), 0);
}
