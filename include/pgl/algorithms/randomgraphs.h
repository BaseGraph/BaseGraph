#ifndef PGL_RANDOM_GRAPHS_H
#define PGL_RANDOM_GRAPHS_H

#include <random>
#include <chrono>

#include "pgl/undirectedgraph.h"


namespace PGL{

UndirectedGraph generateErdosRenyiGraph(size_t n, double p);
UndirectedGraph generateSparseErdosRenyiGraph(size_t n, double p);

void rewireWithConfigurationModel(PGL::UndirectedGraph& graph, size_t requiredSwaps=0, size_t seed=std::chrono::system_clock::now().time_since_epoch().count());

} // namespace PGL

#endif
