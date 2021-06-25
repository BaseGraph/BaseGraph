#include <queue>

#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/metrics/general.h"
#include "BaseGraph/algorithms/graphpaths.h"


using namespace std;


namespace BaseGraph{

template <typename T>
double getClosenessCentralityOfVertexIdx(const T& graph, VertexIndex vertexIdx){
    vector<size_t> shortestPathLengths = findShortestPathLengthsFromVertexIdx(graph, vertexIdx);
    size_t componentSize = 0;
    unsigned long long int sum = 0;

    for (VertexIndex& vertex: graph) {
        if (shortestPathLengths[vertex] != SIZE_T_MAX){
            componentSize += 1;
            sum += shortestPathLengths[vertex];
        }
    }
    return sum > 0 ? ((double) componentSize-1)/sum : 0;
}

template <typename T>
double getHarmonicMeanGeodesicOfVertexIdx(const T& graph, VertexIndex vertexIdx){
    vector<size_t> shortestPathLengths = findShortestPathLengthsFromVertexIdx(graph, vertexIdx);
    size_t componentSize = 0;

    double sumOfInverse = 0;
    for (VertexIndex& vertex: graph) {
        if (shortestPathLengths[vertex] != 0 && shortestPathLengths[vertex] != SIZE_T_MAX){
            componentSize += 1;
            sumOfInverse += 1.0/shortestPathLengths[vertex];
        }
    }
    return componentSize > 0 ? sumOfInverse/componentSize : 0;
}

template <typename T>
double getHarmonicCentralityOfVertexIdx(const T& graph, VertexIndex vertexIdx) {
    vector<size_t> shortestPathLengths = findShortestPathLengthsFromVertexIdx(graph, vertexIdx);

    double harmonicSum = 0;
    for (VertexIndex& vertex: graph)
        if (shortestPathLengths[vertex] != 0 && shortestPathLengths[vertex] != SIZE_T_MAX)
            harmonicSum += 1.0/shortestPathLengths[vertex];

    return harmonicSum;
}

template <>
vector<double> getBetweennesses(const DirectedGraph& graph, bool normalizeWithGeodesicNumber) {
    size_t verticesNumber = graph.getSize();
    vector<double> betweennesses;
    betweennesses.resize(verticesNumber, 0);

    MultiplePredecessors distancesPredecessors;
    list<list<VertexIndex> > currentGeodesics;
    for (const VertexIndex& i: graph) {
        distancesPredecessors = findAllPredecessorsOfVertexIdx(graph, i);
        for (const VertexIndex& j: graph) {
            currentGeodesics = findMultiplePathsToVertexFromPredecessorsIdx(graph, i, j, distancesPredecessors);
            if (currentGeodesics.empty()) continue; // vertices i and j are not in the same component

            for (auto& geodesic: currentGeodesics) {
                for (auto& vertexOnGeodesic: geodesic) {
                    if (vertexOnGeodesic != i && vertexOnGeodesic != j) {
                        if (normalizeWithGeodesicNumber)
                            betweennesses[vertexOnGeodesic] += 1./currentGeodesics.size();
                        else
                            betweennesses[vertexOnGeodesic] += 1;
                    }
                }
            }
        }
    }
    return betweennesses;
}

template <>
vector<double> getBetweennesses(const UndirectedGraph& graph, bool normalizeWithGeodesicNumber) {
    size_t verticesNumber = graph.getSize();
    vector<double> betweennesses;
    betweennesses.resize(verticesNumber, 0);

    MultiplePredecessors distancesPredecessors;
    list<list<VertexIndex> > currentGeodesics;
    for (const VertexIndex& i: graph) {
        distancesPredecessors = findAllPredecessorsOfVertexIdx(graph, i);
        for (const VertexIndex& j: graph) {
            if (i>=j) continue;

            currentGeodesics = findMultiplePathsToVertexFromPredecessorsIdx(graph, i, j, distancesPredecessors);
            if (currentGeodesics.empty()) continue; // vertices i and j are not in the same component

            for (auto& geodesic: currentGeodesics) {
                for (auto& vertexOnGeodesic: geodesic) {
                    if (vertexOnGeodesic != i && vertexOnGeodesic != j) {
                        if (normalizeWithGeodesicNumber)
                            betweennesses[vertexOnGeodesic] += 1./currentGeodesics.size();
                        else
                            betweennesses[vertexOnGeodesic] += 1;
                    }
                }
            }
        }
    }
    return betweennesses;
}

template <typename T>
vector<size_t> getDiameters(const T& graph){
    size_t verticesNumber = graph.getSize();
    vector<size_t> diameters;
    diameters.resize(verticesNumber);

    vector<size_t> shortestPathLengths;
    size_t largestDistance;
    for (const VertexIndex& i: graph) {
        shortestPathLengths = findPredecessorsOfVertexIdx(graph, i).first;
        largestDistance = shortestPathLengths[0];

        for (const VertexIndex& j: graph)
           if (shortestPathLengths[j] > largestDistance && shortestPathLengths[j]!=SIZE_T_MAX)
              largestDistance = shortestPathLengths[j];

        if (largestDistance == SIZE_T_MAX)
            diameters[i] = 0;
        else
            diameters[i] = largestDistance;
    }
    return diameters;
}

template <typename T>
list<Component> findConnectedComponents(const T& graph){
    size_t verticesNumber = graph.getSize();
    if (verticesNumber == 0) throw logic_error("There are no vertices.");

    list<Component> connectedComponents;
    Component currentComponent;
    VertexIndex currentVertex, startVertex;

    queue<VertexIndex> verticesToProcess;
    vector<bool> processedVertices;
    bool allVerticesProcessed = false;
    processedVertices.resize(verticesNumber, false);

    list<VertexIndex>::const_iterator vertexNeighbour;

    while (!allVerticesProcessed){
        allVerticesProcessed = true;
        for (VertexIndex i=0; i<verticesNumber && allVerticesProcessed; ++i){
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

                for (const VertexIndex& vertexNeighbour: graph.getOutEdgesOfIdx(currentVertex)) {
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
        for (const VertexIndex& vertex: component) {
            if (component.size() > 1){
                shortestPathLengths = findPredecessorsOfVertexIdx(graph, vertex).first;

                for (const size_t& pathLength: shortestPathLengths)
                    if (pathLength!=SIZE_T_MAX)
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
            for (const VertexIndex& vertex: component) {
                shortestPathLengths = findPredecessorsOfVertexIdx(graph, vertex).first;

                for (const size_t& pathLength: shortestPathLengths) {
                    if (pathLength!=0 && pathLength!=SIZE_T_MAX) {
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

template double getClosenessCentralityOfVertexIdx(const DirectedGraph& graph, VertexIndex vertexIdx);
template double getClosenessCentralityOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
template double getHarmonicMeanGeodesicOfVertexIdx(const DirectedGraph& graph, VertexIndex vertexIdx);
template double getHarmonicMeanGeodesicOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
template double getHarmonicCentralityOfVertexIdx(const DirectedGraph& graph, VertexIndex vertexIdx);
template double getHarmonicCentralityOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
template std::vector<size_t> getDiameters(const DirectedGraph& graph);
template std::vector<size_t> getDiameters(const UndirectedGraph& graph);
template std::vector<double> getAverageShortestPaths(const DirectedGraph& graph);
template std::vector<double> getAverageShortestPaths(const UndirectedGraph& graph);
template std::vector<std::vector<double> > getShortestPathsDistribution(const DirectedGraph& graph);
template std::vector<std::vector<double> > getShortestPathsDistribution(const UndirectedGraph& graph);

template std::list<Component> findConnectedComponents(const DirectedGraph& graph);
template std::list<Component> findConnectedComponents(const UndirectedGraph& graph);


} // namespace BaseGraph
