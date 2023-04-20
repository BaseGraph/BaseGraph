#include "BaseGraph/algorithms/topology.hpp"
#include <gtest/gtest.h>

TEST(DirectedGraph, getSubgraph_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::DirectedGraph graph(5);
    graph.addEdge(0, 1);
    graph.addReciprocalEdge(2, 1);
    graph.addEdge(2, 3);
    graph.addReciprocalEdge(0, 3);
    graph.addEdge(3, 3);

    auto subgraph = BaseGraph::algorithms::getSubgraph(graph, {0, 2, 3});

    EXPECT_FALSE(subgraph.hasEdge(0, 1));
    EXPECT_FALSE(subgraph.hasEdge(2, 1));
    EXPECT_FALSE(subgraph.hasEdge(1, 2));
    EXPECT_TRUE(subgraph.hasEdge(2, 3));
    EXPECT_TRUE(subgraph.hasEdge(0, 3));
    EXPECT_TRUE(subgraph.hasEdge(3, 0));
    EXPECT_TRUE(subgraph.hasEdge(3, 3));
    EXPECT_EQ(subgraph.getEdgeNumber(), 4);
}

TEST(DirectedGraph, getSubgraph_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(2);

    EXPECT_THROW(BaseGraph::algorithms::getSubgraph(graph, {0, 2, 3}),
                 std::out_of_range);
}

TEST(DirectedGraph,
     getSubgraphWithRemapOf_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::DirectedGraph graph(5);
    graph.addEdge(0, 1);
    graph.addReciprocalEdge(2, 1);
    graph.addEdge(2, 3);
    graph.addReciprocalEdge(0, 3);
    graph.addEdge(3, 3);

    auto subgraph_remap =
        BaseGraph::algorithms::getSubgraphWithRemap(graph, {0, 2, 3});
    auto &subgraph = subgraph_remap.first;
    auto &remap = subgraph_remap.second;

    EXPECT_EQ(subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.hasEdge(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.hasEdge(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.hasEdge(remap[3], remap[0]));
    EXPECT_TRUE(subgraph.hasEdge(remap[3], remap[3]));
    EXPECT_EQ(subgraph.getEdgeNumber(), 4);
}

TEST(DirectedGraph,
     getSubgraphWithRemap_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::DirectedGraph graph(2);

    EXPECT_THROW(BaseGraph::algorithms::getSubgraphWithRemap(graph, {0, 2, 3}),
                 std::out_of_range);
}

TEST(UndirectedGraph, getSubgraph_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::UndirectedGraph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(2, 1);
    graph.addEdge(2, 3);
    graph.addEdge(0, 3);
    graph.addEdge(3, 3);

    auto subgraph = BaseGraph::algorithms::getSubgraph(graph, {0, 2, 3});

    EXPECT_FALSE(subgraph.hasEdge(0, 1));
    EXPECT_FALSE(subgraph.hasEdge(2, 1));
    EXPECT_TRUE(subgraph.hasEdge(2, 3));
    EXPECT_TRUE(subgraph.hasEdge(0, 3));
    EXPECT_TRUE(subgraph.hasEdge(3, 3));
    EXPECT_EQ(subgraph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph, getSubgraph_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(3);

    EXPECT_THROW(BaseGraph::algorithms::getSubgraph(graph, {0, 2, 3}),
                 std::out_of_range);
}

TEST(UndirectedGraph,
     getSubgraphWithRemap_validVertexSubset_graphOnlyHasEdgesOfSubset) {
    BaseGraph::UndirectedGraph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(2, 1);
    graph.addEdge(2, 3);
    graph.addEdge(0, 3);
    graph.addEdge(3, 3);

    auto subgraph_remap =
        BaseGraph::algorithms::getSubgraphWithRemap(graph, {0, 2, 3});
    auto &subgraph = subgraph_remap.first;
    auto &remap = subgraph_remap.second;

    EXPECT_EQ(subgraph.getSize(), 3);
    EXPECT_TRUE(subgraph.hasEdge(remap[2], remap[3]));
    EXPECT_TRUE(subgraph.hasEdge(remap[0], remap[3]));
    EXPECT_TRUE(subgraph.hasEdge(remap[3], remap[3]));
    EXPECT_EQ(subgraph.getEdgeNumber(), 3);
}

TEST(UndirectedGraph,
     getSubgraphWithRemap_vertexSubsetOutOfRange_throwInvalidArgument) {
    BaseGraph::UndirectedGraph graph(3);

    EXPECT_THROW(BaseGraph::algorithms::getSubgraphWithRemap(graph, {0, 2, 3}),
                 std::out_of_range);
}
