#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "pgl/undirectedgraph.h"


using namespace std;


TEST(isEdgeIdx, when_addEdge_expect_isEdgeReturnsTrueInBothDirections){
    PGL::UndirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(3, 2);
    EXPECT_TRUE(graph.isEdgeIdx(0, 1));
    EXPECT_TRUE(graph.isEdgeIdx(1, 0));
    EXPECT_TRUE(graph.isEdgeIdx(3, 2));
    EXPECT_TRUE(graph.isEdgeIdx(2, 3));
}

TEST(addEdgeIdx, when_addingEdge_expect_edgeNumberIncrementsBy1){
    PGL::UndirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(removeEdgeIdx, when_removingEdge_expect_edgeDoesntExistInBothDirections){
    PGL::UndirectedGraph graph;
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 1);

    graph.removeEdgeIdx(1, 2);
    EXPECT_TRUE(graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(graph.isEdgeIdx(1, 2));
    EXPECT_FALSE(graph.isEdgeIdx(2, 1));
}

TEST(removeEdgeIdx, when_removingEdge_expect_edgeNumberDecrementsBy1) {
    PGL::UndirectedGraph graph;
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);
    graph.removeEdgeIdx(0, 1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(removeMultiedges, when_removingMultiedge_expect_edgeNumberToDecreaseByMultiplicityMinus1) {
    PGL::UndirectedGraph graph(5);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2, true);
    graph.addEdgeIdx(2, 1, true);

    graph.removeMultiedges();
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(removeEdgeIdx, when_removingInexistentEdge_expect_edgeNumberUnchanged) {
    PGL::UndirectedGraph graph;
    graph.addEdgeIdx(0, 2);
    graph.removeEdgeIdx(0, 1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(removeVertexFromEdgeListIdx, when_edgeExistFromAndToVertex_expect_edgeNumberDecreases){
    PGL::UndirectedGraph graph(5);
    graph.addEdgeIdx(3, 4);

    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 1, true);
    graph.addEdgeIdx(0, 1);
    graph.removeVertexFromEdgeListIdx(1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(ComparisonOperator, when_comparingTwoEmptyGraphs_expect_true){
    PGL::UndirectedGraph graph;
    PGL::UndirectedGraph graph2;
    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingDifferentNumberOfVerticesGraphs_expect_false){
    PGL::UndirectedGraph graph(3);
    PGL::UndirectedGraph graph2(2);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingDifferentEdgeOrderOfSameGraph_expect_true){
    PGL::UndirectedGraph graph;
    PGL::UndirectedGraph graph2;
    graph.addEdgeIdx(1, 3);
    graph.addEdgeIdx(1, 2);

    graph2.addEdgeIdx(1, 2);
    graph2.addEdgeIdx(1, 3);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithAMissingEdge_expect_false){
    PGL::UndirectedGraph graph;
    PGL::UndirectedGraph graph2;
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 3);

    graph2.addEdgeIdx(1, 3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithDifferentEdges_expect_false){
    PGL::UndirectedGraph graph;
    PGL::UndirectedGraph graph2;
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 3);

    graph2.addEdgeIdx(1, 3);
    graph2.addEdgeIdx(2, 3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_ComparisonOperatorReturnTrue){
    PGL::UndirectedGraph graph;
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);

    PGL::UndirectedGraph copiedNetwork(graph);
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared on the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::UndirectedGraph* graph = new PGL::UndirectedGraph();
    graph->addEdgeIdx(1, 2);
    graph->addEdgeIdx(3, 1);

    PGL::UndirectedGraph copiedNetwork(*graph);
    delete graph;

    EXPECT_TRUE(copiedNetwork.isEdgeIdx(1, 2));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(2, 1));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(3, 1));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(1, 3));
}

TEST(AssignementOperator, when_copyGraph_expect_ComparisonOperatorReturnTrue){
    PGL::UndirectedGraph graph;

    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);

    PGL::UndirectedGraph copiedNetwork = graph;
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(AssignementOperator, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared on the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::UndirectedGraph* graph = new PGL::UndirectedGraph();
    graph->addEdgeIdx(1, 2);
    graph->addEdgeIdx(3, 1);

    PGL::UndirectedGraph copiedNetwork;
    copiedNetwork = *graph;
    delete graph;

    EXPECT_TRUE(copiedNetwork.isEdgeIdx(1, 2));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(2, 1));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(3, 1));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(1, 3));
}
