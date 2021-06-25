#include "BaseGraph/algorithms/percolation.h"
#include "BaseGraph/algorithms/randomgraphs.h"
#include "BaseGraph/metrics/general.h"


namespace BaseGraph{

std::list<std::list<VertexIndex>> getClustersOfBondPercolation(size_t n, double p) {
    return findConnectedComponents(generateErdosRenyiGraph(n, p));
}


} // namespace BaseGraph
