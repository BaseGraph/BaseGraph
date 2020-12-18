#ifndef PGL_GRAPH_PATHS_H
#define PGL_GRAPH_PATHS_H

#include <vector>
#include <list>
#include <limits>

#include "pgl/directedgraph.h"


namespace PGL{

const size_t SIZE_T_MAX = std::numeric_limits<size_t>::max();


std::pair<std::vector<size_t>, std::vector<size_t> > findGeodesicsOfVertex(const DirectedGraph& graph, size_t vertexIdx);
std::pair<std::vector<size_t>, std::vector<std::list<size_t> > > findEveryGeodesicsOfVertex(const DirectedGraph& graph, size_t vertexIdx);
std::list<size_t> findPathToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t destinationIdx,
        const std::pair<std::vector<size_t>, std::vector<size_t> >& predecessors);
std::list<size_t> findPathToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t sourceIdx, size_t destinationIdx,
        const std::pair<std::vector<size_t>, std::vector<size_t> >& predecessors);
std::list<std::list<size_t> > findMultiplePathsToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t sourceIdx, size_t destinationIdx,
        const std::pair<std::vector<size_t>, std::vector<std::list<size_t> > >& distancesPredecessors);
std::list<std::list<size_t> > findMultiplePathsToVertexFromPredecessorsIdx(const DirectedGraph& graph, size_t destinationIdx, 
        const std::pair<std::vector<size_t>, std::vector<std::list<size_t> > >& distancesPredecessors);
} // namespace PGL

#endif
