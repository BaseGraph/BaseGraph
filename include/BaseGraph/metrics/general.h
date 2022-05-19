#ifndef BASE_GRAPH_GENERAL_GRAPH_METRICS_H
#define BASE_GRAPH_GENERAL_GRAPH_METRICS_H

#include <vector>
#include <list>

#include "BaseGraph/directedgraph.h"


namespace BaseGraph { namespace metrics {

typedef std::list<VertexIndex> Component;


template <typename T> std::vector<double> getClosenessCentralities(const T& graph);
template <typename T> std::vector<double> getHarmonicCentralities(const T& graph);
template <typename T> std::vector<double> getBetweennessCentralities(const T& graph, bool normalizeWithGeodesicNumber=true);

template <typename T> std::vector<size_t> getShortestPathLengthsFromVertexIdx(const T& graph, VertexIndex sourceIdx);
template <typename T> std::vector<double> getShortestPathAverages(const T& graph);
template <typename T> std::vector<double> getShortestPathHarmonicAverages(const T& graph);
template <typename T> std::vector<size_t> getDiameters(const T& graph);
template <typename T> std::vector<std::unordered_map<size_t, double>> getShortestPathsDistribution(const T& graph);

template <typename T> std::list<Component> findConnectedComponents(const T& graph);

}} // namespace BaseGraph::metrics

#endif
