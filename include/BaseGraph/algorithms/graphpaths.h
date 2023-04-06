#ifndef BASE_GRAPH_PATHS_H
#define BASE_GRAPH_PATHS_H

#include <limits>
#include <list>
#include <vector>

#include "BaseGraph/directedgraph.h"

namespace BaseGraph {
namespace algorithms {

const size_t BASEGRAPH_SIZE_T_MAX = std::numeric_limits<size_t>::max();

typedef std::pair<std::vector<size_t>, std::vector<VertexIndex>> Predecessors;
typedef std::pair<std::vector<size_t>, std::vector<std::list<VertexIndex>>>
    MultiplePredecessors;
typedef std::list<VertexIndex> Path;
typedef std::list<std::list<VertexIndex>> MultiplePaths;

template <typename T>
Path findGeodesics(const T &graph, VertexIndex source, VertexIndex destination);
template <typename T>
MultiplePaths findAllGeodesics(const T &graph, VertexIndex source,
                               VertexIndex destination);
template <typename T>
std::vector<Path> findGeodesicsFromVertex(const T &graph, VertexIndex vertex);
template <typename T>
std::vector<MultiplePaths> findAllGeodesicsFromVertex(const T &graph,
                                                      VertexIndex vertex);

template <typename T>
Predecessors findPredecessorsOfVertex(const T &graph, VertexIndex vertex);
template <typename T>
Path findPathToVertexFromPredecessors(const T &graph, VertexIndex destination,
                                      const Predecessors &predecessors);
template <typename T>
Path findPathToVertexFromPredecessors(const T &graph, VertexIndex source,
                                      VertexIndex destination,
                                      const Predecessors &predecessors);

template <typename T>
MultiplePredecessors findAllPredecessorsOfVertex(const T &graph,
                                                 VertexIndex vertex);
template <typename T>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const T &graph, VertexIndex source, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);
template <typename T>
MultiplePaths findMultiplePathsToVertexFromPredecessors(
    const T &graph, VertexIndex destination,
    const MultiplePredecessors &distancesPredecessors);

} // namespace algorithms
} // namespace BaseGraph

#endif
