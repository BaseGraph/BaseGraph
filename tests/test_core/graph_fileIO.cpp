#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

#include "gtest/gtest.h"
#include "BaseGraph/fileio.h"


using namespace std;


class SmallGraphChar: public::testing::Test{
    public:
        BaseGraph::VertexLabeledUndirectedGraph<unsigned char> graph;
        std::list<char> vertices = { 10, 20, 30, 40, 50 };
        std::list<std::pair<char, char>> edges = {
            {10, 30}, {10, 40}, {10, 50}, {20, 30}, {20, 40}, {20, 50}, {30, 40}, {40, 50}
        };

        void SetUp(){
            for (auto vertex: vertices)
                graph.addVertex(vertex);

            for (auto edge: edges)
                graph.addEdge(edge.first, edge.second);
        }
};

class SmallDirectedGraphChar: public::testing::Test{
    public:
        BaseGraph::VertexLabeledDirectedGraph<unsigned char> graph;
        std::list<char> vertices = { 10, 20, 30, 40, 50 };
        std::list<std::pair<char, char>> edges = {
            {10, 30}, {10, 40}, {10, 50}, {20, 30}, {20, 40}, {20, 50}, {30, 40}, {40, 50}
        };

        void SetUp(){
            for (auto vertex: vertices)
                graph.addVertex(vertex);

            for (auto edge: edges)
                graph.addEdge(edge.first, edge.second);
        }
};

class CharEdgeLabeledDirectedGraph: public::testing::Test{
    public:
        BaseGraph::EdgeLabeledDirectedGraph<unsigned char> graph = BaseGraph::EdgeLabeledDirectedGraph<unsigned char>(6);
        std::list<std::tuple<size_t, size_t, unsigned char>> edges = {
            {1, 3, 1}, {1, 4, 0}, {1, 5, 2}, {2, 3, 6}, {2, 4, 5}, {2, 5, 4}, {3, 4, 3}, {4, 5, 2}
        };

        void SetUp() {
            for (auto edge: edges)
                graph.addEdgeIdx(get<0>(edge), get<1>(edge), get<2>(edge));
        }
};

class CharEdgeLabeledUndirectedGraph: public::testing::Test{
    public:
        BaseGraph::EdgeLabeledUndirectedGraph<unsigned char> graph = BaseGraph::EdgeLabeledUndirectedGraph<unsigned char>(6);
        std::list<std::tuple<size_t, size_t, unsigned char>> edges = {
            {1, 3, 1}, {1, 4, 0}, {1, 5, 2}, {2, 3, 6}, {2, 4, 5}, {2, 5, 4}, {3, 4, 3}, {4, 5, 2}
        };

        void SetUp() {
            for (auto edge: edges)
                graph.addEdgeIdx(get<0>(edge), get<1>(edge), get<2>(edge));
        }
};

TEST(DirectedGraph, writingEdgeListIdxInTextFileAndReloadingIt_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");
    BaseGraph::DirectedGraph loadedGraph = BaseGraph::loadDirectedEdgeListIdxFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(DirectedGraph, writingEdgeListIdxInBinaryFileAndReloadingIt_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInBinaryFile(graph, "testGraph_tmp.bin");
    BaseGraph::DirectedGraph loadedGraph = BaseGraph::loadDirectedEdgeListIdxFromBinaryFile("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.bin");
}

TEST(UndirectedGraph, writingEdgeListIdxInTextFileAndReloadingIt_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");
    BaseGraph::UndirectedGraph loadedGraph = BaseGraph::loadUndirectedEdgeListIdxFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(UndirectedGraph, writingEdgeListIdxInBinaryFileAndReloadingIt_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInBinaryFile(graph, "testGraph_tmp.bin");
    BaseGraph::UndirectedGraph loadedGraph = BaseGraph::loadUndirectedEdgeListIdxFromBinaryFile("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.bin");
}

TEST_F(SmallGraphChar, writingEdgeListInTextFileAndReloadingIt_allEdgesAndVerticesExist){
    BaseGraph::writeEdgeListInTextFile(graph, "testGraph_tmp.txt");
    auto loadedGraph = BaseGraph::loadUndirectedEdgeListFromTextFile("testGraph_tmp.txt");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(std::to_string(vertex)));

    for (auto edge: edges)
        EXPECT_TRUE(loadedGraph.isEdge(std::to_string(edge.first), std::to_string(edge.second)));

    remove("testGraph_tmp.txt");
}

TEST_F(SmallGraphChar, writingEdgeListInBinaryAndReloadIt_graphContainsAllVerticesAndEdges){
    BaseGraph::writeEdgeListInBinaryFile(graph, "edgeList_tmp.bin");
    auto loadedGraph = BaseGraph::loadUndirectedEdgeListFromBinaryFile<unsigned char>("edgeList_tmp.bin");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(vertex));

    for (auto edge: edges)
        EXPECT_TRUE(loadedGraph.isEdge(edge.first, edge.second));

    remove("edgeList_tmp.bin");
}

TEST_F(SmallDirectedGraphChar, writingEdgeListInTextFileAndReloadingIt_allEdgesAndVerticesExist){
    BaseGraph::writeEdgeListInTextFile(graph, "testGraph_tmp.txt");
    auto loadedGraph = BaseGraph::loadDirectedEdgeListFromTextFile("testGraph_tmp.txt");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(std::to_string(vertex)));

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdge(std::to_string(edge.first), std::to_string(edge.second)));
        EXPECT_FALSE(loadedGraph.isEdge(std::to_string(edge.second), std::to_string(edge.first)));
    }

    remove("testGraph_tmp.txt");
}


TEST_F(SmallDirectedGraphChar, writingEdgeListInBinaryAndReloadIt_graphContainsAllVerticesAndEdges){
    BaseGraph::writeEdgeListInBinaryFile(graph, "edgeList_tmp.bin");
    auto loadedGraph = BaseGraph::loadDirectedEdgeListFromBinaryFile<unsigned char>("edgeList_tmp.bin");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(vertex));

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdge(edge.first, edge.second));
        EXPECT_FALSE(loadedGraph.isEdge(edge.second, edge.first));
    }

    remove("edgeList_tmp.bin");
}

TEST(loadFromEdgeListBinary, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::loadUndirectedEdgeListFromBinaryFile<bool>("\0"), runtime_error);
}

TEST(loadFromTextFile, inexistentFile_expect_throwRuntimeError){
    EXPECT_THROW(BaseGraph::loadUndirectedEdgeListFromTextFile("\0"), runtime_error);
}

TEST_F(SmallGraphChar, writingVerticesBinaryAndReloadThem_graphContainsCorrectVertices){
    writeVerticesInBinaryFile(graph, "verticesList_tmp.bin");
    BaseGraph::VertexLabeledUndirectedGraph<unsigned char> loadedGraph;
    addVerticesFromBinaryFile(loadedGraph, "verticesList_tmp.bin");

    EXPECT_TRUE(loadedGraph.isVertex(10));
    EXPECT_TRUE(loadedGraph.isVertex(20));
    EXPECT_TRUE(loadedGraph.isVertex(30));
    EXPECT_TRUE(loadedGraph.isVertex(40));
    EXPECT_TRUE(loadedGraph.isVertex(50));
    remove("verticesList_tmp.bin");
}


TEST(addVerticesFromBinaryFile, inexistentFile_throwRuntimeError){
    BaseGraph::VertexLabeledUndirectedGraph<bool> graph;
    EXPECT_THROW(addVerticesFromBinaryFile(graph, "\0"), runtime_error);
}

TEST_F(CharEdgeLabeledDirectedGraph, writingEdgesToBinaryAndReloadThem_graphContainsAllEdges) {
    BaseGraph::writeEdgeListIdxInBinaryFile(graph, "verticesList_tmp.bin");
    auto loadedGraph = BaseGraph::loadLabeledDirectedEdgeListIdxFromBinaryFile<unsigned char>("verticesList_tmp.bin");

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdgeIdx(get<0>(edge), get<1>(edge)));
        EXPECT_FALSE(loadedGraph.isEdgeIdx(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabelOf(get<0>(edge), get<1>(edge)), get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getDistinctEdgeNumber(), 8);
    EXPECT_EQ(loadedGraph.getTotalEdgeNumber(), 23);
}

TEST_F(CharEdgeLabeledUndirectedGraph, writingEdgesToBinaryAndReloadThem_graphContainsAllEdges) {
    BaseGraph::writeEdgeListIdxInBinaryFile(graph, "verticesList_tmp.bin");
    auto loadedGraph = BaseGraph::loadLabeledUndirectedEdgeListIdxFromBinaryFile<unsigned char>("verticesList_tmp.bin");

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdgeIdx(get<0>(edge), get<1>(edge)));
        EXPECT_TRUE(loadedGraph.isEdgeIdx(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabelOf(get<0>(edge), get<1>(edge)), get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getDistinctEdgeNumber(), 8);
    EXPECT_EQ(loadedGraph.getTotalEdgeNumber(), 23);
}
