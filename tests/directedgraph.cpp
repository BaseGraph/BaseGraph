#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "pgl/directedgraph.h"


using namespace std;


TEST(isEdgeIdx, when_addEdge_expect_returnTrueInOneDirection){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(3, 2);
    EXPECT_TRUE(graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(graph.isEdgeIdx(1, 0));
    EXPECT_TRUE(graph.isEdgeIdx(3, 2));
    EXPECT_FALSE(graph.isEdgeIdx(2, 3));
}

TEST(isEdgeIdx, when_edgeDoesntExist_expect_returnsFalse){
    PGL::DirectedGraph graph(5);
    EXPECT_FALSE(graph.isEdgeIdx(0, 2));
    graph.addEdgeIdx(0, 1);
    EXPECT_FALSE(graph.isEdgeIdx(0, 2));
}


TEST(getOutEdgesOfIdx, when_vertexHas123outNeighbours_expect_return123){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 3);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), list<size_t>({1,2,3}));
}

TEST(getInEdgesOfVertices, expect_correct_inEdges) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 3);

    EXPECT_EQ(graph.getInEdgesOfVertices(), vector<list<size_t>>({{}, {}, {1}, {0, 2}, {}}));
}

TEST(addEdgeIdx, when_addingEdges_expect_edgeNumberIncrements) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
    graph.addEdgeIdx(1, 2);
    EXPECT_EQ(graph.getEdgeNumber(), 2);
}

TEST(addEdgeIdx, when_addingExistingEdge_expect_edgeNumberUnchanged) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(removeEdgeIdx, when_removingEdge_expect_edgeNumberDecrements) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.removeEdgeIdx(0, 1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
    graph.removeEdgeIdx(0, 2);
    EXPECT_EQ(graph.getEdgeNumber(), 0);
}

TEST(removeEdgeIdx, when_removingNonExistentEdges_expect_edgeNumberUnchanged){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.removeEdgeIdx(0, 2);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(removeMultiedges, when_removingMultiedge_expect_edgeNumberToDecreaseByMultiplicityMinus1) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2, true);
    graph.addEdgeIdx(1, 3);
    graph.addEdgeIdx(1, 2, true);
    graph.removeMultiedges();
    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), list<size_t> ({2, 3}));
}

TEST(removeSelfLoops, when_selfloops_expect_selfloopsDisappearAndEdgeNumberAdjusted) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(2, 2, true);
    graph.addEdgeIdx(3, 0, true);
    graph.addEdgeIdx(3, 3, true);
    graph.addEdgeIdx(3, 4, true);
    graph.removeSelfLoops();

    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getOutEdgesOfIdx(3), list<size_t>({0, 4}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(2), list<size_t>({}));
}

TEST(removeVertexFromEdgeListIdx, when_edgeExistFromAndToVertex_expect_edgeNumberDecreases){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(3, 4);

    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2, true);
    graph.addEdgeIdx(0, 1);
    graph.removeVertexFromEdgeListIdx(1);
    EXPECT_EQ(graph.getEdgeNumber(), 1);
}

TEST(removeVertexFromEdgeListIdx, when_edgelistContainsVertexAndVertexIsRemoved_expect_emptyNeighbourhoodWithoutVertex){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(1, 2);

    graph.removeVertexFromEdgeListIdx(2);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), list<size_t>({1, 3}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), list<size_t>());
}


TEST(removeEdgeIdx, when_removingEdge_expect_edgeDoesntExist){
    PGL::DirectedGraph graph;
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 1);
    graph.addEdgeIdx(1, 2);

    graph.removeEdgeIdx(1, 2);
    EXPECT_FALSE(graph.isEdgeIdx(1, 2));

    EXPECT_TRUE(graph.isEdgeIdx(0, 1));
    EXPECT_TRUE(graph.isEdgeIdx(2, 1));
}


TEST(removeMultiedges, when_removingMultiedge_expect_multiplicityOf1){
    PGL::DirectedGraph graph;
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2, true);
    graph.addEdgeIdx(1, 2, true);
    graph.addEdgeIdx(1, 3);
    graph.addEdgeIdx(1, 0, true);
    graph.removeMultiedges();

    EXPECT_EQ(graph.getOutEdgesOfIdx(1), list<size_t>({0, 2, 3}));
}

TEST(getAdjacencyMatrix, when_gettingAdjacencyMatrix_expect_correctAdjacencyMatrix){
    PGL::DirectedGraph graph(3);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 1);
    graph.addEdgeIdx(2, 1, 1);

    vector<vector<size_t>> expectedAdjacencyMatrix({vector<size_t>({0, 1, 0}),
                                                    vector<size_t>({0, 0, 1}),
                                                    vector<size_t>({0, 2, 0})});
    EXPECT_EQ(graph.getAdjacencyMatrix(), expectedAdjacencyMatrix);
}

TEST(getInDegreeIdx, when_vertexWith3InDegree_expect_return3){
    PGL::DirectedGraph graph;
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(4, 0);

    EXPECT_EQ(graph.getInDegreeIdx(0), 3);
}

TEST(getInDegrees, expect_everyElementEqualsGetInDegreeIdx) {
    PGL::DirectedGraph graph;
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(4, 0);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 5);

    auto inDegrees = graph.getInDegrees();
    for (size_t i=0; i<graph.getSize(); i++)
        EXPECT_EQ(inDegrees[i], graph.getInDegreeIdx(i));
}

TEST(getOutDegreeIdx, when_vertexWith3OutDegree_expect_return3){
    PGL::DirectedGraph graph;
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 3);

    EXPECT_EQ(graph.getOutDegreeIdx(0), 3);
}

TEST(getOutDegrees, expect_everyElementEqualsGetOutDegreeIdx) {
    PGL::DirectedGraph graph;
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(4, 0);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 5);

    auto outDegrees = graph.getOutDegrees();
    for (size_t i=0; i<graph.getSize(); i++)
        EXPECT_EQ(outDegrees[i], graph.getOutDegreeIdx(i));
}


TEST(ComparisonOperator, when_comparingTwoEmptyGraphs_expect_true){
    PGL::DirectedGraph graph;
    PGL::DirectedGraph graph2;
    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingDifferentNumberOfVerticesGraphs_expect_false){
    PGL::DirectedGraph graph(3);
    PGL::DirectedGraph graph2(2);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingDifferentEdgeOrderOfSameGraph_expect_true){
    PGL::DirectedGraph graph;
    PGL::DirectedGraph graph2;
    graph.addEdgeIdx(1, 3);
    graph.addEdgeIdx(1, 2);

    graph2.addEdgeIdx(1, 2);
    graph2.addEdgeIdx(1, 3);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithAMissingEdge_expect_false){
    PGL::DirectedGraph graph;
    PGL::DirectedGraph graph2;
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 3);

    graph2.addEdgeIdx(1, 3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithDifferentEdges_expect_false){
    PGL::DirectedGraph graph;
    PGL::DirectedGraph graph2;
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 3);

    graph2.addEdgeIdx(1, 3);
    graph2.addEdgeIdx(2, 3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_ComparisonOperatorReturnTrue){
    PGL::DirectedGraph graph;
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);

    PGL::DirectedGraph copiedNetwork(graph);
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared in the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::DirectedGraph* graph = new PGL::DirectedGraph(); 
    graph->addEdgeIdx(1, 2);
    graph->addEdgeIdx(3, 1);

    PGL::DirectedGraph copiedNetwork(*graph);
    delete graph;

    EXPECT_TRUE(copiedNetwork.isEdgeIdx(1, 2));
    EXPECT_FALSE(copiedNetwork.isEdgeIdx(2, 1));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(3, 1));
    EXPECT_FALSE(copiedNetwork.isEdgeIdx(1, 3));
}

TEST(AssignementOperator, when_copyGraph_expect_ComparisonOperatorReturnTrue){
    PGL::DirectedGraph graph;

    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);

    PGL::DirectedGraph copiedNetwork = graph;
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(AssignementOperator, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared in the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::DirectedGraph* graph = new PGL::DirectedGraph(); 
    graph->addEdgeIdx(1, 2);
    graph->addEdgeIdx(3, 1);

    PGL::DirectedGraph copiedNetwork;
    copiedNetwork = *graph;
    delete graph;

    EXPECT_TRUE(copiedNetwork.isEdgeIdx(1, 2));
    EXPECT_FALSE(copiedNetwork.isEdgeIdx(2, 1));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(3, 1));
    EXPECT_FALSE(copiedNetwork.isEdgeIdx(1, 3));
}

TEST(RangedBasedFor, expect_returns_vertices){
    PGL::DirectedGraph graph(10);
    list<size_t> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<size_t> loopVertices;

    for(size_t& vertex: graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}

TEST(Iterator, when_postAndPreIncrement_expect_loopsCorrectly) {
    PGL::DirectedGraph graph(10);
    list<size_t> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<size_t> loopVertices;

    for(PGL::DirectedGraph::iterator it=graph.begin(); it!=graph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for(PGL::DirectedGraph::iterator it=graph.begin(); it!=graph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}
