#ifndef PGL_PERCOLATION_H
#define PGL_PERCOLATION_H

#include <list>

#include "pgl/undirectedgraph.h"


namespace PGL{

std::list<std::list<VertexIndex>> getClustersOfBondPercolation(size_t n, double p);


} // namespace PGL

#endif
