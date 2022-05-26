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


TEST_F(UndirectedHouseGraph, when_findingPredecessors_expect_returnsCorrectPathsLengthsAndPredecessors){
    auto shortestPaths = algorithms::findPredecessorsOfVertexIdx(graph, 4);
    EXPECT_EQ(shortestPaths.first[0], 2);
    EXPECT_EQ(shortestPaths.first[1], 1);
    EXPECT_EQ(shortestPaths.first[2], 2);
    EXPECT_EQ(shortestPaths.first[3], 1);
    EXPECT_EQ(shortestPaths.first[4], 0);
    EXPECT_EQ(shortestPaths.first[5], 2);
    EXPECT_EQ(shortestPaths.first[6], algorithms::SIZE_T_MAX);

    EXPECT_EQ(shortestPaths.second[0], 3);
    EXPECT_EQ(shortestPaths.second[1], 4);
    EXPECT_TRUE(shortestPaths.second[2] == 1 || shortestPaths.second[2] == 3);
    EXPECT_EQ(shortestPaths.second[3], 4);
    EXPECT_EQ(shortestPaths.second[4], algorithms::SIZE_T_MAX);
    EXPECT_EQ(shortestPaths.second[5], 3);
    EXPECT_EQ(shortestPaths.second[6], algorithms::SIZE_T_MAX);

}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessor_expect_correctPath){
    auto shortestPaths = algorithms::findPredecessorsOfVertexIdx(graph, 4);

    EXPECT_EQ(algorithms::findPathToVertexFromPredecessorsIdx(graph, 0, shortestPaths),
            list<BaseGraph::VertexIndex>({4, 3, 0}));

    EXPECT_EQ(algorithms::findPathToVertexFromPredecessorsIdx(graph, 5, shortestPaths),
            list<BaseGraph::VertexIndex>({4, 3, 5}));
}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessorToIsolatedVertex_expect_throwRuntimeError){
    auto shortestPaths = algorithms::findPredecessorsOfVertexIdx(graph, 4);
    EXPECT_THROW(algorithms::findPathToVertexFromPredecessorsIdx(graph, 6, shortestPaths), std::runtime_error);
}

TEST_F(UndirectedHouseGraph, when_findingPathFromPredecessorFromIsolatedVertex_expect_throwRuntimeError){
    auto shortestPaths = algorithms::findPredecessorsOfVertexIdx(graph, 6);
    EXPECT_THROW(algorithms::findPathToVertexFromPredecessorsIdx(graph, 0, shortestPaths), std::runtime_error);
}

TEST_F(TreeLikeGraph, when_findingAllPredecessors_expect_returnEveryPredecessor){
    auto shortestPaths = algorithms::findAllPredecessorsOfVertexIdx(graph, 0).second;

    EXPECT_EQ(shortestPaths[7], algorithms::Path({6}));
    EXPECT_EQ(shortestPaths[6], algorithms::Path({3, 4, 5}));
    EXPECT_EQ(shortestPaths[5], algorithms::Path({2}));
    EXPECT_EQ(shortestPaths[4], algorithms::Path({1, 2}));
    EXPECT_EQ(shortestPaths[3], algorithms::Path({1}));
    EXPECT_EQ(shortestPaths[2], algorithms::Path({0}));
    EXPECT_EQ(shortestPaths[1], algorithms::Path({0}));
}

TEST_F(TreeLikeGraph, when_findingAllPredecessors_expect_returnEveryPath){
    auto shortestPaths = algorithms::findAllPredecessorsOfVertexIdx(graph, 0);
    auto geodesics = algorithms::findMultiplePathsToVertexFromPredecessorsIdx(graph, 4, shortestPaths);

    EXPECT_EQ(geodesics, algorithms::MultiplePaths({{0, 2, 4}, {0, 1, 4}}));

    geodesics = algorithms::findMultiplePathsToVertexFromPredecessorsIdx(graph, 7, shortestPaths);
    EXPECT_EQ(geodesics, algorithms::MultiplePaths(
                {{0, 2, 5, 6, 7}, {0, 2, 4, 6, 7},
                {0, 1, 4, 6, 7}, {0, 1, 3, 6, 7} }));

    geodesics = algorithms::findMultiplePathsToVertexFromPredecessorsIdx(graph, 1, shortestPaths);
    EXPECT_EQ(geodesics, list<list<BaseGraph::VertexIndex>>( {{0, 1}} ));
}

TEST_F(UndirectedHouseGraph, when_findingConnectedComponents_expect_returnsCorrectComponents){
    list<BaseGraph::metrics::Component> components = metrics::findConnectedComponents(graph);
    auto component = components.begin();

    EXPECT_EQ(*component, BaseGraph::metrics::Component({0, 2, 3, 1, 4, 5}));
    component++;
    EXPECT_EQ(*component, BaseGraph::metrics::Component({6}));
}

TEST_F(ThreeComponentsGraph, when_findingAverageShortestPaths_expect_returnCorrectAverages) {
    vector<double> averageShortestPaths = metrics::getShortestPathAverages(graph);
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
    auto shortestPathDistribution = metrics::getShortestPathsDistribution(graph);

    vector<unordered_map<size_t, double>> expectedValues = {
         {{1,6/4.}, {2,4/4.}, {3,2/4.}},
         {{1,12/6.}, {2,10/6.}, {3,8/6.}},
         {}
    };
    EXPECT_EQ(shortestPathDistribution, expectedValues);
}

TEST_F(UndirectedHouseGraph, when_findingClosenessCentrality_expect_returnsCorrectCentrality){
    std::vector<double> expectedValues = {
        5./8, 5./7, 5./7, 1, 5./8, 5./9, 0
    };
    EXPECT_EQ(metrics::getClosenessCentralities(graph), expectedValues);
}

TEST_F(UndirectedHouseGraph, when_findingHarmonicMeanGeodesic_expect_returnsCorrectMean){
    std::vector<double> expectedValues = {
        .7, 4./5, 4./5, 1, .7, 3./5, 0
    };
    EXPECT_EQ(metrics::getShortestPathHarmonicAverages(graph), expectedValues);
}

TEST_F(TreeLikeGraph, when_findingDiameters_expect_correctDiameters){
    vector<size_t> diameters = metrics::getDiameters(graph);
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
    std::vector<double> betweenesses = metrics::getBetweennessCentralities(graph, true);
    std::vector<double> expectedValues = {
        1, 3.5, 3.5, 1.75, 4.5, 1.75, 9, 0
    };
    EXPECT_EQ(betweenesses, expectedValues);
}

TEST_F(UndirectedHouseGraph, expect_correctTriangleCount){
    EXPECT_EQ(metrics::countTrianglesAroundVertexIdx(graph, 0), 1);
    EXPECT_EQ(metrics::countTrianglesAroundVertexIdx(graph, 1), 2);
    EXPECT_EQ(metrics::countTrianglesAroundVertexIdx(graph, 2), 2);
    EXPECT_EQ(metrics::countTrianglesAroundVertexIdx(graph, 3), 3);
    EXPECT_EQ(metrics::countTrianglesAroundVertexIdx(graph, 4), 1);
    EXPECT_EQ(metrics::countTrianglesAroundVertexIdx(graph, 5), 0);
    EXPECT_EQ(metrics::countTrianglesAroundVertexIdx(graph, 6), 0);
}

TEST_F(UndirectedHouseGraph, when_countingTriangles_expect_correctTriangleNumber){
    EXPECT_EQ(metrics::countTriangles(graph), 3);
}

TEST_F(UndirectedHouseGraph, when_findingTriangles_expect_returnsAllTriangles){
    list<array<BaseGraph::VertexIndex, 3>> expectedTriangles = {{0, 2, 3}, {1, 2, 3}, {1, 3, 4}};
    EXPECT_EQ(metrics::findAllTriangles(graph), expectedTriangles);
}

TEST_F(UndirectedHouseGraph, when_findingRedundancy_expect_correctRedundancies){
    vector<double> redundancy = metrics::getRedundancy(graph);
    EXPECT_EQ(redundancy[0], 1);
    EXPECT_EQ(redundancy[1], double(4/3.0));
    EXPECT_EQ(redundancy[2], double(4/3.0));
    EXPECT_EQ(redundancy[3], 1.2);
    EXPECT_EQ(redundancy[4], 1);
    EXPECT_EQ(redundancy[5], 0);
    EXPECT_EQ(redundancy[6], 0);
}

TEST_F(UndirectedHouseGraph, when_findingKShellsAndOnionLayer_expect_correctAnswers){
    auto kshells_onionLayer = metrics::getKShellsAndOnionLayers(graph);
    EXPECT_EQ(kshells_onionLayer.first, vector<size_t>({2, 2, 2, 2, 2, 1, 0}));
    EXPECT_EQ(kshells_onionLayer.second, vector<size_t>({3, 4, 4, 4, 3, 2, 1}));
}

TEST_F(UndirectedHouseGraph, when_finding2Core_expect_vertices567){
    graph.addEdgeIdx(0, 1); // Forms a 3-Core with vertices 1-2-3-4
    EXPECT_EQ(metrics::getKCore(graph, 2), list<BaseGraph::VertexIndex> ({4,5,6}));
}

TEST_F(UndirectedHouseGraph, when_findingOnionSpectrum_expect_correctSpectrum) {
    auto onionSpectrum = metrics::getOnionSpectrum(graph);
    unordered_map<size_t, list<double>> expectedSpectrum {
        {0, {1/7.}}, {1, {1/7.}}, {2, {2/7.,3/7.}} };
    EXPECT_EQ(onionSpectrum, expectedSpectrum);
}

TEST_F(UndirectedHouseGraph, when_findingDegreeDistribution_expect_returnCorrectDistribution) {
    auto degreeDistribution = metrics::getDegreeDistribution(graph);
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

    EXPECT_EQ(metrics::getHarmonicCentralities(graph), expectedValues);
}

TEST_F(UndirectedHouseGraph, when_computingLocalClusteringCoefficients_expect_correctAnswers) {
    vector<double> localClustering = metrics::getLocalClusteringCoefficients(graph);
    vector<double> expectedValues = {1., 4/6., 4/6., 6/20., 1., 0, 0};
    EXPECT_EQ(localClustering, expectedValues);
}

TEST_F(UndirectedHouseGraph, when_computingClusteringSpectrum_expect_correctAnswers) {
    graph.addEdgeIdx(5, 6); // make the average not trivial (same local clustering for every degree)
    auto clusteringSpectrum = metrics::getClusteringSpectrum(graph);
    unordered_map<size_t, double> expectedValues = {
        {2, 2/3.}, {3, 4/6.}, {5, 6/20.}
    };
    EXPECT_EQ(clusteringSpectrum, expectedValues);
}

TEST_F(UndirectedHouseGraph, when_computingGlobalClusteringCoefficient_expect_correctAnswer) {
    EXPECT_EQ(metrics::getGlobalClusteringCoefficient(graph), 9./(9+9));
}

TEST_F(UndirectedHouseGraph, when_findingVertexNeighourhoodDegrees_expect_correctDegrees) {
    EXPECT_TRUE(
            metrics::getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({2, 3, 5}) ||
            metrics::getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({2, 5, 3}) ||
            metrics::getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({3, 2, 5}) ||
            metrics::getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({3, 5, 2}) ||
            metrics::getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({5, 2, 3}) ||
            metrics::getNeighbourhoodDegreesOfVertexIdx(graph, 1) == list<size_t> ({5, 3, 2}));
}

TEST_F(UndirectedHouseGraph, when_computingNeighbourDegreeSpectrum_expect_correctAnswer) {
    vector<double> degreeSpectrum = metrics::getNeighbourDegreeSpectrum(graph);
    vector<double> averageNeighbourDegrees = {(3+5)/2., (3+5+2)/3., (2+3+5)/3., (2+3+3+2+1)/5., (3+5)/2., 5, 0};
    EXPECT_EQ(degreeSpectrum, averageNeighbourDegrees);
}

TEST_F(UndirectedHouseGraph, when_computingNormalizedNeighbourDegreeSpectrum_expect_correctAnswer) {
    vector<double> degreeSpectrum = metrics::getNeighbourDegreeSpectrum(graph, true);
    vector<double> averageNeighbourDegrees = {(3+5)/2., (3+5+2)/3., (2+3+5)/3., (2+3+3+2+1)/5., (3+5)/2., 5, 0};

    size_t firstMoment(2+3+3+5+2+1), secondMoment(2*2+3*3+3*3+5*5+2*2+1);
    for (BaseGraph::VertexIndex i: graph)
        EXPECT_EQ(degreeSpectrum[i], averageNeighbourDegrees[i]*firstMoment/secondMoment);
}

TEST_F(UndirectedHouseGraph, when_computingDegreeCorrelation_expect_correctValue) {
    EXPECT_EQ(metrics::getDegreeCorrelation(graph, 16/7.), -629/999.);
}

TEST_F(UndirectedHouseGraph, when_computingModularity_expectCorrectValue) {
    EXPECT_EQ(4/8. - 100/256. - 25/256. - 1/256.,
            metrics::getModularity(graph, vector<size_t>({0, 1, 0, 0, 1, 2, 1})) );
}
