#include <stdexcept>
#include <queue>
#include <stack>
#include <algorithm>

#include <pgl/undirectedgraph.h>
#include "pgl/algorithms/graphpaths.h"


using namespace std;
using namespace PGL;


namespace PGL{

template <typename T>
Path findGeodesicsIdx(const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx) {
    return findPathToVertexFromPredecessorsIdx(graph, sourceIdx, destinationIdx, findPredecessorsOfVertexIdx(graph, sourceIdx));
}

template <typename T>
MultiplePaths findAllGeodesicsIdx(const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx) {
    return findMultiplePathsToVertexFromPredecessorsIdx(graph, sourceIdx, destinationIdx, findAllPredecessorsOfVertexIdx(graph, sourceIdx));
}

template <typename T>
vector<Path> findGeodesicsFromVertexIdx(const T& graph, VertexIndex vertexIdx) {
    auto predecessors = findPredecessorsOfVertexIdx(graph, vertexIdx);

    vector<Path> geodesics;

    for (VertexIndex j: graph)
        geodesics.push_back(findPathToVertexFromPredecessorsIdx(graph, vertexIdx, j, predecessors));
    return geodesics;
}

template <typename T>
vector<MultiplePaths> findAllGeodesicsFromVertexIdx(const T& graph, VertexIndex vertexIdx) {
    auto predecessors = findAllPredecessorsOfVertexIdx(graph, vertexIdx);

    vector<MultiplePaths> allGeodesics;

    for (VertexIndex j: graph)
        allGeodesics.push_back(findMultiplePathsToVertexFromPredecessorsIdx(graph, vertexIdx, j, predecessors));
    return allGeodesics;
}

template <typename T>
Predecessors findPredecessorsOfVertexIdx(const T& graph, VertexIndex vertexIdx){
    VertexIndex currentVertex = vertexIdx;
    size_t verticesNumber = graph.getSize();
    vector<size_t> shortestPaths;
    vector<VertexIndex> predecessor;
    vector<bool> processedVertices;
    queue<VertexIndex> verticesToProcess;

    shortestPaths.resize(verticesNumber, PGL_SIZE_T_MAX);
    shortestPaths[currentVertex] = 0;
    predecessor.resize(verticesNumber, PGL_SIZE_T_MAX);
    processedVertices.resize(verticesNumber, false);
    processedVertices[currentVertex] = true;
    verticesToProcess.push(currentVertex);

    while(!verticesToProcess.empty()){
        currentVertex = verticesToProcess.front();

        for (const VertexIndex& neighbour: graph.getOutEdgesOfIdx(currentVertex)) {
            if (!processedVertices[neighbour]){
                verticesToProcess.push(neighbour);
                processedVertices[neighbour] = true;
                predecessor[neighbour] = currentVertex;
                shortestPaths[neighbour] = shortestPaths[currentVertex] + 1;
            }
        }
        verticesToProcess.pop();
    }
    return {shortestPaths, predecessor};
}

template <typename T>
MultiplePredecessors findAllPredecessorsOfVertexIdx(const T& graph, VertexIndex vertexIdx){
    VertexIndex currentVertex = vertexIdx;
    size_t verticesNumber = graph.getSize();
    vector<size_t> shortestPaths;
    vector<list<VertexIndex>> predecessor;
    vector<bool> processedVertices;
    queue<VertexIndex> verticesToProcess;

    shortestPaths.resize(verticesNumber, PGL_SIZE_T_MAX);
    shortestPaths[currentVertex] = 0;
    predecessor.resize(verticesNumber, list<VertexIndex>());
    processedVertices.resize(verticesNumber, false);
    processedVertices[currentVertex] = true;
    verticesToProcess.push(currentVertex);

    size_t newPathLength;
    while(!verticesToProcess.empty()){
        currentVertex = verticesToProcess.front();

        for (const VertexIndex& neighbour: graph.getOutEdgesOfIdx(currentVertex)){
            if (!processedVertices[neighbour]){
                verticesToProcess.push(neighbour);
                newPathLength = shortestPaths[currentVertex] + 1;

                // if paths are same length and vertex not added
                // newPathLength < shortestPaths[neighbour] because shortestPaths is initialized to PGL_SIZE_T_MAX
                if (newPathLength <= shortestPaths[neighbour] &&
                        find(predecessor[neighbour].begin(), predecessor[neighbour].end(), currentVertex) == predecessor[neighbour].end()){
                    shortestPaths[neighbour] = newPathLength;
                    predecessor[neighbour].push_back(currentVertex);
                }
            }
        }
        processedVertices[currentVertex] = true;
        verticesToProcess.pop();
    }
    return {shortestPaths, predecessor};
}

VertexIndex findSourceVertex(vector<size_t> geodesicLengths){
    bool sourceFound = false;
    VertexIndex sourceIdx;

    // The source vertex is the only one with 0 distance
    for (VertexIndex i=0; i<geodesicLengths.size() && !sourceFound; ++i){
        if (geodesicLengths[i] == 0){
            sourceIdx = i;
            sourceFound = true;
        }
    }
    if (!sourceFound) throw invalid_argument("The predecessor list does not contain the source."
           " There is no shortest path of length 0.");
    return sourceIdx;
}

template <typename T>
Path findPathToVertexFromPredecessorsIdx(const T& graph, VertexIndex destinationIdx, const Predecessors& distancesPredecessors){
    VertexIndex sourceIdx = findSourceVertex(distancesPredecessors.first);
    return findPathToVertexFromPredecessorsIdx(graph, sourceIdx, destinationIdx, distancesPredecessors);
}

template <typename T>
Path findPathToVertexFromPredecessorsIdx(const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx,
        const Predecessors& distancesPredecessors){
    VertexIndex currentVertex = destinationIdx;
    list<VertexIndex> path;

    bool pathFound=false;
    while (!pathFound){
        if (currentVertex == PGL_SIZE_T_MAX) throw runtime_error("Path could not be found.");
        path.push_front(currentVertex);
        currentVertex = distancesPredecessors.second[currentVertex];
        if (currentVertex == sourceIdx) pathFound = true;
    }
    path.push_front(sourceIdx);
    return path;
}

template <typename T>
MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(const T& graph, VertexIndex destinationIdx, const MultiplePredecessors& distancesPredecessors){
    VertexIndex sourceIdx = findSourceVertex(distancesPredecessors.first);
    return findMultiplePathsToVertexFromPredecessorsIdx(graph, sourceIdx, destinationIdx, distancesPredecessors);
}

template <typename T>
MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx,
        const MultiplePredecessors& distancesPredecessors){
    stack<VertexIndex> predecessorsToProcess;
    stack<list<VertexIndex>> associatedPath;
    list<list<VertexIndex>> paths;

    list<VertexIndex> currentList;
    VertexIndex currentVertex = destinationIdx;

    // Add all the first predecessors to the stacks to initialize the loop
    for(const VertexIndex& predecessor: distancesPredecessors.second[destinationIdx]) {
        predecessorsToProcess.push(predecessor);
        associatedPath.push(currentList);
    }


    while (!predecessorsToProcess.empty()){
        currentVertex = predecessorsToProcess.top();
        currentList = associatedPath.top();
        predecessorsToProcess.pop();
        associatedPath.pop();

        if (distancesPredecessors.second[currentVertex].empty() && currentVertex != sourceIdx)
            throw runtime_error("Could not find the path");

        currentList.push_front(currentVertex);
        for(const VertexIndex& predecessor: distancesPredecessors.second[currentVertex]) {
            predecessorsToProcess.push(predecessor);
            associatedPath.push(currentList);
        }

        if (currentVertex == sourceIdx){
            currentList.push_back(destinationIdx);
            paths.push_back(currentList);
        }
    }
    return paths;
}

// Allowed classes

template Path findGeodesicsIdx(const DirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx);
template Path findGeodesicsIdx(const UndirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx);
template MultiplePaths findAllGeodesicsIdx(const DirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx);
template MultiplePaths findAllGeodesicsIdx(const UndirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx);
template std::vector<Path> findGeodesicsFromVertexIdx(const DirectedGraph& graph, VertexIndex vertexIdx);
template std::vector<Path> findGeodesicsFromVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
template std::vector<MultiplePaths> findAllGeodesicsFromVertexIdx(const DirectedGraph& graph, VertexIndex vertexIdx);
template std::vector<MultiplePaths> findAllGeodesicsFromVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);


template Predecessors findPredecessorsOfVertexIdx(const DirectedGraph& graph, VertexIndex vertexIdx);
template Predecessors findPredecessorsOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
template Path findPathToVertexFromPredecessorsIdx(
        const DirectedGraph& graph, VertexIndex destinationIdx, const Predecessors& predecessors);
template Path findPathToVertexFromPredecessorsIdx(
        const UndirectedGraph& graph, VertexIndex destinationIdx, const Predecessors& predecessors);
template Path findPathToVertexFromPredecessorsIdx(
        const DirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx, const Predecessors& predecessors);
template Path findPathToVertexFromPredecessorsIdx(
        const UndirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx, const Predecessors& predecessors);


template MultiplePredecessors findAllPredecessorsOfVertexIdx(const DirectedGraph& graph, VertexIndex vertexIdx);
template MultiplePredecessors findAllPredecessorsOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
template MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const DirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx, const MultiplePredecessors& distancesPredecessors);
template MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const UndirectedGraph& graph, VertexIndex sourceIdx, VertexIndex destinationIdx, const MultiplePredecessors& distancesPredecessors);
template MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const DirectedGraph& graph, VertexIndex destinationIdx, const MultiplePredecessors& distancesPredecessors);
template MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const UndirectedGraph& graph, VertexIndex destinationIdx, const MultiplePredecessors& distancesPredecessors);


} // namespace PGL
