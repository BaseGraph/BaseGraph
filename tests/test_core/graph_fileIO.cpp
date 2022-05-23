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
        std::list<char> vertices = { 'a', 'b', 'c', 'd', 'e' };
        std::list<std::pair<char, char>> edges = {
            {'a', 'c'}, {'a', 'd'}, {'a', 'e'}, {'b', 'c'}, {'b', 'd'}, {'b', 'e'}, {'c', 'd'}, {'d', 'e'}
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
        std::list<char> vertices = { 'a', 'b', 'c', 'd', 'e' };
        std::list<std::pair<char, char>> edges = {
            {'a', 'c'}, {'a', 'd'}, {'a', 'e'}, {'b', 'c'}, {'b', 'd'}, {'b', 'e'}, {'c', 'd'}, {'d', 'e'}
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
            {1, 3, 'b'}, {1, 4, 'a'}, {1, 5, 'c'}, {2, 3, 'g'}, {2, 4, 'f'}, {2, 5, 'e'}, {3, 4, 'd'}, {4, 5, 'c'}
        };
        size_t edgeValueSum = 0;

        void SetUp() {
            for (auto edge: edges) {
                graph.addEdgeIdx(get<0>(edge), get<1>(edge), get<2>(edge));
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
                graph.addEdgeIdx(get<0>(edge), get<1>(edge), get<2>(edge));
                edgeValueSum += get<2>(edge);
            }
        }
};

TEST(DirectedGraph, writingTextEdgeListIdxAndLoadingIt_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::io::writeTextEdgeListIdx(graph, "testGraph_tmp.txt");
    BaseGraph::DirectedGraph loadedGraph = BaseGraph::io::loadDirectedTextEdgeListIdx("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(DirectedGraph, writingBinaryEdgeListIdxAndLoadingIt_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::io::writeBinaryEdgeListIdx(graph, "testGraph_tmp.bin");
    BaseGraph::DirectedGraph loadedGraph = BaseGraph::io::loadDirectedBinaryEdgeListIdx("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.bin");
}

TEST(UndirectedGraph, writingTextEdgeListIdxAndLoadingIt_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::io::writeTextEdgeListIdx(graph, "testGraph_tmp.txt");
    BaseGraph::UndirectedGraph loadedGraph = BaseGraph::io::loadUndirectedTextEdgeListIdx("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(UndirectedGraph, writingBinaryEdgeListIdxAndLoadingIt_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::io::writeBinaryEdgeListIdx(graph, "testGraph_tmp.bin");
    BaseGraph::UndirectedGraph loadedGraph = BaseGraph::io::loadUndirectedBinaryEdgeListIdx("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.bin");
}

TEST_F(SmallGraphChar, writingTextEdgeListAndLoadingIt_allEdgesAndVerticesExist){
    BaseGraph::io::writeTextEdgeList(graph, "testGraph_tmp.txt");
    auto loadedGraph = BaseGraph::io::loadUndirectedTextEdgeList("testGraph_tmp.txt");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(std::string{vertex}));

    for (auto edge: edges)
        EXPECT_TRUE(loadedGraph.isEdge(std::string{edge.first}, std::string{edge.second}));

    remove("testGraph_tmp.txt");
}

TEST_F(SmallGraphChar, writingBinaryEdgeListAndLoadingIt_graphContainsAllVerticesAndEdges){
    BaseGraph::io::writeBinaryEdgeList(graph, "edgeList_tmp.bin");
    auto loadedGraph = BaseGraph::io::loadUndirectedBinaryEdgeList<unsigned char>("edgeList_tmp.bin");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(vertex));

    for (auto edge: edges)
        EXPECT_TRUE(loadedGraph.isEdge(edge.first, edge.second));

    remove("edgeList_tmp.bin");
}

TEST_F(SmallDirectedGraphChar, writingTextEdgeListAndLoadingIt_allEdgesAndVerticesExist){
    BaseGraph::io::writeTextEdgeList(graph, "testGraph_tmp.txt");
    auto loadedGraph = BaseGraph::io::loadDirectedTextEdgeList("testGraph_tmp.txt");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(std::string{vertex}));

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdge(std::string{edge.first}, std::string{edge.second}));
        EXPECT_FALSE(loadedGraph.isEdge(std::string{edge.second}, std::string{edge.first}));
    }

    remove("testGraph_tmp.txt");
}


TEST_F(SmallDirectedGraphChar, writingBinaryEdgeListAndLoadingIt_graphContainsAllVerticesAndEdges){
    BaseGraph::io::writeBinaryEdgeList(graph, "edgeList_tmp.bin");
    auto loadedGraph = BaseGraph::io::loadDirectedBinaryEdgeList<unsigned char>("edgeList_tmp.bin");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(vertex));

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdge(edge.first, edge.second));
        EXPECT_FALSE(loadedGraph.isEdge(edge.second, edge.first));
    }

    remove("edgeList_tmp.bin");
}

TEST(loadFromEdgeListBinary, inexistentFile_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadUndirectedBinaryEdgeList<bool>("\0"), runtime_error);
}

TEST(loadFromTextFile, inexistentFile_expect_throwRuntimeError){
    EXPECT_THROW(BaseGraph::io::loadUndirectedTextEdgeList("\0"), runtime_error);
}

TEST_F(SmallGraphChar, writingVerticesBinaryAndLoadingThem_graphContainsCorrectVertices){
    BaseGraph::io::writeVerticesToBinary(graph, "verticesList_tmp.bin");
    BaseGraph::VertexLabeledUndirectedGraph<unsigned char> loadedGraph;
    BaseGraph::io::addVerticesFromBinary(loadedGraph, "verticesList_tmp.bin");

    for (auto vertex: vertices)
        EXPECT_TRUE(loadedGraph.isVertex(vertex));

    remove("verticesList_tmp.bin");
}


TEST(addVerticesFromBinaryFile, inexistentFile_throwRuntimeError){
    BaseGraph::VertexLabeledUndirectedGraph<bool> graph;
    EXPECT_THROW(BaseGraph::io::addVerticesFromBinary(graph, "\0"), runtime_error);
}

TEST_F(CharEdgeLabeledDirectedGraph, writingEdgesToBinaryAndLoadingThem_graphContainsAllEdges) {
    BaseGraph::io::writeBinaryEdgeListIdx(graph, "verticesList_tmp.bin");
    auto loadedGraph = BaseGraph::io::loadLabeledDirectedBinaryEdgeListIdx<unsigned char>("verticesList_tmp.bin");

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdgeIdx(get<0>(edge), get<1>(edge)));
        EXPECT_FALSE(loadedGraph.isEdgeIdx(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabelOfIdx(get<0>(edge), get<1>(edge)), get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getEdgeNumber(), 8);
    EXPECT_EQ(loadedGraph.getTotalEdgeNumber(), edgeValueSum);

    remove("verticesList_tmp.bin");
}

TEST_F(CharEdgeLabeledUndirectedGraph, writingEdgesToBinaryAndReloadThem_graphContainsAllEdges) {
    BaseGraph::io::writeBinaryEdgeListIdx(graph, "verticesList_tmp.bin");
    auto loadedGraph = BaseGraph::io::loadLabeledUndirectedBinaryEdgeListIdx<unsigned char>("verticesList_tmp.bin");

    for (auto edge: edges) {
        EXPECT_TRUE(loadedGraph.isEdgeIdx(get<0>(edge), get<1>(edge)));
        EXPECT_TRUE(loadedGraph.isEdgeIdx(get<1>(edge), get<0>(edge)));
        EXPECT_EQ(loadedGraph.getEdgeLabelOfIdx(get<0>(edge), get<1>(edge)), get<2>(edge));
    }

    EXPECT_EQ(loadedGraph.getEdgeNumber(), 8);
    EXPECT_EQ(loadedGraph.getTotalEdgeNumber(), edgeValueSum);

    remove("verticesList_tmp.bin");
}
