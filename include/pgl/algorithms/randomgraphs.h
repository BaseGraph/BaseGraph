#ifndef PGL_RANDOM_GRAPHS_H
#define PGL_RANDOM_GRAPHS_H

#include <random>

#include "pgl/undirectedgraph.h"


namespace PGL{


extern std::mt19937_64 rng;

UndirectedGraph generateErdosRenyiGraph(size_t n, double p);
UndirectedGraph generateSparseErdosRenyiGraph(size_t n, double p);
UndirectedGraph generateGraphWithDegreeDistributionStubMatching(const std::vector<size_t>& degreeDistribution);

std::vector<std::pair<size_t, size_t>> getEdgeVectorOfGraph(const UndirectedGraph& graph);
void shuffleGraphWithConfigurationModel(PGL::UndirectedGraph &graph, size_t swaps=0);
void shuffleGraphWithConfigurationModel(PGL::UndirectedGraph &graph, std::vector<std::pair<size_t, size_t>>& edgeVector, size_t swaps=0);

} // namespace PGL

#endif
