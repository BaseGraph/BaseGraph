#include "undirected.h"
#include "BaseGraph/extensions/metrics/undirected.h"


namespace BaseGraph { namespace metrics {

double getTwiceModularity(const UndirectedGraph& graph, const std::vector<size_t>& vertexCommunities) {
    return 2*getModularity(graph, vertexCommunities);
}

}} // namespace BaseGraph::metrics
