#ifndef PGL_FILEIO_H
#define PGL_FILEIO_H

#include <map>
#include <string>

#include "pgl/directedgraph.h"


namespace PGL{

std::map<std::string, std::size_t> loadGraphFromEdgelist(const std::string edgelist_filename, DirectedGraph& graph, const bool allow_multiedges = false, const bool allow_selfloops = false);



} // namespace PGL

#endif
