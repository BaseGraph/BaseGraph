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

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), list<PGL::VertexIndex>({1,2,3}));
}

TEST(getInEdgesOfVertices, expect_correct_inEdges) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 3);

    EXPECT_EQ(graph.getInEdgesOfVertices(), vector<list<PGL::VertexIndex>>({{}, {}, {1}, {0, 2}, {}}));
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
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), list<PGL::VertexIndex> ({2, 3}));
}

TEST(removeSelfLoops, when_selfloops_expect_selfloopsDisappearAndEdgeNumberAdjusted) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(2, 2, true);
    graph.addEdgeIdx(3, 0, true);
    graph.addEdgeIdx(3, 3, true);
    graph.addEdgeIdx(3, 4, true);
    graph.removeSelfLoops();

    EXPECT_EQ(graph.getEdgeNumber(), 2);
    EXPECT_EQ(graph.getOutEdgesOfIdx(3), list<PGL::VertexIndex>({0, 4}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(2), list<PGL::VertexIndex>({}));
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

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), list<PGL::VertexIndex>({1, 3}));
    EXPECT_EQ(graph.getOutEdgesOfIdx(1), list<PGL::VertexIndex>());
}


TEST(removeEdgeIdx, when_removingEdge_expect_edgeDoesntExist){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 1);
    graph.addEdgeIdx(1, 2);

    graph.removeEdgeIdx(1, 2);
    EXPECT_FALSE(graph.isEdgeIdx(1, 2));

    EXPECT_TRUE(graph.isEdgeIdx(0, 1));
    EXPECT_TRUE(graph.isEdgeIdx(2, 1));
}


TEST(removeMultiedges, when_removingMultiedge_expect_multiplicityOf1){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 2, true);
    graph.addEdgeIdx(1, 2, true);
    graph.addEdgeIdx(1, 3);
    graph.addEdgeIdx(1, 0, true);
    graph.removeMultiedges();

    EXPECT_EQ(graph.getOutEdgesOfIdx(1), list<PGL::VertexIndex>({0, 2, 3}));
}

TEST(clear, when_clearGraph_expect_noEdge) {
    PGL::DirectedGraph graph(4);
    graph.addEdgeIdx(0, 1);
    graph.addReciprocalEdgeIdx(2, 3);

    graph.clear();
    EXPECT_EQ(graph.getEdgeNumber(), 0);
    EXPECT_FALSE(graph.isEdgeIdx(0, 1));
    EXPECT_FALSE(graph.isEdgeIdx(2, 3));
    EXPECT_FALSE(graph.isEdgeIdx(3, 2));
}

TEST(getSubgraph, when_getSubgraphWithoutRemap_expect_containsOnlyInsideEdges) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addReciprocalEdgeIdx(2, 1);
    graph.addEdgeIdx(2, 3);
    graph.addReciprocalEdgeIdx(0, 3);

    auto subgraph = graph.getSubgraph({0, 2, 3});
    EXPECT_FALSE(subgraph.isEdgeIdx(0, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(2, 1));
    EXPECT_FALSE(subgraph.isEdgeIdx(1, 2));
    EXPECT_TRUE(subgraph.isEdgeIdx(2, 3));
    EXPECT_TRUE(subgraph.isEdgeIdx(0, 3));
    EXPECT_TRUE(subgraph.isEdgeIdx(3, 0));
    EXPECT_EQ(subgraph.getEdgeNumber(), 3);
}

TEST(getSubgraph, when_getSubgraphWithRemap_expect_containsOnlyInsideEdgesAndIsResized) {
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addReciprocalEdgeIdx(2, 1);
    graph.addEdgeIdx(2, 3);
    graph.addReciprocalEdgeIdx(0, 3);

    auto subgraph_remap = graph.getSubgraphWithRemap({0, 2, 3});
    auto& subgraph = subgraph_remap.first;
    auto& remap = subgraph_remap.second;

    EXPECT_EQ(subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.isEdgeIdx(remap[3], remap[0]));
    EXPECT_EQ(subgraph.getEdgeNumber(), 3);
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
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(4, 0);

    EXPECT_EQ(graph.getInDegreeIdx(0), 3);
}

TEST(getInDegrees, expect_everyElementEqualsGetInDegreeIdx) {
    PGL::DirectedGraph graph(6);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(4, 0);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 5);

    auto inDegrees = graph.getInDegrees();
    for (PGL::VertexIndex i: graph)
        EXPECT_EQ(inDegrees[i], graph.getInDegreeIdx(i));
}

TEST(getOutDegreeIdx, when_vertexWith3OutDegree_expect_return3){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 3);

    EXPECT_EQ(graph.getOutDegreeIdx(0), 3);
}

TEST(getOutDegrees, expect_everyElementEqualsGetOutDegreeIdx) {
    PGL::DirectedGraph graph(6);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(4, 0);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 5);

    auto outDegrees = graph.getOutDegrees();
    for (PGL::VertexIndex i: graph)
        EXPECT_EQ(outDegrees[i], graph.getOutDegreeIdx(i));
}


TEST(ComparisonOperator, when_comparingTwoEmptyGraphs_expect_true){
    PGL::DirectedGraph graph(5);
    PGL::DirectedGraph graph2(5);
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
    PGL::DirectedGraph graph(5);
    PGL::DirectedGraph graph2(5);
    graph.addEdgeIdx(1, 3);
    graph.addEdgeIdx(1, 2);

    graph2.addEdgeIdx(1, 2);
    graph2.addEdgeIdx(1, 3);

    EXPECT_TRUE(graph == graph2);
    EXPECT_TRUE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithAMissingEdge_expect_false){
    PGL::DirectedGraph graph(5);
    PGL::DirectedGraph graph2(5);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 3);

    graph2.addEdgeIdx(1, 3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(ComparisonOperator, when_comparingGraphsWithDifferentEdges_expect_false){
    PGL::DirectedGraph graph(5);
    PGL::DirectedGraph graph2(5);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(1, 3);

    graph2.addEdgeIdx(1, 3);
    graph2.addEdgeIdx(2, 3);
    EXPECT_FALSE(graph == graph2);
    EXPECT_FALSE(graph2 == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_ComparisonOperatorReturnTrue){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);

    PGL::DirectedGraph copiedNetwork(graph);
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(CopyConstructor, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared in the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::DirectedGraph* graph = new PGL::DirectedGraph(5);
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
    PGL::DirectedGraph graph(5);

    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);

    PGL::DirectedGraph copiedNetwork = graph;
    EXPECT_TRUE(copiedNetwork == graph);
}

TEST(AssignementOperator, when_copyGraph_expect_validObjectAfterDestructionOfSource){
    /* Source graph declared in the heap because otherwise google test make a second call to the destructor
     * at the end of the test
     */
    PGL::DirectedGraph* graph = new PGL::DirectedGraph(5);
    graph->addEdgeIdx(1, 2);
    graph->addEdgeIdx(3, 1);

    PGL::DirectedGraph copiedNetwork(1);
    copiedNetwork = *graph;
    delete graph;

    EXPECT_TRUE(copiedNetwork.isEdgeIdx(1, 2));
    EXPECT_FALSE(copiedNetwork.isEdgeIdx(2, 1));
    EXPECT_TRUE(copiedNetwork.isEdgeIdx(3, 1));
    EXPECT_FALSE(copiedNetwork.isEdgeIdx(1, 3));
}

TEST(Reverse, when_reverseGraph_expect_onlyReverseEdgesExist) {
    PGL::DirectedGraph graph(5);

    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(3, 1);
    graph.addReciprocalEdgeIdx(3, 4);

    auto reversedGraph = graph.getReversedGraph();

    EXPECT_FALSE(reversedGraph.isEdgeIdx(1, 2));
    EXPECT_TRUE(reversedGraph.isEdgeIdx(2, 1));
    EXPECT_FALSE(reversedGraph.isEdgeIdx(3, 1));
    EXPECT_TRUE(reversedGraph.isEdgeIdx(1, 3));
    EXPECT_TRUE(reversedGraph.isEdgeIdx(3, 4));
    EXPECT_TRUE(reversedGraph.isEdgeIdx(4, 3));

    EXPECT_EQ(graph.getEdgeNumber(), reversedGraph.getEdgeNumber());
}

TEST(RangedBasedFor, expect_returns_vertices){
    PGL::DirectedGraph graph(10);
    list<PGL::VertexIndex> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<PGL::VertexIndex> loopVertices;

    for(PGL::VertexIndex& vertex: graph)
        loopVertices.push_back(vertex);
    EXPECT_EQ(loopVertices, expectedVertices);
}

TEST(Iterator, when_postAndPreIncrement_expect_loopsCorrectly) {
    PGL::DirectedGraph graph(10);
    list<PGL::VertexIndex> expectedVertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<PGL::VertexIndex> loopVertices;

    for(PGL::DirectedGraph::iterator it=graph.begin(); it!=graph.end(); it++)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);

    loopVertices.clear();
    for(PGL::DirectedGraph::iterator it=graph.begin(); it!=graph.end(); ++it)
        loopVertices.push_back(*it);
    EXPECT_EQ(loopVertices, expectedVertices);
}
