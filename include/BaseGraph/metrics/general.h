#ifndef BASE_GRAPH_GENERAL_GRAPH_METRICS_H
#define BASE_GRAPH_GENERAL_GRAPH_METRICS_H

#include <vector>
#include <list>

#include "BaseGraph/directedgraph.h"


namespace BaseGraph{

typedef std::list<VertexIndex> Component;


template <typename T> double getClosenessCentralityOfVertexIdx(const T& graph, VertexIndex vertexIdx);
template <typename T> double getHarmonicMeanGeodesicOfVertexIdx(const T& graph, VertexIndex vertexIdx);
template <typename T> double getHarmonicCentralityOfVertexIdx(const T& graph, VertexIndex vertexIdx);
template <typename T> std::vector<double> getBetweennesses(const T& graph, bool normalizeWithGeodesicNumber=true);
template <typename T> std::vector<size_t> getDiameters(const T& graph);
template <typename T> std::vector<double> getAverageShortestPaths(const T& graph);
template <typename T> std::vector<std::vector<double> > getShortestPathsDistribution(const T& graph);

template <typename T> std::list<Component> findConnectedComponents(const T& graph);

} // namespace BaseGraph

#endif
