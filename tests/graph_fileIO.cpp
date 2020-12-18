#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

#include "gtest/gtest.h"
#include "pgl/directedgraph.h"
#include "pgl/undirectedgraph.h"
#include "pgl/vertexlabeled_directedgraph.hpp"
#include "pgl/vertexlabeled_undirectedgraph.hpp"


using namespace std;


class SmallGraphChar: public::testing::Test{
    public:
        PGL::VertexLabeledUndirectedGraph<unsigned char> graph;
        void SetUp(){
            graph.addVertex(10);
            graph.addVertex(20);
            graph.addVertex(30);
            graph.addVertex(40);
            graph.addVertex(50);

            graph.addEdge(10, 30);
            graph.addEdge(10, 40);
            graph.addEdge(10, 50);
            graph.addEdge(20, 30);
            graph.addEdge(20, 40);
            graph.addEdge(20, 50);
            graph.addEdge(30, 40);
            graph.addEdge(40, 50);
        }
};

class SmallDirectedGraphChar: public::testing::Test{
    public:
        PGL::VertexLabeledDirectedGraph<unsigned char> graph;
        void SetUp(){
            graph.addVertex(10);
            graph.addVertex(20);
            graph.addVertex(30);
            graph.addVertex(40);
            graph.addVertex(50);

            graph.addEdge(10, 30);
            graph.addEdge(10, 40);
            graph.addEdge(10, 50);
            graph.addEdge(20, 30);
            graph.addEdge(20, 40);
            graph.addEdge(20, 50);
            graph.addEdge(30, 40);
            graph.addEdge(40, 50);
        }
};

class SmallGraphString: public::testing::Test{
    public:
        PGL::VertexLabeledUndirectedGraph<string> graph;
        void SetUp(){
            graph.addVertex("10");
            graph.addVertex("20");
            graph.addVertex("30");
            graph.addVertex("40");
            graph.addVertex("50");

            graph.addEdge("10", "30");
            graph.addEdge("10", "40");
            graph.addEdge("10", "50");
            graph.addEdge("20", "30");
            graph.addEdge("20", "40");
            graph.addEdge("20", "50");
            graph.addEdge("30", "40");
            graph.addEdge("40", "50");
        }
};

TEST(DirectedGraph, when_writingEdgeListIdxInTextFileAndReloadingIt_expect_allEdgesExist){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 1);

    graph.writeEdgeListIdxInTextFile("testGraph_tmp.txt");
    PGL::DirectedGraph loadedGraph = PGL::DirectedGraph::loadEdgeListIdxFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 1));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 3));

    remove("testGraph_tmp.txt");
}

TEST(DirectedGraph, when_writingEdgeListIdxInBinaryFileAndReloadingIt_expect_allEdgesExist){
    PGL::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 1);

    graph.writeEdgeListIdxInBinaryFile("testGraph_tmp.bin");
    PGL::DirectedGraph loadedGraph = PGL::DirectedGraph::loadEdgeListIdxFromBinaryFile("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 1));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 3));

    remove("testGraph_tmp.bin");
}

TEST(UndirectedGraph, when_writingEdgeListIdxInTextFileAndReloadingIt_expect_allEdgesExist){
    PGL::UndirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 1);

    graph.writeEdgeListIdxInTextFile("testGraph_tmp.txt");
    PGL::UndirectedGraph loadedGraph = PGL::UndirectedGraph::loadEdgeListIdxFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 3));

    remove("testGraph_tmp.txt");
}

TEST(UndirectedGraph, when_writingEdgeListIdxInBinaryFileAndReloadingIt_expect_allEdgesExist){
    PGL::UndirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 1);

    graph.writeEdgeListIdxInBinaryFile("testGraph_tmp.bin");
    PGL::UndirectedGraph loadedGraph = PGL::UndirectedGraph::loadEdgeListIdxFromBinaryFile("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 3));

    remove("testGraph_tmp.txt");
}

TEST_F(SmallGraphChar, when_writingEdgeListInTextFileAndReloadingIt_expect_allEdgesAndVerticesExist){
    graph.writeEdgeListInTextFile("testGraph_tmp.txt");
    PGL::VertexLabeledUndirectedGraph<string> loadedGraph;
    loadedGraph = PGL::VertexLabeledUndirectedGraph<string>::loadEdgeListFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isVertex("10"));
    EXPECT_TRUE(loadedGraph.isVertex("20"));
    EXPECT_TRUE(loadedGraph.isVertex("30"));
    EXPECT_TRUE(loadedGraph.isVertex("40"));
    EXPECT_TRUE(loadedGraph.isVertex("50"));

    EXPECT_TRUE(loadedGraph.isEdge("10", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("30", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("40", "50"));

    remove("testGraph_tmp.txt");
}

TEST_F(SmallGraphChar, when_writingEdgeListInBinaryAndReloadIt_expect_graphContainsAllVerticesAndEdges){
    graph.writeEdgeListInBinaryFile("edgeList_tmp.bin");
    PGL::VertexLabeledUndirectedGraph<unsigned char> loadedGraph = PGL::VertexLabeledUndirectedGraph<unsigned char>::loadEdgeListFromBinaryFile("edgeList_tmp.bin");
    
    EXPECT_TRUE(loadedGraph.isVertex(10));
    EXPECT_TRUE(loadedGraph.isVertex(20));
    EXPECT_TRUE(loadedGraph.isVertex(30));
    EXPECT_TRUE(loadedGraph.isVertex(40));
    EXPECT_TRUE(loadedGraph.isVertex(50));

    EXPECT_TRUE(loadedGraph.isEdge(10, 30));
    EXPECT_TRUE(loadedGraph.isEdge(10, 40));
    EXPECT_TRUE(loadedGraph.isEdge(10, 50));
    EXPECT_TRUE(loadedGraph.isEdge(20, 30));
    EXPECT_TRUE(loadedGraph.isEdge(20, 40));
    EXPECT_TRUE(loadedGraph.isEdge(20, 50));
    EXPECT_TRUE(loadedGraph.isEdge(30, 40));
    EXPECT_TRUE(loadedGraph.isEdge(40, 50));

    remove("edgeList_tmp.bin");
}

TEST_F(SmallDirectedGraphChar, when_writingEdgeListInTextFileAndReloadingIt_expect_allEdgesAndVerticesExist){
    graph.writeEdgeListInTextFile("testGraph_tmp.txt");
    PGL::VertexLabeledDirectedGraph<string> loadedGraph;
    loadedGraph = PGL::VertexLabeledDirectedGraph<string>::loadEdgeListFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isVertex("10"));
    EXPECT_TRUE(loadedGraph.isVertex("20"));
    EXPECT_TRUE(loadedGraph.isVertex("30"));
    EXPECT_TRUE(loadedGraph.isVertex("40"));
    EXPECT_TRUE(loadedGraph.isVertex("50"));

    EXPECT_TRUE(loadedGraph.isEdge("10", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("30", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("40", "50"));

    EXPECT_FALSE(loadedGraph.isEdge("30", "10"));
    EXPECT_FALSE(loadedGraph.isEdge("40", "10"));
    EXPECT_FALSE(loadedGraph.isEdge("50", "10"));
    EXPECT_FALSE(loadedGraph.isEdge("30", "20"));
    EXPECT_FALSE(loadedGraph.isEdge("40", "20"));
    EXPECT_FALSE(loadedGraph.isEdge("50", "20"));
    EXPECT_FALSE(loadedGraph.isEdge("40", "30"));
    EXPECT_FALSE(loadedGraph.isEdge("50", "40"));

    remove("testGraph_tmp.txt");
}


TEST_F(SmallDirectedGraphChar, when_writingEdgeListInBinaryAndReloadIt_expect_graphContainsAllVerticesAndEdges){
    graph.writeEdgeListInBinaryFile("edgeList_tmp.bin");
    PGL::VertexLabeledDirectedGraph<unsigned char> loadedGraph = PGL::VertexLabeledDirectedGraph<unsigned char>::loadEdgeListFromBinaryFile("edgeList_tmp.bin");
    
    EXPECT_TRUE(loadedGraph.isVertex(10));
    EXPECT_TRUE(loadedGraph.isVertex(20));
    EXPECT_TRUE(loadedGraph.isVertex(30));
    EXPECT_TRUE(loadedGraph.isVertex(40));
    EXPECT_TRUE(loadedGraph.isVertex(50));

    EXPECT_TRUE(loadedGraph.isEdge(10, 30));
    EXPECT_TRUE(loadedGraph.isEdge(10, 40));
    EXPECT_TRUE(loadedGraph.isEdge(10, 40));
    EXPECT_TRUE(loadedGraph.isEdge(20, 30));
    EXPECT_TRUE(loadedGraph.isEdge(20, 40));
    EXPECT_TRUE(loadedGraph.isEdge(20, 50));
    EXPECT_TRUE(loadedGraph.isEdge(30, 40));
    EXPECT_TRUE(loadedGraph.isEdge(40, 50));

    EXPECT_FALSE(loadedGraph.isEdge(30, 10));
    EXPECT_FALSE(loadedGraph.isEdge(40, 10));
    EXPECT_FALSE(loadedGraph.isEdge(40, 10));
    EXPECT_FALSE(loadedGraph.isEdge(30, 20));
    EXPECT_FALSE(loadedGraph.isEdge(40, 20));
    EXPECT_FALSE(loadedGraph.isEdge(50, 20));
    EXPECT_FALSE(loadedGraph.isEdge(40, 30));
    EXPECT_FALSE(loadedGraph.isEdge(50, 40));

    remove("edgeList_tmp.bin");
}

TEST_F(SmallGraphString, when_writingStringEdgeListInBinary_except_throwLogicError){
    EXPECT_THROW(graph.writeEdgeListInBinaryFile("edgeList_tmp.bin"), logic_error);
    EXPECT_THROW(PGL::VertexLabeledUndirectedGraph<string>::loadEdgeListFromBinaryFile("edgeList_tmp.bin"), logic_error);
    remove("edgeList_tmp.bin");
}

TEST(loadFromEdgeListBinary, when_loadingNonExistingEdgeListBinary_expect_throwRuntimeError){
    EXPECT_THROW(PGL::VertexLabeledUndirectedGraph<bool>::loadEdgeListFromBinaryFile("\0"), runtime_error);
}

TEST(loadFromTextFile, when_loadingNonExistingEdgeListTextFile_expect_throwRuntimeError){
    EXPECT_THROW(PGL::VertexLabeledUndirectedGraph<bool>::loadEdgeListFromBinaryFile("\0"), runtime_error);
}

TEST_F(SmallGraphChar, when_writingVerticesBinaryAndReloadThem_except_graphContainsCorrectVertices){
    graph.writeVerticesInBinaryFile("verticesList_tmp.bin");
    PGL::VertexLabeledUndirectedGraph<unsigned char> loadedGraph;
    loadedGraph.addVerticesFromBinaryFile("verticesList_tmp.bin");

    EXPECT_TRUE(loadedGraph.isVertex(10));
    EXPECT_TRUE(loadedGraph.isVertex(20));
    EXPECT_TRUE(loadedGraph.isVertex(30));
    EXPECT_TRUE(loadedGraph.isVertex(40));
    EXPECT_TRUE(loadedGraph.isVertex(50));
    remove("verticesList_tmp.bin");
}


TEST(addVerticesFromBinaryFile, when_loadingNonExistingVerticesBinaryFile_expect_throwRuntimeError){
    PGL::VertexLabeledUndirectedGraph<bool> graph;
    EXPECT_THROW(graph.addVerticesFromBinaryFile("\0"), runtime_error);
}
