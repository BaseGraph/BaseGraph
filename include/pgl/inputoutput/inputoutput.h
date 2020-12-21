#ifndef PGL_INPUT_OUTPUT_H
#define PGL_INPUT_OUTPUT_H

#include <vector>

#include "pgl/directedgraph.h"


namespace PGL{

std::vector<std::string> loadGraphFromEdgelist(const DirectedGraph& graph);



} // namespace PGL

#endif
