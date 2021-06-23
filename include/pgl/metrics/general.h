#ifndef PGL_GENERAL_GRAPH_METRICS_H
#define PGL_GENERAL_GRAPH_METRICS_H

#include <vector>
#include <list>

#include "pgl/directedgraph.h"


namespace PGL{

template <typename T> double getClosenessCentralityOfVertexIdx(const T& graph, size_t vertexIdx);
template <typename T> double getHarmonicMeanGeodesicOfVertexIdx(const T& graph, size_t vertexIdx);
template <typename T> double getHarmonicCentralityOfVertexIdx(const T& graph, size_t vertexIdx);
template <typename T> std::vector<double> getBetweennesses(const T& graph, bool normalizeWithGeodesicNumber=true);
template <typename T> std::vector<size_t> getDiameters(const T& graph);
template <typename T> std::vector<double> getAverageShortestPaths(const T& graph);
template <typename T> std::vector<std::vector<double> > getShortestPathsDistribution(const T& graph);

template <typename T> std::list<std::list<size_t> > findConnectedComponents(const T& graph);

} // namespace PGL

#endif
