#ifndef BASE_GRAPH_PERCOLATION_H
#define BASE_GRAPH_PERCOLATION_H

#include <list>

#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph { namespace random {

std::list<std::list<VertexIndex>> getClustersOfBondPercolation(size_t n, double p);


}} // namespace BaseGraph::random

#endif
