#ifndef BASE_GRAPH_UNDIRECTED_GRAPH_METRICS_H
#define BASE_GRAPH_UNDIRECTED_GRAPH_METRICS_H

#include <vector>
#include <list>
#include <array>
#include <unordered_map>

#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph { namespace metrics {

double getDegreeCorrelation(const UndirectedGraph& graph);
double getDegreeCorrelation(const UndirectedGraph& graph, double averageDegree);

std::list<std::array<VertexIndex, 3>> findAllTriangles(const UndirectedGraph& graph);
size_t countTriangles(const UndirectedGraph& graph);
size_t countTrianglesAroundVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
std::vector<double> getDegreeDistribution(const UndirectedGraph& graph);

double getGlobalClusteringCoefficient(const UndirectedGraph& graph);
double getGlobalClusteringCoefficient(const UndirectedGraph& graph, const std::vector<size_t>& vertexTriangleNumbers);

std::vector<double> getLocalClusteringCoefficients(const UndirectedGraph& graph);
std::unordered_map<size_t, double> getClusteringSpectrum(const UndirectedGraph& graph);
std::vector<double> getRedundancy(const UndirectedGraph& graph);

std::pair<std::vector<size_t>, std::vector<size_t>> getKShellsAndOnionLayers(const UndirectedGraph& graph);
std::vector<size_t> getKShells(const UndirectedGraph& graph);
std::vector<size_t> getOnionLayers(const UndirectedGraph& graph);
std::unordered_map<size_t, std::list<double>> getOnionSpectrum(const UndirectedGraph& graph);
std::unordered_map<size_t, std::list<double>> getOnionSpectrum(const UndirectedGraph& graph, const std::vector<size_t>& kshells, const std::vector<size_t>& onionLayers);
std::list<VertexIndex> getKCore(const UndirectedGraph& graph, size_t k);
std::list<VertexIndex> getKCore(size_t k, const std::vector<size_t>& kshells);

std::list<size_t> getNeighbourhoodDegreesOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx);
std::vector<double> getNeighbourDegreeSpectrum(const UndirectedGraph& graph, bool normalized=false);

double getModularity(const UndirectedGraph& graph, const std::vector<size_t>& vertexCommunities);

}} // namespace BaseGraph::metrics

#endif
