#include <list>
#include <set>
#include <vector>

#include "gtest/gtest.h"
#include "BaseGraph/extensions/random/randomgraphs.h"


using namespace std;


TEST(ConfigurationModel, when_doubleEdgeSwappingEdges_expect_degreeSequencePreservedAndGraphChanged){
    BaseGraph::UndirectedGraph graph(11);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 3);

    graph.addEdgeIdx(4, 5);
    graph.addEdgeIdx(5, 6);
    graph.addEdgeIdx(6, 4);
    graph.addEdgeIdx(6, 7);
    graph.addEdgeIdx(7, 8);
    graph.addEdgeIdx(7, 9);

    BaseGraph::UndirectedGraph graphBeforeSwaps = graph;

    BaseGraph::random::rng.seed(0);
    BaseGraph::random::shuffleGraphWithConfigurationModel(graph);
    EXPECT_NE(graph, graphBeforeSwaps);
    EXPECT_EQ(graph.getDegrees(), graphBeforeSwaps.getDegrees());
}
