#include <stdexcept>
#include <queue>
#include <stack>

#include "pgl/algorithms/graphpaths.h"


using namespace std;
using namespace PGL;



namespace PGL{
pair<vector<size_t>, vector<size_t>> findGeodesicsOfVertex(const DirectedGraph& graph, size_t vertexIdx){
    size_t currentVertex = vertexIdx;
    size_t verticesNumber = graph.getSize();
    vector<size_t> shortestPaths;
    vector<size_t> predecessor;
    vector<bool> processedVertices;
    queue<size_t> verticesToProcess;

    shortestPaths.resize(verticesNumber, SIZE_T_MAX);
    shortestPaths[currentVertex] = 0;
    predecessor.resize(verticesNumber, SIZE_T_MAX);
    processedVertices.resize(verticesNumber, false);
    processedVertices[currentVertex] = true;
    verticesToProcess.push(currentVertex);

    while(!verticesToProcess.empty()){
        currentVertex = verticesToProcess.front();

        for (const size_t& neighbour: graph.getOutEdgesOfIdx(currentVertex)) {
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

pair<vector<size_t>, vector<list<size_t>>> findEveryGeodesicsOfVertex(const DirectedGraph& graph, size_t vertexIdx){
    size_t currentVertex = vertexIdx;
    size_t verticesNumber = graph.getSize();
    vector<size_t> shortestPaths;
    vector<list<size_t>> predecessor;
    vector<bool> processedVertices;
    queue<size_t> verticesToProcess;

    shortestPaths.resize(verticesNumber, SIZE_T_MAX);
    shortestPaths[currentVertex] = 0;
    predecessor.resize(verticesNumber, list<size_t>());
    processedVertices.resize(verticesNumber, false);
    processedVertices[currentVertex] = true;
    verticesToProcess.push(currentVertex);

    size_t newPathLength;
    while(!verticesToProcess.empty()){
        currentVertex = verticesToProcess.front();

        for (const size_t& neighbour: graph.getOutEdgesOfIdx(currentVertex)){
            if (!processedVertices[neighbour]){
                verticesToProcess.push(neighbour);
                newPathLength = shortestPaths[currentVertex] + 1;

                // if paths are same length and vertex not added
                // newPathLength < shortestPaths[neighbour] because shortestPaths is initialized to SIZE_T_MAX
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

size_t findSourceVertex(vector<size_t> geodesicLengths){
    bool sourceFound = false;
    size_t sourceIdx;

    // The source vertex is the only one with 0 distance
    for (size_t i=0; i<geodesicLengths.size() && !sourceFound; ++i){
        if (geodesicLengths[i] == 0){
            sourceIdx = i;
            sourceFound = true;
        }
    }
    if (!sourceFound) throw invalid_argument("The predecessor list does not contain the source."
           " There is no shortest path of length 0.");
    return sourceIdx;
}

list<size_t> findPathToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t destinationIdx,
        const pair<vector<size_t>, vector<size_t>>& distancesPredecessors){
    size_t sourceIdx = findSourceVertex(distancesPredecessors.first);
    return findPathToVertexFromPredecessorsIdx(graph, sourceIdx, destinationIdx, distancesPredecessors);
}

list<size_t> findPathToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t sourceIdx, size_t destinationIdx,
        const pair<vector<size_t>, vector<size_t>>& distancesPredecessors){
    size_t currentVertex = destinationIdx;
    list<size_t> path;

    bool pathFound=false;
    while (!pathFound){
        if (currentVertex == SIZE_T_MAX) throw runtime_error("Path could not be found.");
        path.push_front(currentVertex);
        currentVertex = distancesPredecessors.second[currentVertex];
        if (currentVertex == sourceIdx) pathFound = true;
    }
    path.push_front(sourceIdx);
    return path;
}

list<list<size_t>> findMultiplePathsToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t destinationIdx, 
        const pair<vector<size_t>, vector<list<size_t>>>& distancesPredecessors){
    size_t sourceIdx = findSourceVertex(distancesPredecessors.first);
    return findMultiplePathsToVertexFromPredecessorsIdx(graph, sourceIdx, destinationIdx, distancesPredecessors);
}

list<list<size_t>> findMultiplePathsToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t sourceIdx, size_t destinationIdx, 
        const pair<vector<size_t>, vector<list<size_t>>>& distancesPredecessors){
    stack<size_t> predecessorsToProcess;
    stack<list<size_t>> associatedPath;
    list<list<size_t>> paths;

    list<size_t> currentList;
    size_t currentVertex = destinationIdx;

    // Add all the first predecessors to the stacks to initialize the loop
    for(const size_t& predecessor: distancesPredecessors.second[destinationIdx]) {
        predecessorsToProcess.push(predecessor);
        associatedPath.push(currentList);
    }


    bool pathFound=false;
    while (!predecessorsToProcess.empty()){
        currentVertex = predecessorsToProcess.top();
        currentList = associatedPath.top();
        predecessorsToProcess.pop();
        associatedPath.pop();

        if (distancesPredecessors.second[currentVertex].empty() && currentVertex != sourceIdx)
            throw runtime_error("Could not find the path");

        currentList.push_front(currentVertex);
        for(const size_t& predecessor: distancesPredecessors.second[currentVertex]) {
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
} // namespace PGL
