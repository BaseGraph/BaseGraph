#include <gtest/gtest.h>
#include "BaseGraph/undirected_graph.hpp"
#include "BaseGraph/algorithms/paths.hpp"
#include "fixtures.hpp"


using namespace BaseGraph;

TEST_F(UndirectedHouseGraph, when_findingPredecessors_expect_returnsCorrectPathsLengthsAndPredecessors){
    auto shortestPaths = algorithms::findPredecessorsOfVertex(graph, 4);
    EXPECT_EQ(shortestPaths.first[0], 2);
    EXPECT_EQ(shortestPaths.first[1], 1);
    EXPECT_EQ(shortestPaths.first[2], 2);
    EXPECT_EQ(shortestPaths.first[3], 1);
    EXPECT_EQ(shortestPaths.first[4], 0);
    EXPECT_EQ(shortestPaths.first[5], 2);
    EXPECT_EQ(shortestPaths.first[6], algorithms::BASEGRAPH_VERTEX_MAX);

    EXPECT_EQ(shortestPaths.second[0], 3);
    EXPECT_EQ(shortestPaths.second[1], 4);
    EXPECT_TRUE(shortestPaths.second[2] == 1 || shortestPaths.second[2] == 3);
    EXPECT_EQ(shortestPaths.second[3], 4);
    EXPECT_EQ(shortestPaths.second[4], algorithms::BASEGRAPH_VERTEX_MAX);
    EXPECT_EQ(shortestPaths.second[5], 3);
    EXPECT_EQ(shortestPaths.second[6], algorithms::BASEGRAPH_VERTEX_MAX);

}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessor_expect_correctPath){
    auto shortestPaths = algorithms::findPredecessorsOfVertex(graph, 4);

    EXPECT_EQ(algorithms::findPathToVertexFromPredecessors(graph, 0, shortestPaths),
            std::list<BaseGraph::VertexIndex>({4, 3, 0}));

    EXPECT_EQ(algorithms::findPathToVertexFromPredecessors(graph, 5, shortestPaths),
            std::list<BaseGraph::VertexIndex>({4, 3, 5}));
}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessorToIsolatedVertex_expect_throwRuntimeError){
    auto shortestPaths = algorithms::findPredecessorsOfVertex(graph, 4);
    EXPECT_THROW(algorithms::findPathToVertexFromPredecessors(graph, 6, shortestPaths), std::runtime_error);
}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessorFromIsolatedVertex_expect_throwRuntimeError){
    auto shortestPaths = algorithms::findPredecessorsOfVertex(graph, 6);
    EXPECT_THROW(algorithms::findPathToVertexFromPredecessors(graph, 0, shortestPaths), std::runtime_error);
}

TEST_F(TreeLikeGraph, when_findingAllPredecessors_expect_returnEveryPredecessor){
    auto shortestPaths = algorithms::findAllPredecessorsOfVertex(graph, 0).second;

    EXPECT_EQ(shortestPaths[7], algorithms::Path({6}));
    EXPECT_EQ(shortestPaths[6], algorithms::Path({3, 4, 5}));
    EXPECT_EQ(shortestPaths[5], algorithms::Path({2}));
    EXPECT_EQ(shortestPaths[4], algorithms::Path({1, 2}));
    EXPECT_EQ(shortestPaths[3], algorithms::Path({1}));
    EXPECT_EQ(shortestPaths[2], algorithms::Path({0}));
    EXPECT_EQ(shortestPaths[1], algorithms::Path({0}));
}

TEST_F(TreeLikeGraph, when_findingAllPredecessors_expect_returnEveryPath){
    auto shortestPaths = algorithms::findAllPredecessorsOfVertex(graph, 0);
    auto geodesics = algorithms::findMultiplePathsToVertexFromPredecessors(graph, 4, shortestPaths);

    EXPECT_EQ(geodesics, algorithms::MultiplePaths({{0, 2, 4}, {0, 1, 4}}));

    geodesics = algorithms::findMultiplePathsToVertexFromPredecessors(graph, 7, shortestPaths);
    EXPECT_EQ(geodesics, algorithms::MultiplePaths(
                {{0, 2, 5, 6, 7}, {0, 2, 4, 6, 7},
                {0, 1, 4, 6, 7}, {0, 1, 3, 6, 7} }));

    geodesics = algorithms::findMultiplePathsToVertexFromPredecessors(graph, 1, shortestPaths);
    EXPECT_EQ(geodesics, std::list<std::list<BaseGraph::VertexIndex>>( {{0, 1}} ));
}
