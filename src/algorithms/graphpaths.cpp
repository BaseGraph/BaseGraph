#include "BaseGraph/algorithms/graphpaths.h"

#include <algorithm>
#include <queue>
#include <stack>
#include <stdexcept>

#include <BaseGraph/undirectedgraph.h>

using namespace std;

namespace BaseGraph {
namespace algorithms {

template <typename T>
Path findGeodesics(const T &graph, VertexIndex source,
                   VertexIndex destination) {
    if (source == destination)
        return {source};

    auto predecessors = findPredecessorsOfVertex(graph, source);

    if (predecessors.first[destination] != BASEGRAPH_SIZE_T_MAX)
        return findPathToVertexFromPredecessors(graph, source, destination,
                                                predecessors);
    else
        return {};
}

template <typename T>
MultiplePaths findAllGeodesics(const T &graph, VertexIndex source,
                               VertexIndex destination) {
    if (source == destination)
        return {{source}};

    auto predecessors = findAllPredecessorsOfVertex(graph, source);

    if (predecessors.first[destination] != BASEGRAPH_SIZE_T_MAX)
        return findMultiplePathsToVertexFromPredecessors(
            graph, source, destination, predecessors);
    else
        return {};
}

template <typename T>
vector<Path> findGeodesicsFromVertex(const T &graph, VertexIndex vertex) {
    auto predecessors = findPredecessorsOfVertex(graph, vertex);

    vector<Path> geodesics;

    for (VertexIndex j : graph) {
        if (predecessors.first[j] != BASEGRAPH_SIZE_T_MAX)
            geodesics.push_back(findPathToVertexFromPredecessors(
                graph, vertex, j, predecessors));
        else
            geodesics.push_back({});
    }
    return geodesics;
}

template <typename T>
vector<MultiplePaths> findAllGeodesicsFromVertex(const T &graph,
                                                 VertexIndex vertex) {
    auto predecessors = findAllPredecessorsOfVertex(graph, vertex);

    vector<MultiplePaths> allGeodesics;

    for (VertexIndex j : graph)
        if (predecessors.first[j] != BASEGRAPH_SIZE_T_MAX)
            allGeodesics.push_back(findMultiplePathsToVertexFromPredecessors(
                graph, vertex, j, predecessors));
        else
            allGeodesics.push_back({});
    return allGeodesics;
}

template <typename T>
Predecessors findPredecessorsOfVertex(const T &graph, VertexIndex vertex) {
    VertexIndex currentVertex = vertex;
    size_t verticesNumber = graph.getSize();

    vector<size_t> shortestPaths(verticesNumber, BASEGRAPH_SIZE_T_MAX);
    vector<VertexIndex> predecessors(verticesNumber, BASEGRAPH_SIZE_T_MAX);
    vector<bool> processedVertices(verticesNumber, false);
    queue<VertexIndex> verticesToProcess({currentVertex});

    shortestPaths[currentVertex] = 0;
    processedVertices[currentVertex] = true;

    while (!verticesToProcess.empty()) {
        currentVertex = verticesToProcess.front();

        for (const VertexIndex &neighbour :
             graph.getOutEdgesOf(currentVertex)) {
            if (!processedVertices[neighbour]) {
                verticesToProcess.push(neighbour);
                processedVertices[neighbour] = true;
                predecessors[neighbour] = currentVertex;
                shortestPaths[neighbour] = shortestPaths[currentVertex] + 1;
            }
        }
        verticesToProcess.pop();
    }
    return {shortestPaths, predecessors};
}

template <typename T>
MultiplePredecessors findAllPredecessorsOfVertex(const T &graph,
                                                 VertexIndex vertex) {
    VertexIndex currentVertex = vertex;
    size_t verticesNumber = graph.getSize();

    vector<size_t> shortestPaths(verticesNumber, BASEGRAPH_SIZE_T_MAX);
    vector<list<VertexIndex>> predecessors(verticesNumber, list<VertexIndex>());
    vector<bool> processedVertices(verticesNumber, false);
    queue<VertexIndex> verticesToProcess({currentVertex});

    shortestPaths[currentVertex] = 0;
    processedVertices[currentVertex] = true;

    size_t newPathLength;
    while (!verticesToProcess.empty()) {
        currentVertex = verticesToProcess.front();

        for (const VertexIndex &neighbour :
             graph.getOutEdgesOf(currentVertex)) {
            if (!processedVertices[neighbour]) {
                verticesToProcess.push(neighbour);
                newPathLength = shortestPaths[currentVertex] + 1;

                // if paths are same length and vertex not added
                // newPathLength < shortestPaths[neighbour] because
                // shortestPaths is initialized to SIZE_T_MAX
                if (newPathLength <= shortestPaths[neighbour] &&
                    find(predecessors[neighbour].begin(),
                         predecessors[neighbour].end(),
                         currentVertex) == predecessors[neighbour].end()) {
                    shortestPaths[neighbour] = newPathLength;
                    predecessors[neighbour].push_back(currentVertex);
                }
            }
        }
        processedVertices[currentVertex] = true;
        verticesToProcess.pop();
    }
    return {shortestPaths, predecessors};
}

VertexIndex findSourceVertex(vector<size_t> geodesicLengths) {
    bool sourceFound = false;
    VertexIndex source;

    // The source vertex is the only one with 0 distance
    for (VertexIndex i = 0; i < geodesicLengths.size() && !sourceFound; ++i) {
        if (geodesicLengths[i] == 0) {
            source = i;
            sourceFound = true;
        }
    }
    if (!sourceFound)
        throw invalid_argument(
            "The predecessor list does not contain the source."
            " There is no shortest path of length 0.");
    return source;
}

template <typename T>
Path findPathToVertexFromPredecessors(
    const T &graph, VertexIndex destination,
    const Predecessors &distancesPredecessors) {
    VertexIndex source = findSourceVertex(distancesPredecessors.first);
    return findPathToVertexFromPredecessors(graph, source, destination,
                                            distancesPredecessors);
}

template <typename T>
Path findPathToVertexFromPredecessors(
    const T &graph, VertexIndex source, VertexIndex destination,
    const Predecessors &distancesPredecessors) {
    if (source == destination)
        return {source};

    VertexIndex currentVertex = destination;
    list<VertexIndex> path;

    bool pathFound = false;
    while (!pathFound) {
        if (currentVertex == BASEGRAPH_SIZE_T_MAX)
            throw runtime_error("Path could not be found.");
        path.push_front(currentVertex);
        currentVertex = distancesPredecessors.second[currentVertex];
        if (currentVertex == source)
            pathFound = true;
    }
    path.push_front(source);
    return path;
}

template <typename T>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const T &graph, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors) {
    VertexIndex source = findSourceVertex(distancesPredecessors.first);
    return findMultiplePathsToVertexFromPredecessors(graph, source, destination,
                                                     distancesPredecessors);
}

template <typename T>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const T &graph, VertexIndex source, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors) {
    if (source == destination)
        return {{source}};

    stack<VertexIndex> predecessorsToProcess;
    stack<list<VertexIndex>> associatedPath;
    list<list<VertexIndex>> paths;

    list<VertexIndex> currentList;
    VertexIndex currentVertex = destination;

    // Add all the first predecessors to the stacks to initialize the loop
    for (const VertexIndex &predecessor :
         distancesPredecessors.second[destination]) {
        predecessorsToProcess.push(predecessor);
        associatedPath.push(currentList);
    }

    while (!predecessorsToProcess.empty()) {
        currentVertex = predecessorsToProcess.top();
        currentList = associatedPath.top();
        predecessorsToProcess.pop();
        associatedPath.pop();

        if (distancesPredecessors.second[currentVertex].empty() &&
            currentVertex != source)
            throw runtime_error("Could not find the path");

        currentList.push_front(currentVertex);
        for (const VertexIndex &predecessor :
             distancesPredecessors.second[currentVertex]) {
            predecessorsToProcess.push(predecessor);
            associatedPath.push(currentList);
        }

        if (currentVertex == source) {
            currentList.push_back(destination);
            paths.push_back(currentList);
        }
    }
    return paths;
}

// Allowed classes

template Path findGeodesics(const _DirectedGraph &graph, VertexIndex source,
                            VertexIndex destination);
template Path findGeodesics(const _UndirectedGraph &graph, VertexIndex source,
                            VertexIndex destination);
template MultiplePaths findAllGeodesics(const _DirectedGraph &graph,
                                        VertexIndex source,
                                        VertexIndex destination);
template MultiplePaths findAllGeodesics(const _UndirectedGraph &graph,
                                        VertexIndex source,
                                        VertexIndex destination);
template std::vector<Path> findGeodesicsFromVertex(const _DirectedGraph &graph,
                                                   VertexIndex vertex);
template std::vector<Path> findGeodesicsFromVertex(const _UndirectedGraph &graph,
                                                   VertexIndex vertex);
template std::vector<MultiplePaths>
findAllGeodesicsFromVertex(const _DirectedGraph &graph, VertexIndex vertex);
template std::vector<MultiplePaths>
findAllGeodesicsFromVertex(const _UndirectedGraph &graph, VertexIndex vertex);

template Predecessors findPredecessorsOfVertex(const _DirectedGraph &graph,
                                               VertexIndex vertex);
template Predecessors findPredecessorsOfVertex(const _UndirectedGraph &graph,
                                               VertexIndex vertex);
template Path
findPathToVertexFromPredecessors(const _DirectedGraph &graph,
                                 VertexIndex destination,
                                 const Predecessors &predecessors);
template Path
findPathToVertexFromPredecessors(const _UndirectedGraph &graph,
                                 VertexIndex destination,
                                 const Predecessors &predecessors);
template Path
findPathToVertexFromPredecessors(const _DirectedGraph &graph, VertexIndex source,
                                 VertexIndex destination,
                                 const Predecessors &predecessors);
template Path
findPathToVertexFromPredecessors(const _UndirectedGraph &graph,
                                 VertexIndex source, VertexIndex destination,
                                 const Predecessors &predecessors);

template MultiplePredecessors
findAllPredecessorsOfVertex(const _DirectedGraph &graph, VertexIndex vertex);
template MultiplePredecessors
findAllPredecessorsOfVertex(const _UndirectedGraph &graph, VertexIndex vertex);
template MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const _DirectedGraph &graph, VertexIndex source, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);
template MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const _UndirectedGraph &graph, VertexIndex source, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);
template MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const _DirectedGraph &graph, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);
template MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const _UndirectedGraph &graph, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);

} // namespace algorithms
} // namespace BaseGraph
