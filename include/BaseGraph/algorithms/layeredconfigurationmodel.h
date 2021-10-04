#ifndef BASE_GRAPH_LAYERED_CONFIGURATION_MODEL_H
#define BASE_GRAPH_LAYERED_CONFIGURATION_MODEL_H

#include <random>

#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph{

void shuffleGraphWithLayeredConfigurationModel(UndirectedGraph &graph, size_t swaps=0);
void shuffleGraphWithLayeredConfigurationModel(UndirectedGraph &graph, std::vector<std::pair<VertexIndex, VertexIndex>>& edgeVector, size_t swaps=0);

} // namespace BaseGraph

#endif
