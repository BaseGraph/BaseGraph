#include "pgl/algorithms/percolation.h"
#include "pgl/algorithms/randomgraphs.h"
#include "pgl/metrics/general.h"


namespace PGL{

std::list<std::list<size_t>> getClustersOfBondPercolation(size_t n, double p) {
    return findConnectedComponents(generateErdosRenyiGraph(n, p));
}


} // namespace PGL
