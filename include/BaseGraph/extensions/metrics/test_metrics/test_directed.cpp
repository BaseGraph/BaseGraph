#include <vector>
#include <list>
#include <utility>

#include "gtest/gtest.h"
#include "fixtures.hpp"
#include "BaseGraph/algorithms/graphpaths.h"
#include "BaseGraph/extensions/metrics/general.h"
#include "BaseGraph/extensions/metrics/directed.h"
#include "BaseGraph/extensions/metrics/undirected.h"


using namespace std;
using namespace BaseGraph;


TEST(HouseGraph_directed, when_findingTriangles_expect_returnAllUndirectedTriangles) {
    DirectedGraph graph(7);
    graph.addReciprocalEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(2, 1);
    graph.addEdgeIdx(3, 1);
    graph.addEdgeIdx(1, 4);
    graph.addEdgeIdx(2, 3);
    graph.addEdgeIdx(3, 4);
    graph.addEdgeIdx(5, 3);

    list<array<BaseGraph::VertexIndex, 3>> expectedTriangles = {{0, 2, 3}, {1, 2, 3}, {1, 3, 4}};
    EXPECT_EQ(metrics::findAllDirectedTriangles(graph), expectedTriangles);
}

static void expectClassifiedAs(map<string, size_t> spectrum, const string& triangleType) {
    for (const string& type: list<string>({"3cycle", "3nocycle", "4cycle", "4outward", "4inward", "5cycle", "6cycle"})) {
        if (type == triangleType)
            EXPECT_EQ(spectrum[type], 1) << "With type=\"" << type << "\"\n";
        else
            EXPECT_EQ(spectrum[type], 0) << "With type=\"" << type << "\"\n";
    }
}

static void underRotationsExpectClassifyTriangleAs(
        const DirectedGraph& graph, const array<BaseGraph::VertexIndex, 3>& triangle, const string& triangleType) {
    array<BaseGraph::VertexIndex, 3> rotatedTriangle;
    for (const auto& rotationIdx: list<array<BaseGraph::VertexIndex, 3>>({{0, 1, 2}, {1, 2, 0}, {2, 0, 1}}) ) {
        for (BaseGraph::VertexIndex idx=0; idx<3; idx++)
            rotatedTriangle[idx] = triangle[rotationIdx[idx]];

        expectClassifiedAs(metrics::getTriangleSpectrum(graph, {triangle}), triangleType);
    }
}

TEST(DirectedTriangleSpectrum, when_clockwiseCycles_expect_classifiesTrianglesProperly) {
    DirectedGraph graph(10);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(1, 2);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(1, 3);
    graph.addEdgeIdx(2, 3);

    graph.addEdgeIdx(3, 4);
    graph.addReciprocalEdgeIdx(4, 5);
    graph.addEdgeIdx(5, 3);
    graph.addEdgeIdx(4, 6);
    graph.addEdgeIdx(5, 6);

    graph.addEdgeIdx(6, 7);
    graph.addReciprocalEdgeIdx(7, 8);
    graph.addReciprocalEdgeIdx(8, 6);
    graph.addReciprocalEdgeIdx(7, 9);
    graph.addReciprocalEdgeIdx(8, 9);

    underRotationsExpectClassifyTriangleAs(graph, {0, 1, 2}, "3cycle");
    underRotationsExpectClassifyTriangleAs(graph, {1, 2, 3}, "3nocycle");
    underRotationsExpectClassifyTriangleAs(graph, {3, 4, 5}, "4cycle");
    underRotationsExpectClassifyTriangleAs(graph, {4, 5, 6}, "4outward");
    underRotationsExpectClassifyTriangleAs(graph, {6, 7, 8}, "5cycle");
    underRotationsExpectClassifyTriangleAs(graph, {7, 8, 9}, "6cycle");
}

TEST(DirectedTriangleSpectrum, when_counterClockwiseCycles_expect_classifiesTrianglesProperly) {
    DirectedGraph graph(9);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(2, 1);
    graph.addEdgeIdx(1, 0);
    graph.addEdgeIdx(3, 1);
    graph.addEdgeIdx(3, 2);

    graph.addEdgeIdx(3, 5);
    graph.addReciprocalEdgeIdx(5, 4);
    graph.addEdgeIdx(4, 3);
    graph.addEdgeIdx(6, 4);
    graph.addEdgeIdx(6, 5);

    graph.addEdgeIdx(7, 6);
    graph.addReciprocalEdgeIdx(7, 8);
    graph.addReciprocalEdgeIdx(8, 6);

    underRotationsExpectClassifyTriangleAs(graph, {0, 1, 2}, "3cycle");
    underRotationsExpectClassifyTriangleAs(graph, {1, 2, 3}, "3nocycle");
    underRotationsExpectClassifyTriangleAs(graph, {3, 4, 5}, "4cycle");
    underRotationsExpectClassifyTriangleAs(graph, {4, 5, 6}, "4inward");
    underRotationsExpectClassifyTriangleAs(graph, {6, 7, 8}, "5cycle");
}

TEST(directedDensity, when_fiveEdgesAndNodes_expectDensityOfAQuarter){
    DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(0, 4);
    graph.addEdgeIdx(1, 4);
    EXPECT_EQ(metrics::getDensity(graph), 0.25);
}

TEST(reciprocity, when_HalfReciprocitalEdges_expectHalf){
    DirectedGraph graph(5);
    graph.addReciprocalEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(1, 3);

    EXPECT_EQ(metrics::getReciprocity(graph), .5);
}

TEST(reciprocities, when_twoReciprocitalEdges_expectOne){
    DirectedGraph graph(5);
    graph.addReciprocalEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 0);
    graph.addReciprocalEdgeIdx(1, 3);

    auto reciprocalDegrees = metrics::getReciprocalDegrees(graph);
    EXPECT_EQ(reciprocalDegrees[0], 1);
    EXPECT_EQ(reciprocalDegrees[1], 2);
    EXPECT_EQ(reciprocalDegrees[2], 0);
    EXPECT_EQ(reciprocalDegrees[3], 1);
    EXPECT_EQ(reciprocalDegrees[4], 0);
}

TEST(jaccardReciprocity, expect_correctReciprocities){
    DirectedGraph graph(5);
    graph.addReciprocalEdgeIdx(0, 2);
    graph.addEdgeIdx(2, 3);
    graph.addReciprocalEdgeIdx(1, 3);
    graph.addReciprocalEdgeIdx(1, 4);
    graph.addEdgeIdx(4, 3);

    auto jaccardReciprocity = metrics::getJaccardReciprocities(graph);
    EXPECT_EQ(jaccardReciprocity[0], 1);
    EXPECT_EQ(jaccardReciprocity[1], 1);
    EXPECT_EQ(jaccardReciprocity[2], (double)1/2 );
    EXPECT_EQ(jaccardReciprocity[3], (double)1/3 );
    EXPECT_EQ(jaccardReciprocity[4], (double)1/2 );
}

TEST(reciprocityRatios, expect_correctReciprocities){
    DirectedGraph graph(5);
    graph.addReciprocalEdgeIdx(0, 2);
    graph.addEdgeIdx(2, 3);
    graph.addReciprocalEdgeIdx(1, 3);
    graph.addReciprocalEdgeIdx(1, 4);
    graph.addEdgeIdx(4, 3);

    auto reciprocityRatios = metrics::getReciprocityRatios(graph);
    EXPECT_EQ(reciprocityRatios[0], 1);
    EXPECT_EQ(reciprocityRatios[1], 1);
    EXPECT_EQ(reciprocityRatios[2], (double)2/3 );
    EXPECT_EQ(reciprocityRatios[3], (double)1/2 );
    EXPECT_EQ(reciprocityRatios[4], (double)2/3 );
}

TEST_F(DirectedHouseGraph, expect_correctDirectedLocalClustering) {
    auto localClustering = metrics::getUndirectedLocalClusteringCoefficients(graph);
    vector<double> expectedValues = {1., 4/6., 4/6., 6/20., 1., 0, 0};
    EXPECT_EQ(localClustering, expectedValues);
}

TEST_F(DirectedHouseGraph, expect_correctGlobalClustering) {
    EXPECT_EQ(metrics::getUndirectedGlobalClusteringCoefficient(graph), 9./(9+9));
}

TEST_F(DirectedHouseGraph, expect_correctOutDegreeHistogram) {
    auto outDegreeHistogram = metrics::getOutDegreeHistogram(graph);
    map<size_t, size_t> expectedValues = {{0,2}, {1,3}, {2,1}, {4,1}};
    EXPECT_EQ(outDegreeHistogram, expectedValues);
}

TEST_F(DirectedHouseGraph, expect_correctInDegreeHistogram) {
    auto inDegreeHistogram = metrics::getInDegreeHistogram(graph);
    map<size_t, size_t> expectedValues = {{0,1}, {1,4}, {2,1}, {3,1}};
    EXPECT_EQ(inDegreeHistogram, expectedValues);
}
