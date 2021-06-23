#include <vector>
#include <list>
#include <utility>

#include "gtest/gtest.h"
#include "pgl/vertexlabeled_undirectedgraph.hpp"
#include "pgl/metrics/general.h"
#include "pgl/metrics/directed.h"
#include "pgl/metrics/undirected.h"


using namespace std;
using namespace PGL;


class UndirectedHouseGraph: public::testing::Test{
    /*
     * (0)     (1)
     *  | \   / | \
     *  |  \ /  |  \
     *  |   X   |  (4)
     *  |  / \  |  /
     *  | /   \ | /
     * (2)-----(3)-----(5)
     *
     *      (6)
     */
    public:
        UndirectedGraph graph = UndirectedGraph(7);
        void SetUp() {
            graph.addEdgeIdx(0, 2);
            graph.addEdgeIdx(0, 3);
            graph.addEdgeIdx(1, 2);
            graph.addEdgeIdx(1, 3);
            graph.addEdgeIdx(1, 4);
            graph.addEdgeIdx(2, 3);
            graph.addEdgeIdx(3, 4);
            graph.addEdgeIdx(3, 5);
        }
};

class DirectedHouseGraph: public::testing::Test{
    /*
     * (0)_    (1)
     *  ||\   / | \
     *  |  \ /  |  V
     *  |   X   |  (4)
     *  |  / \  |  /
     *  V V   \ | V
     *  (2)---->(3)---->(5)
     *
     *      (6)
     */
    public:
        DirectedGraph graph = DirectedGraph(7);
        void SetUp() {
            graph.addEdgeIdx(0, 2);
            graph.addEdgeIdx(3, 0);
            graph.addEdgeIdx(1, 2);
            graph.addEdgeIdx(3, 1);
            graph.addEdgeIdx(1, 4);
            graph.addEdgeIdx(2, 3);
            graph.addEdgeIdx(4, 3);
            graph.addEdgeIdx(3, 5);
        }
};


class TreeLikeGraph: public::testing::Test{
    /*
     *        (0)
     *       /   \
     *     (1)   (2)
     *    /   \ /   \
     *   (3)  (4)  (5)
     *      \  |  /
     *        (6)
     *         |
     *        (7)
     */
    public:
        UndirectedGraph graph = UndirectedGraph(8);
        void SetUp(){
            graph.addEdgeIdx(0, 1);
            graph.addEdgeIdx(0, 2);
            graph.addEdgeIdx(1, 3);
            graph.addEdgeIdx(1, 4);
            graph.addEdgeIdx(2, 4);
            graph.addEdgeIdx(2, 5);
            graph.addEdgeIdx(3, 6);
            graph.addEdgeIdx(4, 6);
            graph.addEdgeIdx(5, 6);
            graph.addEdgeIdx(6, 7);
        }
};

class ThreeComponentsGraph: public::testing::Test{
    /*
     *        (0)--(1)--(2)--(3)
     *
     *           (7)--(8)
     *           /  \
     *         (6)  (9)
     *        /   \
     *      (4)---(5)     (10)
     */
    public:
        UndirectedGraph graph = UndirectedGraph(11);
        void SetUp(){
            graph.addEdgeIdx(0, 1);
            graph.addEdgeIdx(1, 2);
            graph.addEdgeIdx(2, 3);

            graph.addEdgeIdx(4, 5);
            graph.addEdgeIdx(5, 6);
            graph.addEdgeIdx(6, 4);
            graph.addEdgeIdx(6, 7);
            graph.addEdgeIdx(7, 8);
            graph.addEdgeIdx(7, 9);
        }
};

TEST_F(UndirectedHouseGraph, when_callingGetDegree_expect_returnsCorrectDegrees){
    EXPECT_EQ(graph.getDegreeIdx(0), 2);
    EXPECT_EQ(graph.getDegreeIdx(1), 3);
    EXPECT_EQ(graph.getDegreeIdx(2), 3);
    EXPECT_EQ(graph.getDegreeIdx(3), 5);
    EXPECT_EQ(graph.getDegreeIdx(4), 2);
    EXPECT_EQ(graph.getDegreeIdx(5), 1);
    EXPECT_EQ(graph.getDegreeIdx(6), 0);
}

TEST_F(UndirectedHouseGraph, when_callingGetDegrees_expect_returnsCorrectDegrees){
    vector<size_t> returnedDegrees(graph.getDegrees());
    vector<size_t> expectedDegrees({2, 3, 3, 5, 2, 1, 0});

    for (size_t i=0; i<expectedDegrees.size(); i++)
        EXPECT_EQ(expectedDegrees[i], returnedDegrees[i]);
}

TEST_F(UndirectedHouseGraph, when_findingPredecessors_expect_returnsCorrectPathsLengthsAndPredecessors){
    auto shortestPaths = findPredecessorsOfVertexIdx(graph, 4);
    EXPECT_EQ(shortestPaths.first[0], 2);
    EXPECT_EQ(shortestPaths.first[1], 1);
    EXPECT_EQ(shortestPaths.first[2], 2);
    EXPECT_EQ(shortestPaths.first[3], 1);
    EXPECT_EQ(shortestPaths.first[4], 0);
    EXPECT_EQ(shortestPaths.first[5], 2);
    EXPECT_EQ(shortestPaths.first[6], PGL_SIZE_T_MAX);

    EXPECT_EQ(shortestPaths.second[0], 3);
    EXPECT_EQ(shortestPaths.second[1], 4);
    EXPECT_TRUE(shortestPaths.second[2] == 1 || shortestPaths.second[2] == 3);
    EXPECT_EQ(shortestPaths.second[3], 4);
    EXPECT_EQ(shortestPaths.second[4], PGL_SIZE_T_MAX);
    EXPECT_EQ(shortestPaths.second[5], 3);
    EXPECT_EQ(shortestPaths.second[6], PGL_SIZE_T_MAX);

}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessor_expect_correctPath){
    auto shortestPaths = findPredecessorsOfVertexIdx(graph, 4);

    EXPECT_EQ(findPathToVertexFromPredecessorsIdx(graph, 0, shortestPaths),
            list<size_t>({4, 3, 0}));

    EXPECT_EQ(findPathToVertexFromPredecessorsIdx(graph, 5, shortestPaths),
            list<size_t>({4, 3, 5}));
}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessorToIsolatedVertex_expect_throwRuntimeError){
    auto shortestPaths = findPredecessorsOfVertexIdx(graph, 4);
    EXPECT_THROW(findPathToVertexFromPredecessorsIdx(graph, 6, shortestPaths), std::runtime_error);
}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessorFromIsolatedVertex_expect_throwRuntimeError){
    auto shortestPaths = findPredecessorsOfVertexIdx(graph, 6);
    EXPECT_THROW(findPathToVertexFromPredecessorsIdx(graph, 0, shortestPaths), std::runtime_error);
}

TEST_F(TreeLikeGraph, when_findingAllPredecessors_expect_returnEveryPredecessor){
    auto shortestPaths = findAllPredecessorsOfVertexIdx(graph, 0).second;

    EXPECT_EQ(shortestPaths[7], list<size_t>({6}));
    EXPECT_EQ(shortestPaths[6], list<size_t>({3, 4, 5}));
    EXPECT_EQ(shortestPaths[5], list<size_t>({2}));
    EXPECT_EQ(shortestPaths[4], list<size_t>({1, 2}));
    EXPECT_EQ(shortestPaths[3], list<size_t>({1}));
    EXPECT_EQ(shortestPaths[2], list<size_t>({0}));
    EXPECT_EQ(shortestPaths[1], list<size_t>({0}));
}

TEST_F(TreeLikeGraph, when_findingAllPredecessors_expect_returnEveryPath){
    auto shortestPaths = findAllPredecessorsOfVertexIdx(graph, 0);
    list<list<size_t>> geodesics = findMultiplePathsToVertexFromPredecessorsIdx(graph, 4, shortestPaths);

    EXPECT_EQ(geodesics, list<list<size_t>>({{0, 2, 4}, {0, 1, 4}}));

    geodesics = findMultiplePathsToVertexFromPredecessorsIdx(graph, 7, shortestPaths);
    EXPECT_EQ(geodesics, list<list<size_t>>(
                {{0, 2, 5, 6, 7}, {0, 2, 4, 6, 7},
                {0, 1, 4, 6, 7}, {0, 1, 3, 6, 7} }));

    geodesics = findMultiplePathsToVertexFromPredecessorsIdx(graph, 1, shortestPaths);
    EXPECT_EQ(geodesics, list<list<size_t>>( {{0, 1}} ));
}

TEST_F(UndirectedHouseGraph, when_findingConnectedComponents_expect_returnsCorrectComponents){
    list<list<size_t>> components = findConnectedComponents(graph);
    list<list<size_t>>::iterator component = components.begin();

    EXPECT_EQ(*component, list<size_t>({0, 2, 3, 1, 4, 5}));
    component++;
    EXPECT_EQ(*component, list<size_t>({6}));
}

TEST_F(ThreeComponentsGraph, when_findingAverageShortestPaths_expect_returnCorrectAverages) {
    vector<double> averageShortestPaths = getAverageShortestPaths(graph);
    EXPECT_EQ(averageShortestPaths[0], 2);
    EXPECT_EQ(averageShortestPaths[1], 4/3.);
    EXPECT_EQ(averageShortestPaths[2], 4/3.);
    EXPECT_EQ(averageShortestPaths[3], 2);

    EXPECT_EQ(averageShortestPaths[4], 10/5.);
    EXPECT_EQ(averageShortestPaths[5], 10/5.);
    EXPECT_EQ(averageShortestPaths[6], 7/5.);
    EXPECT_EQ(averageShortestPaths[7], 7/5.);
    EXPECT_EQ(averageShortestPaths[8], 11/5.);
    EXPECT_EQ(averageShortestPaths[9], 11/5.);

    EXPECT_EQ(averageShortestPaths[10], 0);
}

TEST_F(ThreeComponentsGraph, when_findingShortestPathsDistribution_expect_returnCorrectDistribution) {
    auto shortestPathDistribution = getShortestPathsDistribution(graph);

    EXPECT_EQ(shortestPathDistribution[0], vector<double>({0, 6/4., 4/4., 2/4.}));
    EXPECT_EQ(shortestPathDistribution[1], vector<double>({0, 12/6., 10/6., 8/6.}));
    EXPECT_EQ(shortestPathDistribution[2], vector<double>({0}));
}

TEST_F(UndirectedHouseGraph, when_findingClosenessCentrality_expect_returnsCorrectCentrality){
    EXPECT_EQ(getClosenessCentralityOfVertexIdx(graph, 4), 5./8);
}

TEST_F(UndirectedHouseGraph, when_findingHarmonicMeanGeodesic_expect_returnsCorrectMean){
    EXPECT_EQ(getHarmonicMeanGeodesicOfVertexIdx(graph, 4), 0.7);
}

TEST_F(TreeLikeGraph, when_findingDiameters_expect_correctDiameters){
    vector<size_t> diameters = getDiameters(graph);
    EXPECT_EQ(diameters[0], 4);
    EXPECT_EQ(diameters[1], 3);
    EXPECT_EQ(diameters[2], 3);
    EXPECT_EQ(diameters[3], 3);
    EXPECT_EQ(diameters[4], 2);
    EXPECT_EQ(diameters[5], 3);
    EXPECT_EQ(diameters[6], 3);
    EXPECT_EQ(diameters[7], 4);
}

TEST_F(TreeLikeGraph, expect_correctBetweenesses){
    std::vector<double> betweenesses = getBetweennesses(graph, true);
    std::vector<double> expectedValues = {
        1, 3.5, 3.5, 1.75, 4.5, 1.75, 9, 0
    };
    EXPECT_EQ(betweenesses, expectedValues);
}

TEST_F(UndirectedHouseGraph, expect_correctTriangleCount){
    EXPECT_EQ(countTrianglesAroundVertexIdx(graph, 0), 1);
    EXPECT_EQ(countTrianglesAroundVertexIdx(graph, 1), 2);
    EXPECT_EQ(countTrianglesAroundVertexIdx(graph, 2), 2);
    EXPECT_EQ(countTrianglesAroundVertexIdx(graph, 3), 3);
    EXPECT_EQ(countTrianglesAroundVertexIdx(graph, 4), 1);
    EXPECT_EQ(countTrianglesAroundVertexIdx(graph, 5), 0);
    EXPECT_EQ(countTrianglesAroundVertexIdx(graph, 6), 0);
}

TEST_F(UndirectedHouseGraph, when_countingTriangles_expect_correctTriangleNumber){
    EXPECT_EQ(countTriangles(graph), 3);
}

TEST_F(UndirectedHouseGraph, when_findingTriangles_expect_returnsAllTriangles){
    list<array<size_t, 3>> expectedTriangles = {{0, 2, 3}, {1, 2, 3}, {1, 3, 4}};
    EXPECT_EQ(findAllTriangles(graph), expectedTriangles);
}

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

    list<array<size_t, 3>> expectedTriangles = {{0, 2, 3}, {1, 2, 3}, {1, 3, 4}};
    EXPECT_EQ(findAllDirectedTriangles(graph), expectedTriangles);
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
        const DirectedGraph& graph, const array<size_t, 3>& triangle, const string& triangleType) {
    array<size_t, 3> rotatedTriangle;
    for (const auto& rotationIdx: list<array<size_t, 3>>({{0, 1, 2}, {1, 2, 0}, {2, 0, 1}}) ) {
        for (size_t idx=0; idx<3; idx++)
            rotatedTriangle[idx] = triangle[rotationIdx[idx]];

        expectClassifiedAs(getTriangleSpectrum(graph, {triangle}), triangleType);
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

TEST_F(UndirectedHouseGraph, when_findingRedundancy_expect_correctRedundancies){
    vector<double> redundancy = getRedundancy(graph);
    EXPECT_EQ(redundancy[0], 1);
    EXPECT_EQ(redundancy[1], double(4/3.0));
    EXPECT_EQ(redundancy[2], double(4/3.0));
    EXPECT_EQ(redundancy[3], 1.2);
    EXPECT_EQ(redundancy[4], 1);
    EXPECT_EQ(redundancy[5], 0);
    EXPECT_EQ(redundancy[6], 0);
}

TEST_F(UndirectedHouseGraph, when_findingKShellsAndOnionLayer_expect_correctAnswers){
    auto kshells_onionLayer = getKShellsAndOnionLayers(graph);
    EXPECT_EQ(kshells_onionLayer.first, vector<size_t>({2, 2, 2, 2, 2, 1, 0}));
    EXPECT_EQ(kshells_onionLayer.second, vector<size_t>({3, 4, 4, 4, 3, 2, 1}));
}

TEST_F(UndirectedHouseGraph, when_finding2Core_expect_vertices567){
    graph.addEdgeIdx(0, 1); // Forms a 3-Core with vertices 1-2-3-4
    EXPECT_EQ(getKCore(graph, 2), list<size_t> ({4,5,6}));
}

TEST_F(UndirectedHouseGraph, when_findingOnionSpectrum_expect_correctSpectrum) {
    auto onionSpectrum = getOnionSpectrum(graph);
    vector<list<double>> expectedSpectrum ({ {1/7.}, {1/7.}, {2/7., 3/7.} });
    EXPECT_EQ(onionSpectrum, expectedSpectrum);
}

TEST(directedDensity, when_fiveEdgesAndNodes_expectDensityOfAQuarter){
    DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(0, 4);
    graph.addEdgeIdx(1, 4);
    EXPECT_EQ(getDensity(graph), 0.25);
}

TEST(reciprocity, when_HalfReciprocitalEdges_expectHalf){
    DirectedGraph graph(5);
    graph.addReciprocalEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 0);
    graph.addEdgeIdx(1, 3);

    EXPECT_EQ(PGL::getReciprocity(graph), .5);
}

TEST(reciprocities, when_twoReciprocitalEdges_expectOne){
    DirectedGraph graph(5);
    graph.addReciprocalEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 0);
    graph.addReciprocalEdgeIdx(1, 3);

    auto reciprocalDegrees = getReciprocalDegrees(graph);
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

    auto jaccardReciprocity = getJaccardReciprocities(graph);
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

    auto reciprocityRatios = getReciprocityRatios(graph);
    EXPECT_EQ(reciprocityRatios[0], 1);
    EXPECT_EQ(reciprocityRatios[1], 1);
    EXPECT_EQ(reciprocityRatios[2], (double)2/3 );
    EXPECT_EQ(reciprocityRatios[3], (double)1/2 );
    EXPECT_EQ(reciprocityRatios[4], (double)2/3 );
}

TEST_F(UndirectedHouseGraph, when_findingDegreeDistribution_expect_returnCorrectDistribution) {
    auto degreeDistribution = getDegreeDistribution(graph);
    EXPECT_EQ(degreeDistribution, vector<double> ({2./7, 3./7, 3./7, 5./7, 2./7, 1./7, 0.}) );
}

TEST_F(UndirectedHouseGraph, when_computingHarmonicCentrality_expect_correctAnswer) {
    vector<double> expectedValues {
                         0.5+1+1+0.5+0.5,
                         0.5+1+1+1+0.5,
                         1+1+1+0.5+0.5,
                         1+1+1+1+1,
                         0.5+1+0.5+1+0.5,
                         0.5+0.5+0.5+1+0.5,
                         0};

    for (size_t i=0; i<graph.getSize(); i++)
        EXPECT_EQ(getHarmonicCentralityOfVertexIdx(graph, i), expectedValues[i]);
}

TEST_F(UndirectedHouseGraph, when_computingLocalClusteringCoefficients_expect_correctAnswers) {
    vector<double> localClustering = getLocalClusteringCoefficients(graph);
    vector<double> expectedValues = {1., 4/6., 4/6., 6/20., 1., 0, 0};
    EXPECT_EQ(localClustering, expectedValues);
}

TEST_F(DirectedHouseGraph, expect_correctDirectedLocalClustering) {
    auto localClustering = getUndirectedLocalClusteringCoefficients(graph);
    vector<double> expectedValues = {1., 4/6., 4/6., 6/20., 1., 0, 0};
    EXPECT_EQ(localClustering, expectedValues);
}

TEST_F(UndirectedHouseGraph, when_computingClusteringSpectrum_expect_correctAnswers) {
    graph.addEdgeIdx(5, 6); // make the average not trivial (same local clustering for every degree)
    vector<double> clusteringSpectrum = getClusteringSpectrum(graph);
    EXPECT_EQ(clusteringSpectrum, vector<double>({0, 0, 2/3., 4/6., 0, 6/20.}));
}

TEST_F(UndirectedHouseGraph, when_computingGlobalClusteringCoefficient_expect_correctAnswer) {
    EXPECT_EQ(getGlobalClusteringCoefficient(graph), 9./(9+9));
}

TEST_F(DirectedHouseGraph, expect_correctGlobalClustering) {
    EXPECT_EQ(getUndirectedGlobalClusteringCoefficient(graph), 9./(9+9));
}

TEST_F(UndirectedHouseGraph, when_findingVertexNeighourhoodDegrees_expect_correctDegrees) {
    EXPECT_TRUE(
            getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({2, 3, 5}) ||
            getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({2, 5, 3}) ||
            getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({3, 2, 5}) ||
            getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({3, 5, 2}) ||
            getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({5, 2, 3}) ||
            getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({5, 3, 2}));
}

TEST_F(UndirectedHouseGraph, when_computingNeighbourDegreeSpectrum_expect_correctAnswer) {
    vector<double> degreeSpectrum = getNeighbourDegreeSpectrum(graph);
    vector<double> averageNeighbourDegrees = {(3+5)/2., (3+5+2)/3., (2+3+5)/3., (2+3+3+2+1)/5., (3+5)/2., 5, 0};
    EXPECT_EQ(degreeSpectrum, averageNeighbourDegrees);
}

TEST_F(UndirectedHouseGraph, when_computingNormalizedNeighbourDegreeSpectrum_expect_correctAnswer) {
    vector<double> degreeSpectrum = getNeighbourDegreeSpectrum(graph, true);
    vector<double> averageNeighbourDegrees = {(3+5)/2., (3+5+2)/3., (2+3+5)/3., (2+3+3+2+1)/5., (3+5)/2., 5, 0};

    size_t firstMoment(2+3+3+5+2+1), secondMoment(2*2+3*3+3*3+5*5+2*2+1);
    for (size_t i=0; i<graph.getSize(); i++)
        EXPECT_EQ(degreeSpectrum[i], averageNeighbourDegrees[i]*firstMoment/secondMoment);
}

TEST_F(UndirectedHouseGraph, when_computingDegreeCorrelation_expect_correctValue) {
    EXPECT_EQ(getDegreeCorrelation(graph, 16/7.), -629/999.);
}

TEST_F(UndirectedHouseGraph, when_computingModularity_expectCorrectValue) {
    EXPECT_EQ(4/8. - 100/256. - 25/256. - 1/256.,
            getModularity(graph, vector<size_t>({0, 1, 0, 0, 1, 2, 1})) );
}

TEST_F(DirectedHouseGraph, expect_correctOutDegreeHistogram) {
    auto outDegreeHistogram = getOutDegreeHistogram(graph);
    map<size_t, size_t> expectedValues = {{0,2}, {1,3}, {2,1}, {3,1}};
    EXPECT_EQ(outDegreeHistogram, expectedValues);
}

TEST_F(DirectedHouseGraph, expect_correctInDegreeHistogram) {
    auto inDegreeHistogram = getInDegreeHistogram(graph);
    map<size_t, size_t> expectedValues = {{0,1}, {1,4}, {2,2}};
    EXPECT_EQ(inDegreeHistogram, expectedValues);
}
