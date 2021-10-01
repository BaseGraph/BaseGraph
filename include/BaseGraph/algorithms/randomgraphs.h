#ifndef BASE_GRAPH_RANDOM_GRAPHS_H
#define BASE_GRAPH_RANDOM_GRAPHS_H

#include <random>

#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph{


extern std::mt19937_64 rng;

UndirectedGraph generateErdosRenyiGraph(size_t n, double p);
UndirectedGraph generateSparseErdosRenyiGraph(size_t n, double p);
UndirectedGraph generateGraphWithDegreeDistributionStubMatching(const std::vector<size_t>& degreeDistribution);

std::vector<Edge> getEdgeVectorOfGraph(const UndirectedGraph& graph);
void shuffleGraphWithConfigurationModel(UndirectedGraph &graph, size_t swaps=0);
void shuffleGraphWithConfigurationModel(UndirectedGraph &graph, std::vector<Edge>& edgeVector, size_t swaps=0);

} // namespace BaseGraph

#endif
