#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "BaseGraph/undirected_multigraph.h"
#include "fixtures.hpp"



TEST(UndirectedMultigraph, addMultiedgeIdx_inexistent_newMultiedge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addMultiedgeIdx(0, 1, 3);
    graph.addMultiedgeIdx(0, 2, 1);
    graph.addEdgeIdx     (0, 0);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 3}, {2, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 3}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    5);
}

TEST(UndirectedMultigraph, addMultiedgeIdx_existent_multiplicityIncremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addEdgeIdx     (0, 2);
    graph.addEdgeIdx     (0, 0);
    graph.addMultiedgeIdx(0, 2, 1);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {2, 2}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 2}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);
}

TEST(UndirectedMultigraph, addMultiedgeIdx_existentSelfLoop_multiplicityIncremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addEdgeIdx     (0, 0);
    graph.addEdgeIdx     (0, 2);
    graph.addMultiedgeIdx(0, 0, 1);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {0, 2}, {2, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);
}

TEST(UndirectedMultigraph, addMultiedgeIdx_existentMultiedgeAndForce_newMultiedge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addEdgeIdx     (0, 2);
    graph.addEdgeIdx     (0, 0);
    graph.addMultiedgeIdx(0, 2, 1, true);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {2, 1}, {0, 1}, {2, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}, {0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 4);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);
}

TEST(UndirectedMultigraph, addMultiedgeIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.addMultiedgeIdx(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.addMultiedgeIdx(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.addMultiedgeIdx(0, 1, 1), std::out_of_range);
}


TEST(UndirectedMultigraph, removeMultiedgeIdx_existentEdgeWithHigherMultiplicity_multiplicityDecremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 2, 3);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 2);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {2, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    3);
}

TEST(UndirectedMultigraph, removeMultiedgeIdx_existentEdgeWithEqualMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 2, 3);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 3);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(UndirectedMultigraph, removeMultiedgeIdx_existentEdgeWithLowerMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 2, 3);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 4);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(UndirectedMultigraph, removeMultiedgeIdx_existentSelfLoopWithHigherMultiplicity_multiplicityDecremented) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 0, 3);
    graph.addEdgeIdx     (0, 2);

    graph.removeMultiedgeIdx(0, 0, 2);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {0, 1}, {2, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    3);
}

TEST(UndirectedMultigraph, removeMultiedgeIdx_existentSelfLoopWithEqualMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 0, 3);
    graph.addEdgeIdx     (0, 2);

    graph.removeMultiedgeIdx(0, 0, 3);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {2, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(UndirectedMultigraph, removeMultiedgeIdx_existentSelfLoopWithLowerMultiplicity_noEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 0, 3);
    graph.addEdgeIdx     (0, 2);

    graph.removeMultiedgeIdx(0, 0, 4);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {2, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(UndirectedMultigraph, removeMultiedgeIdx_inexistentEdge_graphUnchanged) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 4);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{1, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(UndirectedMultigraph, removeMultiedgeIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.removeMultiedgeIdx(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.removeMultiedgeIdx(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.removeMultiedgeIdx(0, 1, 1), std::out_of_range);
}


TEST(UndirectedMultigraph, setEdgeMultiplicityIdx_inexistentEdgeToPositiveMultiplicity_addEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx            (0, 2);
    graph.setEdgeMultiplicityIdx(0, 1, 2);
    graph.addEdgeIdx            (0, 0);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{2, 1}, {1, 2}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 2}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);
}

TEST(UndirectedMultigraph, setEdgeMultiplicityIdx_inexistentEdgeToMultiplicity0_doNothing) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx            (0, 2);
    graph.setEdgeMultiplicityIdx(0, 1, 0);
    graph.addEdgeIdx            (0, 0);


    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{2, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(UndirectedMultigraph, setEdgeMultiplicityIdx_existentEdgeToMultiplicity0_removeEdge) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx            (0, 2, 1);
    graph.addEdgeIdx            (0, 1, 1);
    graph.addEdgeIdx            (0, 0);
    graph.setEdgeMultiplicityIdx(0, 1, 0);

    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{2, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(UndirectedMultigraph, setEdgeMultiplicityIdx_existentEdgeToNonZeroMultiplicity_multiplicityAndEdgeNumberUpdated) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);

    graph.setEdgeMultiplicityIdx(0, 1, 1);
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{2, 1}, {1, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    3);

    graph.setEdgeMultiplicityIdx(0, 1, 2);
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{2, 1}, {1, 2}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 2}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);

    graph.setEdgeMultiplicityIdx(0, 1, 1);
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(0), {{2, 1}, {1, 1}, {0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(1), {{0, 1}});
    EXPECT_NEIGHBOURS_EQ(graph.getOutEdgesOfIdx(2), {{0, 1}});
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    3);
}

TEST(UndirectedMultigraph, setEdgeMultiplicityIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.setEdgeMultiplicityIdx(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.setEdgeMultiplicityIdx(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.setEdgeMultiplicityIdx(0, 1, 1), std::out_of_range);
}

TEST(UndirectedMultigraph, getEdgeMultiplicityIdx_edgeOfMultiplicity2_return2) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx(0, 0);
    graph.addMultiedgeIdx(0, 1, 2);
    graph.addEdgeIdx(1, 2);

    EXPECT_EQ(graph.getEdgeMultiplicityIdx(0, 1), 2);
    EXPECT_EQ(graph.getEdgeMultiplicityIdx({0, 1}), 2);
    EXPECT_EQ(graph.getEdgeMultiplicityIdx(1, 0), 2);
    EXPECT_EQ(graph.getEdgeMultiplicityIdx({1, 0}), 2);
}

TEST(UndirectedMultigraph, getEdgeMultiplicityIdx_inexistentEdge_return0) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addEdgeIdx(0, 0);
    graph.addMultiedgeIdx(0, 1, 2);

    EXPECT_EQ(graph.getEdgeMultiplicityIdx(0, 2), 0);
    EXPECT_EQ(graph.getEdgeMultiplicityIdx({0, 2}), 0);
}

TEST(UndirectedMultigraph, getEdgeMultiplicityIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.getEdgeMultiplicityIdx(0, 0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getEdgeMultiplicityIdx(1, 0), std::out_of_range);
    EXPECT_THROW(graph.getEdgeMultiplicityIdx(0, 1), std::out_of_range);
}

TEST(UndirectedMultigraph, getDegrees_anyGraph_returnCorrectDegrees) {
    BaseGraph::UndirectedMultigraph graph(3);
    graph.addMultiedgeIdx(0, 1, 2);
    graph.addMultiedgeIdx(0, 0, 2);
    graph.addEdgeIdx(1, 0);

    EXPECT_EQ(graph.getDegrees(), std::vector<size_t>({7, 3, 0}) );
    EXPECT_EQ(graph.getDegreeOfIdx(0), 7);
    EXPECT_EQ(graph.getDegreeOfIdx(1), 3);
    EXPECT_EQ(graph.getDegreeOfIdx(2), 0);
}

TEST(UndirectedMultigraph, getDegreeIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::UndirectedMultigraph graph(0);
    EXPECT_THROW(graph.getDegreeOfIdx(0), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.getDegreeOfIdx(1), std::out_of_range);
}
