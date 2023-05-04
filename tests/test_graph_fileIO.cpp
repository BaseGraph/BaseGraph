#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "BaseGraph/fileio.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"
#include "fixtures.hpp"
#include "gtest/gtest.h"

using namespace std;

class CharLabeledDirectedGraph : public ::testing::Test {
  public:
    BaseGraph::LabeledDirectedGraph<unsigned char> graph =
        BaseGraph::LabeledDirectedGraph<unsigned char>(6);
    std::list<std::tuple<size_t, size_t, unsigned char>> edges = {
        {1, 3, 'b'}, {1, 4, 'a'}, {1, 5, 'c'}, {2, 3, 'g'},
        {2, 4, 'f'}, {2, 5, 'e'}, {3, 4, 'd'}, {4, 5, 'c'}};
    size_t edgeValueSum = 0;

    void SetUp() {
        for (auto edge : edges) {
            graph.addEdge(get<0>(edge), get<1>(edge), get<2>(edge));
            edgeValueSum += get<2>(edge);
        }
    }
};

class CharEdgeLabeledUndirectedGraph : public ::testing::Test {
  public:
    BaseGraph::LabeledUndirectedGraph<unsigned char> graph =
        BaseGraph::LabeledUndirectedGraph<unsigned char>(6);
    std::list<std::tuple<size_t, size_t, unsigned char>> edges = {
        {1, 3, 'b'}, {1, 4, 'a'}, {1, 5, 'c'}, {2, 3, 'g'},
        {2, 4, 'f'}, {2, 5, 'e'}, {3, 4, 'd'}, {4, 5, 'c'}};
    size_t edgeValueSum = 0;

    void SetUp() {
        for (auto edge : edges) {
            graph.addEdge(get<0>(edge), get<1>(edge), get<2>(edge));
            edgeValueSum += get<2>(edge);
        }
    }
};

TEST(DirectedTextEdgeList, writeAndLoadGraph_allEdgesExist) {
    BaseGraph::DirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeTextEdgeList(graph, "testGraph_tmp.txt");
    auto loadedGraph_vertices =
        BaseGraph::io::loadTextEdgeList<BaseGraph::LabeledDirectedGraph,
                                        BaseGraph::NoLabel>(
            "testGraph_tmp.txt");
    auto &loadedGraph = loadedGraph_vertices.first;

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_FALSE(loadedGraph.hasEdge(1, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(2, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(DirectedTextEdgeList, inexistentFile_throwRuntimeError) {
    auto f = [](const std::string &s) {
        BaseGraph::io::loadTextEdgeList<BaseGraph::LabeledDirectedGraph,
                                        BaseGraph::NoLabel>(s);
    };
    EXPECT_THROW(f("\0"), std::runtime_error);
}

TEST(DirectedBinaryEdgeList, writeAndLoadGraph_allEdgesExist) {
    BaseGraph::DirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeBinaryEdgeList(graph, "testGraph_tmp.bin");
    BaseGraph::DirectedGraph loadedGraph =
        BaseGraph::io::loadBinaryEdgeList<BaseGraph::LabeledDirectedGraph,
                                          BaseGraph::NoLabel>(
            "testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_FALSE(loadedGraph.hasEdge(1, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(2, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.bin");
}

TEST(BinaryEdgeList, inexistentFile_throwRuntimeError) {
    auto f = [](const std::string &s) {
        return BaseGraph::io::loadBinaryEdgeList<
            BaseGraph::LabeledDirectedGraph, char>(s);
    };
    EXPECT_THROW(f("\0"), runtime_error);
}

TEST(UnlabeledBinaryEdgeList, inexistentFile_throwRuntimeError) {
    auto f = [](const std::string &s) {
        return BaseGraph::io::loadBinaryEdgeList<
            BaseGraph::LabeledDirectedGraph, BaseGraph::NoLabel>(s);
    };
    EXPECT_THROW(f("\0"), runtime_error);
}

TEST(UndirectedTextEdgeList, writeAndLoadGraph_allEdgesExist) {
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeTextEdgeList(graph, "testGraph_tmp.txt");
    auto loadedGraph_vertices =
        BaseGraph::io::loadTextEdgeList<BaseGraph::LabeledUndirectedGraph,
                                        BaseGraph::NoLabel>(
            "testGraph_tmp.txt");
    auto &loadedGraph = loadedGraph_vertices.first;

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_TRUE(loadedGraph.hasEdge(1, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(2, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(UndirectedBinaryEdgeList, writeAndLoadGraph_allEdgesExist) {
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeBinaryEdgeList(graph, "testGraph_tmp.bin");
    BaseGraph::UndirectedGraph loadedGraph =
        BaseGraph::io::loadBinaryEdgeList<BaseGraph::LabeledUndirectedGraph,
                                          BaseGraph::NoLabel>(
            "testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_TRUE(loadedGraph.hasEdge(1, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(2, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.bin");
}

TEST_F(CharLabeledDirectedGraph,
       writingEdgesToBinaryAndLoadingThem_graphContainsAllEdges) {
    BaseGraph::io::writeBinaryEdgeList(graph, "verticesList_tmp.bin");
    auto loadedGraph =
        BaseGraph::io::loadBinaryEdgeList<BaseGraph::LabeledDirectedGraph,
                                          char>("verticesList_tmp.bin");

    for (auto edge : edges) {
        EXPECT_TRUE(loadedGraph.hasEdge(get<0>(edge), get<1>(edge)));
        EXPECT_FALSE(loadedGraph.hasEdge(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabel(get<0>(edge), get<1>(edge)),
                  get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getEdgeNumber(), 8);
    remove("verticesList_tmp.bin");
}

TEST_F(CharEdgeLabeledUndirectedGraph,
       writingEdgesToBinaryAndReloadThem_graphContainsAllEdges) {
    BaseGraph::io::writeBinaryEdgeList(graph, "verticesList_tmp.bin");
    auto loadedGraph =
        BaseGraph::io::loadBinaryEdgeList<BaseGraph::LabeledUndirectedGraph,
                                          unsigned char>(
            "verticesList_tmp.bin");

    for (auto edge : edges) {
        EXPECT_TRUE(loadedGraph.hasEdge(get<0>(edge), get<1>(edge)));
        EXPECT_TRUE(loadedGraph.hasEdge(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabel(get<0>(edge), get<1>(edge)),
                  get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getEdgeNumber(), 8);
    remove("verticesList_tmp.bin");
}
