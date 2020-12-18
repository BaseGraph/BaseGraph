#include <stdexcept>
#include <vector>

#include "gtest/gtest.h"
#include "pgl/vertexlabeled_undirectedgraph.hpp"


using namespace std;

TEST(findVertexIndex, when_vertexAdded_expect_returnsProperIndex){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(3);
    graph.addVertex(2);
    
    EXPECT_EQ(graph.findVertexIndex(1), 0);
    EXPECT_EQ(graph.findVertexIndex(3), 1);
    EXPECT_EQ(graph.findVertexIndex(2), 2);
}

TEST(findVertexIndex, when_vertexDoesntExist_expect_throwLogicError){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    EXPECT_THROW(graph.findVertexIndex(0), logic_error);
    
    graph.addVertex(2);
    EXPECT_THROW(graph.findVertexIndex(1), logic_error);
}

TEST(isVertex, when_addingVertex_expect_returnsTrue){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(3);
    graph.addVertex(2);

    EXPECT_TRUE(graph.isVertex(1));
    EXPECT_TRUE(graph.isVertex(3));
    EXPECT_TRUE(graph.isVertex(2));
}

TEST(getNeighboursOf, when_AEdgelistContainsBAndC_expect_returnVectorWithBAndC){
    PGL::VertexLabeledUndirectedGraph<string> graph;
    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");
    graph.addEdge("A", "B");
    graph.addEdge("C", "A");

    list<string> expectedAdjacentVertices = {"B", "C"};
    const list<string>& returnedAdjacentVertices = graph.getNeighboursOf("A");
    EXPECT_EQ(expectedAdjacentVertices, returnedAdjacentVertices);
}

TEST(isVertex, when_vertexDoesntExist_expect_returnFalse){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    EXPECT_FALSE(graph.isVertex(0));

    graph.addVertex(1);
    EXPECT_FALSE(graph.isVertex(0));
}

TEST(changeVertexIdentifierTo, when_changeVertexName_expect_newNameExistsAndOldNameDoesnt){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(0);
    graph.addVertex(1);
    graph.changeVertexObjectTo(0, 3);
    EXPECT_FALSE(graph.isVertex(0));
    EXPECT_TRUE(graph.isVertex(3));
}

TEST(removeVertexFromEdgeList, when_removeVertex_expect_edgesWithVertexDontExist){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(0);
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addEdge(0, 1);
    graph.addEdge(2, 1);
    graph.addEdge(3, 0);

    graph.removeVertexFromEdgeList(1);
    EXPECT_FALSE(graph.isEdge(1, 2));
    EXPECT_FALSE(graph.isEdge(2, 1));
    EXPECT_FALSE(graph.isEdge(1, 0));
    EXPECT_FALSE(graph.isEdge(0, 1));
    EXPECT_TRUE(graph.isEdge(3, 0));
}

TEST(removeEdge, when_removingEdge_expect_edgeDoesntExist){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(0);
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addEdge(0, 1);
    graph.addEdge(2, 1);

    graph.removeEdge(1, 2);
    EXPECT_TRUE(graph.isEdge(0, 1));
    EXPECT_FALSE(graph.isEdge(1, 2));
    EXPECT_FALSE(graph.isEdge(2, 1));
}

TEST(removeVertexFromEdgeList, when_removingInexistentVertex_expect_throwLogicError){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    EXPECT_THROW(graph.removeVertexFromEdgeList(0), logic_error);
    graph.addVertex(1);
    EXPECT_THROW(graph.removeVertexFromEdgeList(0), logic_error);
}

TEST(removeMultiedges, when_removingMultiedge_expect_multiplicityOf1){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(0);
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addEdge(0, 1);
    graph.addEdge(2, 1);
    graph.addEdge(2, 1, true);
    graph.addEdge(2, 1, true);
    graph.addEdge(3, 1);
    graph.addEdge(0, 1, true);
    graph.removeMultiedges();

    EXPECT_EQ(graph.getNeighboursOf(1), list<int>({0, 2, 3}));
}


TEST(isEdge, when_addingEdge_expect_returnsTrueInBothDirections){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addEdge(1, 2);

    EXPECT_TRUE(graph.isEdge(1, 2));
    EXPECT_TRUE(graph.isEdge(2, 1));
}

TEST(isEdge, when_edgeDoesntExist_expect_returnsFalse){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    EXPECT_FALSE(graph.isEdge(1, 2));
    EXPECT_FALSE(graph.isEdge(2, 1));
}

TEST(ComparisonOperator, when_comparingTwoEmptyGraphs_expect_true){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    PGL::VertexLabeledUndirectedGraph<int> graph2;
    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingDifferentNumberOfVerticesGraphs_expect_false){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    PGL::VertexLabeledUndirectedGraph<int> graph2;
    graph.addVertex(1);
    graph.addVertex(2);
    graph2.addVertex(1);
    graph2.addVertex(2);
    graph2.addVertex(3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingDifferentVertexOrderOfSameGraph_expect_true){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    PGL::VertexLabeledUndirectedGraph<int> graph2;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addEdge(1, 3);
    graph.addEdge(1, 2);

    graph2.addVertex(3);
    graph2.addVertex(1);
    graph2.addVertex(2);
    graph2.addEdge(1, 3);
    graph2.addEdge(1, 2);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingDifferentEdgeOrderOfSameGraph_expect_true){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    PGL::VertexLabeledUndirectedGraph<int> graph2;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addEdge(1, 3);
    graph.addEdge(1, 2);

    graph2.addVertex(3);
    graph2.addVertex(1);
    graph2.addVertex(2);
    graph2.addEdge(1, 2);
    graph2.addEdge(1, 3);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithDifferentVertices_expect_false){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    PGL::VertexLabeledUndirectedGraph<int> graph2;
    graph.addVertex(1);
    graph.addVertex(4);
    graph.addVertex(3);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);

    graph2.addVertex(3);
    graph2.addVertex(1);
    graph2.addVertex(2);
    graph2.addEdge(1, 2);
    graph2.addEdge(1, 3);

    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithDifferentEdges_expect_false){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    PGL::VertexLabeledUndirectedGraph<int> graph2;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);

    graph2.addVertex(1);
    graph2.addVertex(2);
    graph2.addVertex(3);
    graph2.addEdge(1, 3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_ComparisonOperatorReturnTrue){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2);
    graph.addEdge(3, 1);

    PGL::VertexLabeledUndirectedGraph<int> copiedNetwork(graph);
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared in the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::VertexLabeledUndirectedGraph<int>* graph = new PGL::VertexLabeledUndirectedGraph<int>(); 
    graph->addVertex(1);
    graph->addVertex(2);
    graph->addVertex(3);
    graph->addVertex(4);
    graph->addEdge(1, 2);
    graph->addEdge(3, 1);

    PGL::VertexLabeledUndirectedGraph<int> copiedNetwork(*graph);
    delete graph;

    EXPECT_TRUE(copiedNetwork.isVertex(1));
    EXPECT_TRUE(copiedNetwork.isVertex(2));
    EXPECT_TRUE(copiedNetwork.isVertex(3));
    EXPECT_TRUE(copiedNetwork.isVertex(4));

    EXPECT_TRUE(copiedNetwork.isEdge(1, 2));
    EXPECT_TRUE(copiedNetwork.isEdge(2, 1));
    EXPECT_TRUE(copiedNetwork.isEdge(3, 1));
    EXPECT_TRUE(copiedNetwork.isEdge(1, 3));
}

TEST(CopyConstructorFromBase, when_copyGraphFromBaseClass_expect_hasSameEdges){
    PGL::UndirectedGraph graph(4);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);

    PGL::VertexLabeledUndirectedGraph<int> templateCopy(graph, std::vector<int>({0, 1, 2, 3}));
    EXPECT_TRUE(templateCopy.isEdgeIdx(1, 2));
    EXPECT_TRUE(templateCopy.isEdgeIdx(2, 1));
    EXPECT_TRUE(templateCopy.isEdgeIdx(1, 3));
    EXPECT_TRUE(templateCopy.isEdgeIdx(3, 1));
}

TEST(AssignementOperator, when_copyGraph_expect_ComparisonOperatorReturnTrue){
    PGL::VertexLabeledUndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2);
    graph.addEdge(3, 1);

    PGL::VertexLabeledUndirectedGraph<int> copiedNetwork = graph;
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(AssignementOperator, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared in the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::VertexLabeledUndirectedGraph<int>* graph = new PGL::VertexLabeledUndirectedGraph<int>(); 
    graph->addVertex(1);
    graph->addVertex(2);
    graph->addVertex(3);
    graph->addVertex(4);
    graph->addEdge(1, 2);
    graph->addEdge(3, 1);

    PGL::VertexLabeledUndirectedGraph<int> copiedNetwork;
    copiedNetwork = *graph;
    delete graph;

    EXPECT_TRUE(copiedNetwork.isVertex(1));
    EXPECT_TRUE(copiedNetwork.isVertex(2));
    EXPECT_TRUE(copiedNetwork.isVertex(3));
    EXPECT_TRUE(copiedNetwork.isVertex(4));

    EXPECT_TRUE(copiedNetwork.isEdge(1, 2));
    EXPECT_TRUE(copiedNetwork.isEdge(2, 1));
    EXPECT_TRUE(copiedNetwork.isEdge(3, 1));
    EXPECT_TRUE(copiedNetwork.isEdge(1, 3));
}
