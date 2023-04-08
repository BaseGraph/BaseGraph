#ifndef BASE_GRAPH_PATHS_HPP
#define BASE_GRAPH_PATHS_HPP

#include <algorithm>
#include <limits>
#include <list>
#include <stack>
#include <queue>
#include <stdexcept>
#include <vector>

#include "BaseGraph/types.h"

namespace BaseGraph {
namespace algorithms {

const size_t BASEGRAPH_SIZE_T_MAX = std::numeric_limits<size_t>::max();

typedef std::pair<std::vector<size_t>, std::vector<VertexIndex>> Predecessors;
typedef std::pair<std::vector<size_t>, std::vector<std::list<VertexIndex>>>
    MultiplePredecessors;
typedef std::list<VertexIndex> Path;
typedef std::list<std::list<VertexIndex>> MultiplePaths;

template <template <class...> class Graph, typename EdgeLabel>
Path findGeodesics(const Graph<EdgeLabel> &graph, VertexIndex source,
                   VertexIndex destination);
template <template <class...> class Graph, typename EdgeLabel>
MultiplePaths findAllGeodesics(const Graph<EdgeLabel> &graph,
                               VertexIndex source, VertexIndex destination);

template <template <class...> class Graph, typename EdgeLabel>
std::vector<Path> findGeodesicsFromVertex(const Graph<EdgeLabel> &graph,
                                          VertexIndex vertex);
template <template <class...> class Graph, typename EdgeLabel>
std::vector<MultiplePaths>
findAllGeodesicsFromVertex(const Graph<EdgeLabel> &graph, VertexIndex vertex);

template <template <class...> class Graph, typename EdgeLabel>
Predecessors findPredecessorsOfVertex(const Graph<EdgeLabel> &graph,
                                      VertexIndex vertex);
template <template <class...> class Graph, typename EdgeLabel>
Path findPathToVertexFromPredecessors(const Graph<EdgeLabel> &graph,
                                      VertexIndex destination,
                                      const Predecessors &predecessors);
template <template <class...> class Graph, typename EdgeLabel>
Path findPathToVertexFromPredecessors(const Graph<EdgeLabel> &graph,
                                      VertexIndex source,
                                      VertexIndex destination,
                                      const Predecessors &predecessors);

template <template <class...> class Graph, typename EdgeLabel>
MultiplePredecessors findAllPredecessorsOfVertex(const Graph<EdgeLabel> &graph,
                                                 VertexIndex vertex);
template <template <class...> class Graph, typename EdgeLabel>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const Graph<EdgeLabel> &graph, VertexIndex source, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);
template <template <class...> class Graph, typename EdgeLabel>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const Graph<EdgeLabel> &graph, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);

template <template <class...> class Graph, typename EdgeLabel>
Path findGeodesics(const Graph<EdgeLabel> &graph, VertexIndex source,
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

template <template <class...> class Graph, typename EdgeLabel>
MultiplePaths findAllGeodesics(const Graph<EdgeLabel> &graph,
                               VertexIndex source, VertexIndex destination) {
    if (source == destination)
        return {{source}};

    auto predecessors = findAllPredecessorsOfVertex(graph, source);

    if (predecessors.first[destination] != BASEGRAPH_SIZE_T_MAX)
        return findMultiplePathsToVertexFromPredecessors(
            graph, source, destination, predecessors);
    else
        return {};
}

template <template <class...> class Graph, typename EdgeLabel>
std::vector<Path> findGeodesicsFromVertex(const Graph<EdgeLabel> &graph,
                                          VertexIndex vertex) {
    auto predecessors = findPredecessorsOfVertex(graph, vertex);
    std::vector<Path> geodesics;

    for (VertexIndex j : graph) {
        if (predecessors.first[j] != BASEGRAPH_SIZE_T_MAX)
            geodesics.push_back(findPathToVertexFromPredecessors(
                graph, vertex, j, predecessors));
        else
            geodesics.push_back({});
    }
    return geodesics;
}

template <template <class...> class Graph, typename EdgeLabel>
std::vector<MultiplePaths>
findAllGeodesicsFromVertex(const Graph<EdgeLabel> &graph, VertexIndex vertex) {
    auto predecessors = findAllPredecessorsOfVertex(graph, vertex);

    std::vector<MultiplePaths> allGeodesics;

    for (VertexIndex j : graph)
        if (predecessors.first[j] != BASEGRAPH_SIZE_T_MAX)
            allGeodesics.push_back(findMultiplePathsToVertexFromPredecessors(
                graph, vertex, j, predecessors));
        else
            allGeodesics.push_back({});
    return allGeodesics;
}

template <template <class...> class Graph, typename EdgeLabel>
Predecessors findPredecessorsOfVertex(const Graph<EdgeLabel> &graph, VertexIndex vertex) {
    VertexIndex currentVertex = vertex;
    size_t verticesNumber = graph.getSize();

    std::vector<size_t> shortestPaths(verticesNumber, BASEGRAPH_SIZE_T_MAX);
    std::vector<VertexIndex> predecessors(verticesNumber, BASEGRAPH_SIZE_T_MAX);
    std::vector<bool> processedVertices(verticesNumber, false);
    std::queue<VertexIndex> verticesToProcess({currentVertex});

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

template <template <class...> class Graph, typename EdgeLabel>
MultiplePredecessors findAllPredecessorsOfVertex(const Graph<EdgeLabel> &graph,
                                                 VertexIndex vertex) {
    VertexIndex currentVertex = vertex;
    size_t verticesNumber = graph.getSize();

    std::vector<size_t> shortestPaths(verticesNumber, BASEGRAPH_SIZE_T_MAX);
    std::vector<std::list<VertexIndex>> predecessors(verticesNumber, std::list<VertexIndex>());
    std::vector<bool> processedVertices(verticesNumber, false);
    std::queue<VertexIndex> verticesToProcess({currentVertex});

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
                    std::find(predecessors[neighbour].begin(),
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

inline VertexIndex findSourceVertex(std::vector<size_t> geodesicLengths) {
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
        throw std::invalid_argument(
            "The predecessor list does not contain the source."
            " There is no shortest path of length 0.");
    return source;
}

template <template <class...> class Graph, typename EdgeLabel>
Path findPathToVertexFromPredecessors(
    const Graph<EdgeLabel> &graph, VertexIndex destination,
    const Predecessors &distancesPredecessors) {
    VertexIndex source = findSourceVertex(distancesPredecessors.first);
    return findPathToVertexFromPredecessors(graph, source, destination,
                                            distancesPredecessors);
}

template <template <class...> class Graph, typename EdgeLabel>
Path findPathToVertexFromPredecessors(
    const Graph<EdgeLabel> &graph, VertexIndex source, VertexIndex destination,
    const Predecessors &distancesPredecessors) {
    if (source == destination)
        return {source};

    VertexIndex currentVertex = destination;
    std::list<VertexIndex> path;

    bool pathFound = false;
    while (!pathFound) {
        if (currentVertex == BASEGRAPH_SIZE_T_MAX)
            throw std::runtime_error("Path could not be found.");
        path.push_front(currentVertex);
        currentVertex = distancesPredecessors.second[currentVertex];
        if (currentVertex == source)
            pathFound = true;
    }
    path.push_front(source);
    return path;
}

template <template <class...> class Graph, typename EdgeLabel>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const Graph<EdgeLabel> &graph, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors) {
    VertexIndex source = findSourceVertex(distancesPredecessors.first);
    return findMultiplePathsToVertexFromPredecessors(graph, source, destination,
                                                     distancesPredecessors);
}

template <template <class...> class Graph, typename EdgeLabel>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const Graph<EdgeLabel> &graph, VertexIndex source, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors) {
    if (source == destination)
        return {{source}};

    std::stack<VertexIndex> predecessorsToProcess;
    std::stack<std::list<VertexIndex>> associatedPath;
    std::list<std::list<VertexIndex>> paths;

    std::list<VertexIndex> currentList;
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
            throw std::runtime_error("Could not find the path");

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

} // namespace algorithms
} // namespace BaseGraph

#endif
