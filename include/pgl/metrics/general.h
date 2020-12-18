#ifndef PGL_GENERAL_GRAPH_METRICS_H
#define PGL_GENERAL_GRAPH_METRICS_H

#include <vector>
#include <list>

#include "pgl/directedgraph.h"


namespace PGL{

double getClosenessCentralityOfVertexIdx(const DirectedGraph& graph, size_t vertexIdx);
double getHarmonicMeanGeodesicOfVertexIdx(const DirectedGraph& graph, size_t vertexIdx);
double getHarmonicCentralityOfVertexIdx(const DirectedGraph& graph, size_t vertexIdx);
std::vector<double> getBetweenesses(const DirectedGraph& graph, bool normalizeWithGeodesicNumber=true);
std::vector<size_t> getDiameters(const DirectedGraph& graph);
std::vector<double> getAverageShortestPaths(const DirectedGraph& graph);
std::vector<std::vector<double> > getShortestPathsDistribution(const DirectedGraph& graph);

std::list<std::list<size_t> > findConnectedComponents(const DirectedGraph& graph);

} // namespace PGL

#endif
