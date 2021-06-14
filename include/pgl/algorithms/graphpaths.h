#ifndef PGL_GRAPH_PATHS_H
#define PGL_GRAPH_PATHS_H

#include <vector>
#include <list>
#include <limits>

#include "pgl/directedgraph.h"


namespace PGL{

const size_t PGL_SIZE_T_MAX = std::numeric_limits<size_t>::max();

typedef std::pair<std::vector<size_t>, std::vector<size_t>> Predecessors;
typedef std::pair<std::vector<size_t>, std::vector<std::list<size_t>>> MultiplePredecessors;
typedef std::list<size_t> Path;
typedef std::list<std::list<size_t>> MultiplePaths;


template <typename T> Predecessors findGeodesicsOfVertex(const T& graph, size_t vertexIdx);
template <typename T> Path findPathToVertexFromPredecessorsIdx(
        const T& graph, size_t destinationIdx, const Predecessors& predecessors);
template <typename T> Path findPathToVertexFromPredecessorsIdx(
        const T& graph, size_t sourceIdx, size_t destinationIdx, const Predecessors& predecessors);


template <typename T> MultiplePredecessors findEveryGeodesicsOfVertex(const T& graph, size_t vertexIdx);
template <typename T> MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const T& graph, size_t sourceIdx, size_t destinationIdx, const MultiplePredecessors& distancesPredecessors);
template <typename T> MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const T& graph, size_t destinationIdx, const MultiplePredecessors& distancesPredecessors);

} // namespace PGL

#endif
