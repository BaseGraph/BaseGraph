#include <queue>

#include "pgl/undirectedgraph.h"
#include "pgl/metrics/general.h"
#include "pgl/algorithms/graphpaths.h"


using namespace std;
using namespace PGL;


namespace PGL{

template <typename T>
double getClosenessCentralityOfVertexIdx(const T& graph, size_t vertexIdx){
    vector<size_t> shortestPathLengths = findPredecessorsOfVertexIdx(graph, vertexIdx).first;
    size_t componentSize = 0;
    unsigned long long int sum = 0;

    for (size_t& vertex: graph) {
        if (shortestPathLengths[vertex] != PGL_SIZE_T_MAX){
            componentSize += 1;
            sum += shortestPathLengths[vertex];
        }
    }
    return ((double) componentSize)/sum;
}

template <typename T>
double getHarmonicMeanGeodesicOfVertexIdx(const T& graph, size_t vertexIdx){
    vector<size_t> shortestPathLengths = findPredecessorsOfVertexIdx(graph, vertexIdx).first;
    size_t componentSize = 0;

    double sumOfInverse;
    for (size_t& vertex: graph) {
        if (shortestPathLengths[vertex] != 0 && shortestPathLengths[vertex] != PGL_SIZE_T_MAX){
            componentSize += 1;
            sumOfInverse += 1.0/shortestPathLengths[vertex];
        }
    }
    return sumOfInverse/componentSize;
}

template <typename T>
double getHarmonicCentralityOfVertexIdx(const T& graph, size_t vertexIdx) {
    vector<size_t> shortestPathLengths = findPredecessorsOfVertexIdx(graph, vertexIdx).first;

    double harmonicSum = 0;
    for (size_t& vertex: graph)
        if (shortestPathLengths[vertex] != 0 && shortestPathLengths[vertex] != PGL_SIZE_T_MAX)
            harmonicSum += 1./shortestPathLengths[vertex];
    return (double) harmonicSum/(graph.getSize()-1);
}

template <typename T>
vector<double> getBetweenesses(const T& graph, bool normalizeWithGeodesicNumber) {
    size_t verticesNumber = graph.getSize();
    vector<double> betweeness;
    betweeness.resize(verticesNumber, 0);

    pair<vector<size_t>, vector<list<size_t> > > distancesPredecessors;
    list<list<size_t> > currentGeodesics;
    for (size_t& i: graph) {
        distancesPredecessors = findAllPredecessorsOfVertexIdx(graph, i);
        for (size_t& j: graph) {
            currentGeodesics = findMultiplePathsToVertexFromPredecessorsIdx(graph, i, j, distancesPredecessors);
            if (currentGeodesics.empty()) continue; // vertices i and j are not in the same component

            for (auto& geodesic: currentGeodesics) {
                for (auto& vertexOnGeodesic: geodesic) {
                    if (normalizeWithGeodesicNumber)
                        betweeness[vertexOnGeodesic] += 1./currentGeodesics.size();
                    else
                        betweeness[vertexOnGeodesic] += 1;
                }
            }

        }
    }
    return betweeness;
}

template <typename T>
vector<size_t> getDiameters(const T& graph){
    size_t verticesNumber = graph.getSize();
    vector<size_t> diameters;
    diameters.resize(verticesNumber);

    vector<size_t> shortestPathLengths;
    size_t largestDistance;
    for (size_t& i: graph) {
        shortestPathLengths = findPredecessorsOfVertexIdx(graph, i).first;
        largestDistance = shortestPathLengths[0];

        for (size_t& j: graph)
           if (shortestPathLengths[j] > largestDistance && shortestPathLengths[j]!=PGL_SIZE_T_MAX)
              largestDistance = shortestPathLengths[j];

        if (largestDistance == PGL_SIZE_T_MAX)
            diameters[i] = 0;
        else
            diameters[i] = largestDistance;
    }
    return diameters;
}

template <typename T>
list<list<size_t> > findConnectedComponents(const T& graph){
    size_t verticesNumber = graph.getSize();
    if (verticesNumber == 0) throw logic_error("There are no vertices.");

    list<list<size_t> > connectedComponents;
    list<size_t> currentComponent;
    size_t currentVertex, startVertex;

    queue<size_t> verticesToProcess;
    vector<bool> processedVertices;
    bool allVerticesProcessed = false;
    processedVertices.resize(verticesNumber, false);

    list<size_t>::const_iterator vertexNeighbour;

    while (!allVerticesProcessed){
        allVerticesProcessed = true;
        for (size_t i=0; i<verticesNumber && allVerticesProcessed; ++i){
            if (!processedVertices[i]) {
                allVerticesProcessed = false;
                startVertex = i;
            }
        }

        if (!allVerticesProcessed){
            currentComponent.clear();
            verticesToProcess.push(startVertex);
            processedVertices[startVertex] = true;

            while (!verticesToProcess.empty()) {
                currentVertex = verticesToProcess.front();

                for (const size_t& vertexNeighbour: graph.getOutEdgesOfIdx(currentVertex)) {
                    if (!processedVertices[vertexNeighbour]) {
                        verticesToProcess.push(vertexNeighbour);
                        processedVertices[vertexNeighbour] = true;
                    }
                }
                currentComponent.push_back(currentVertex);
                verticesToProcess.pop();
            }
            connectedComponents.push_back(currentComponent);
        }
    }
    return connectedComponents;
}

template <typename T>
vector<double> getAverageShortestPaths(const T& graph) {
    vector<double> averageShortestPaths(graph.getSize(), 0);
    vector<size_t> shortestPathLengths;

    for (auto component: findConnectedComponents(graph)) {
        for (const size_t& vertex: component) {
            if (component.size() > 1){
                shortestPathLengths = findPredecessorsOfVertexIdx(graph, vertex).first;

                for (const size_t& pathLength: shortestPathLengths)
                    if (pathLength!=PGL_SIZE_T_MAX)
                        averageShortestPaths[vertex]+=pathLength;

                averageShortestPaths[vertex] /= component.size()-1;
            }
        }
    }
    return averageShortestPaths;
}

template <typename T>
std::vector<std::vector<double> > getShortestPathsDistribution(const T& graph) {
    auto connectedComponents = findConnectedComponents(graph);

    vector<size_t> shortestPathLengths;
    vector<vector<double> > shortestPathDistribution(connectedComponents.size(), vector<double>(1, 0));
    size_t componentIndex = 0;

    for (auto component: connectedComponents) {
        auto& currentDistribution = shortestPathDistribution[componentIndex];

        if (component.size() > 1) {
            for (const size_t& vertex: component) {
                shortestPathLengths = findPredecessorsOfVertexIdx(graph, vertex).first;

                for (const size_t& pathLength: shortestPathLengths) {
                    if (pathLength!=0 && pathLength!=PGL_SIZE_T_MAX) {
                        if (pathLength+1 > currentDistribution.size())
                            currentDistribution.resize(pathLength+1, 0);
                        currentDistribution[pathLength]++;
                    }
                }
            }
            for (double& element: currentDistribution)
                element /= component.size();
        }

        componentIndex++;
    }
    return shortestPathDistribution;
}

// Allowed classes for metrics

template double getClosenessCentralityOfVertexIdx(const DirectedGraph& graph, size_t vertexIdx);
template double getClosenessCentralityOfVertexIdx(const UndirectedGraph& graph, size_t vertexIdx);
template double getHarmonicMeanGeodesicOfVertexIdx(const DirectedGraph& graph, size_t vertexIdx);
template double getHarmonicMeanGeodesicOfVertexIdx(const UndirectedGraph& graph, size_t vertexIdx);
template double getHarmonicCentralityOfVertexIdx(const DirectedGraph& graph, size_t vertexIdx);
template double getHarmonicCentralityOfVertexIdx(const UndirectedGraph& graph, size_t vertexIdx);
template std::vector<double> getBetweenesses(const DirectedGraph& graph, bool normalizeWithGeodesicNumber);
template std::vector<double> getBetweenesses(const UndirectedGraph& graph, bool normalizeWithGeodesicNumber);
template std::vector<size_t> getDiameters(const DirectedGraph& graph);
template std::vector<size_t> getDiameters(const UndirectedGraph& graph);
template std::vector<double> getAverageShortestPaths(const DirectedGraph& graph);
template std::vector<double> getAverageShortestPaths(const UndirectedGraph& graph);
template std::vector<std::vector<double> > getShortestPathsDistribution(const DirectedGraph& graph);
template std::vector<std::vector<double> > getShortestPathsDistribution(const UndirectedGraph& graph);

template std::list<std::list<size_t> > findConnectedComponents(const DirectedGraph& graph);
template std::list<std::list<size_t> > findConnectedComponents(const UndirectedGraph& graph);


} // namespace PGL
