#include <list>
#include <set>
#include <vector>

#include "gtest/gtest.h"
#include "BaseGraph/extensions/random/randomgraphs.h"


using namespace std;


TEST(ConfigurationModel, when_doubleEdgeSwappingEdges_expect_degreeSequencePreservedAndGraphChanged){
    BaseGraph::UndirectedGraph graph(11);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);

    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(6, 4);
    graph.addEdge(6, 7);
    graph.addEdge(7, 8);
    graph.addEdge(7, 9);

    BaseGraph::UndirectedGraph graphBeforeSwaps = graph;

    BaseGraph::random::rng.seed(0);
    BaseGraph::random::shuffleGraphWithConfigurationModel(graph);
    EXPECT_NE(graph, graphBeforeSwaps);
    EXPECT_EQ(graph.getDegrees(), graphBeforeSwaps.getDegrees());
}
