#include "BaseGraph/random/percolation.h"
#include "BaseGraph/random/randomgraphs.h"
#include "BaseGraph/metrics/general.h"


namespace BaseGraph { namespace random {

std::list<std::list<VertexIndex>> getClustersOfBondPercolation(size_t n, double p) {
    return metrics::findConnectedComponents(generateErdosRenyiRandomGraph(n, p));
}

}} // namespace BaseGraph::random
