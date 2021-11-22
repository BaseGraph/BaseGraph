#ifndef BASE_GRAPH_RANDOM_GRAPHS_H
#define BASE_GRAPH_RANDOM_GRAPHS_H

#include <random>

#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph{


extern std::mt19937_64 rng;

void seed(size_t _seed);
UndirectedGraph generateErdosRenyiRandomGraph(size_t n, size_t m);
UndirectedGraph generateGilbertRandomGraph(size_t n, double p);
UndirectedGraph generateGraphWithDegreeDistributionStubMatching(const std::vector<size_t>& degreeDistribution);

std::vector<Edge> getEdgeVectorOfGraph(const UndirectedGraph&);
void shuffleGraphWithConfigurationModel(UndirectedGraph&, size_t swaps=0);
void shuffleGraphWithConfigurationModel(UndirectedGraph&, std::vector<Edge>& edges, size_t swaps=0);
UndirectedGraph generateSmallWorldRandomGraph(size_t n, size_t d, double p);

} // namespace BaseGraph

#endif
