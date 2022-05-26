#ifndef BASE_GRAPH_UNDIRECTED_GRAPH_METRICS_H
#define BASE_GRAPH_UNDIRECTED_GRAPH_METRICS_H

#include <vector>
#include <list>
#include <array>
#include <unordered_map>

#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph { namespace metrics {


double getModularity(const UndirectedGraph& graph, const std::vector<size_t>& vertexCommunities);

}} // namespace BaseGraph::metrics

#endif
