#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

#include "gtest/gtest.h"
#include "BaseGraph/fileio.h"


using namespace std;


class CharEdgeLabeledDirectedGraph: public::testing::Test{
    public:
        BaseGraph::EdgeLabeledDirectedGraph<unsigned char> graph = BaseGraph::EdgeLabeledDirectedGraph<unsigned char>(6);
        std::list<std::tuple<size_t, size_t, unsigned char>> edges = {
            {1, 3, 'b'}, {1, 4, 'a'}, {1, 5, 'c'}, {2, 3, 'g'}, {2, 4, 'f'}, {2, 5, 'e'}, {3, 4, 'd'}, {4, 5, 'c'}
        };
        size_t edgeValueSum = 0;

        void SetUp() {
            for (auto edge: edges) {
                graph.addEdge(get<0>(edge), get<1>(edge), get<2>(edge));
                edgeValueSum += get<2>(edge);
            }
        }
};

class CharEdgeLabeledUndirectedGraph: public::testing::Test{
    public:
        BaseGraph::EdgeLabeledUndirectedGraph<unsigned char> graph = BaseGraph::EdgeLabeledUndirectedGraph<unsigned char>(6);
        std::list<std::tuple<size_t, size_t, unsigned char>> edges = {
            {1, 3, 'b'}, {1, 4, 'a'}, {1, 5, 'c'}, {2, 3, 'g'}, {2, 4, 'f'}, {2, 5, 'e'}, {3, 4, 'd'}, {4, 5, 'c'}
        };
        size_t edgeValueSum = 0;

        void SetUp() {
            for (auto edge: edges) {
                graph.addEdge(get<0>(edge), get<1>(edge), get<2>(edge));
                edgeValueSum += get<2>(edge);
            }
        }
};

TEST(DirectedTextEdgeList, writeAndLoadGraph_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeTextEdgeList(graph, "testGraph_tmp.txt");
    auto loadedGraph_vertices = BaseGraph::io::loadDirectedTextEdgeList("testGraph_tmp.txt");
    auto& loadedGraph = loadedGraph_vertices.first;

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_FALSE(loadedGraph.hasEdge(1, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(2, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(DirectedTextEdgeList, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadDirectedTextEdgeList("\0"), runtime_error);
}


TEST(DirectedBinaryEdgeList, writeAndLoadGraph_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeBinaryEdgeList(graph, "testGraph_tmp.bin");
    BaseGraph::DirectedGraph loadedGraph = BaseGraph::io::loadDirectedBinaryEdgeList("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_FALSE(loadedGraph.hasEdge(1, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(2, 0));
    EXPECT_FALSE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.bin");
}

TEST(DirectedBinaryEdgeList, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadDirectedBinaryEdgeList("\0"), runtime_error);
}


TEST(UndirectedTextEdgeList, writeAndLoadGraph_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeTextEdgeList(graph, "testGraph_tmp.txt");
    auto loadedGraph_vertices = BaseGraph::io::loadUndirectedTextEdgeList("testGraph_tmp.txt");
    auto& loadedGraph = loadedGraph_vertices.first;

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_TRUE(loadedGraph.hasEdge(1, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(2, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(UndirectedTextEdgeList, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadUndirectedTextEdgeList("\0"), runtime_error);
}


TEST(UndirectedBinaryEdgeList, writeAndLoadGraph_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 14);

    BaseGraph::io::writeBinaryEdgeList(graph, "testGraph_tmp.bin");
    BaseGraph::UndirectedGraph loadedGraph = BaseGraph::io::loadUndirectedBinaryEdgeList("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.hasEdge(0, 1));
    EXPECT_TRUE(loadedGraph.hasEdge(0, 2));
    EXPECT_TRUE(loadedGraph.hasEdge(3, 14));
    EXPECT_TRUE(loadedGraph.hasEdge(1, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(2, 0));
    EXPECT_TRUE(loadedGraph.hasEdge(14, 3));

    remove("testGraph_tmp.bin");
}

TEST(UndirectedBinaryEdgeList, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadUndirectedBinaryEdgeList("\0"), runtime_error);
}


TEST_F(CharEdgeLabeledDirectedGraph, writingEdgesToBinaryAndLoadingThem_graphContainsAllEdges) {
    BaseGraph::io::writeBinaryEdgeList(graph, "verticesList_tmp.bin");
    auto loadedGraph = BaseGraph::io::loadLabeledDirectedBinaryEdgeList<unsigned char>("verticesList_tmp.bin");

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.hasEdge(get<0>(edge), get<1>(edge)));
        EXPECT_FALSE(loadedGraph.hasEdge(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabelOf(get<0>(edge), get<1>(edge)), get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getEdgeNumber(), 8);
    EXPECT_EQ(loadedGraph.getTotalEdgeNumber(), edgeValueSum);

    remove("verticesList_tmp.bin");
}

TEST(DirectedBinaryLabeledEdgeList, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadLabeledDirectedBinaryEdgeList<unsigned char>("\0"), runtime_error);
}


TEST_F(CharEdgeLabeledUndirectedGraph, writingEdgesToBinaryAndReloadThem_graphContainsAllEdges) {
    BaseGraph::io::writeBinaryEdgeList(graph, "verticesList_tmp.bin");
    auto loadedGraph = BaseGraph::io::loadLabeledUndirectedBinaryEdgeList<unsigned char>("verticesList_tmp.bin");

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.hasEdge(get<0>(edge), get<1>(edge)));
        EXPECT_TRUE(loadedGraph.hasEdge(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabelOf(get<0>(edge), get<1>(edge)), get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getEdgeNumber(), 8);
    EXPECT_EQ(loadedGraph.getTotalEdgeNumber(), edgeValueSum);

    remove("verticesList_tmp.bin");
}

TEST(UndirectedTextLabeledEdgeList, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadLabeledUndirectedBinaryEdgeList<unsigned char>("\0"), runtime_error);
}
