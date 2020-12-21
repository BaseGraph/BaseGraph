#ifndef PGL_INPUT_OUTPUT_H
#define PGL_INPUT_OUTPUT_H

#include <map>
#include <string>

#include "pgl/directedgraph.h"


namespace PGL{

std::map<std::string, int> loadGraphFromEdgelist(const std::string edgelist_filename, DirectedGraph& graph, bool allow_multiedges = false, bool allow_selfloops = false);



} // namespace PGL

#endif
