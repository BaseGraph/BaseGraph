#ifndef PGL_INPUT_OUTPUT_H
#define PGL_INPUT_OUTPUT_H

#include <vector>

#include "pgl/directedgraph.h"


namespace PGL{

std::vector<std::string> loadGraphFromEdgelist(const string edgelist_filename, DirectedGraph& graph, bool ignore_multiedges = false, bool ignore_selfloops = false);



} // namespace PGL

#endif
