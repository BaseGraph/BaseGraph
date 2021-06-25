#ifndef PGL_GRAPH_PATHS_H
#define PGL_GRAPH_PATHS_H

#include <vector>
#include <list>
#include <limits>

#include "pgl/directedgraph.h"


namespace PGL{

const size_t PGL_SIZE_T_MAX = std::numeric_limits<size_t>::max();

typedef std::pair<std::vector<size_t>, std::vector<VertexIndex>> Predecessors;
typedef std::pair<std::vector<size_t>, std::vector<std::list<VertexIndex>>> MultiplePredecessors;
typedef std::list<VertexIndex> Path;
typedef std::list<std::list<VertexIndex>> MultiplePaths;



template <typename T> Path findGeodesicsIdx(const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx);
template <typename T> MultiplePaths findAllGeodesicsIdx(const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx);
template <typename T> std::vector<Path> findGeodesicsFromVertexIdx(const T& graph, VertexIndex vertexIdx);
template <typename T> std::vector<MultiplePaths> findAllGeodesicsFromVertexIdx(const T& graph, VertexIndex vertexIdx);


template <typename T> Predecessors findPredecessorsOfVertexIdx(const T& graph, VertexIndex vertexIdx);
template <typename T> Path findPathToVertexFromPredecessorsIdx(
        const T& graph, VertexIndex destinationIdx, const Predecessors& predecessors);
template <typename T> Path findPathToVertexFromPredecessorsIdx(
        const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx, const Predecessors& predecessors);

template <typename T> std::vector<size_t> findShortestPathLengthsFromVertexIdx(const T& graph, VertexIndex sourceIdx) {
    return findPredecessorsOfVertexIdx(graph, sourceIdx).first;
}


template <typename T> MultiplePredecessors findAllPredecessorsOfVertexIdx(const T& graph, VertexIndex vertexIdx);
template <typename T> MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const T& graph, VertexIndex sourceIdx, VertexIndex destinationIdx, const MultiplePredecessors& distancesPredecessors);
template <typename T> MultiplePaths findMultiplePathsToVertexFromPredecessorsIdx(
        const T& graph, VertexIndex destinationIdx, const MultiplePredecessors& distancesPredecessors);

} // namespace PGL

#endif
