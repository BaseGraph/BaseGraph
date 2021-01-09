#ifndef PGL_UNDIRECTED_GRAPH_METRICS_H
#define PGL_UNDIRECTED_GRAPH_METRICS_H

#include <vector>
#include <list>
#include <array>
#include <map>

#include "pgl/undirectedgraph.h"


namespace PGL{


double getDegreeCorrelation(const UndirectedGraph& graph, double averageDegree);

std::list<std::array<size_t, 3>> findAllTriangles(const PGL::UndirectedGraph& graph);
size_t countTriangles(const PGL::UndirectedGraph& graph);
size_t countTrianglesAroundVertexIdx(const PGL::UndirectedGraph& graph, size_t vertexIdx);
std::vector<double> getDegreeDistribution(const PGL::UndirectedGraph& graph);

double getGlobalClusteringCoefficient(const PGL::UndirectedGraph& graph);
double getGlobalClusteringCoefficient(const PGL::UndirectedGraph& graph, const std::vector<size_t>& vertexTriangleNumbers);

std::vector<double> getLocalClusteringCoefficients(const PGL::UndirectedGraph& graph);
std::vector<double> getClusteringSpectrum(const PGL::UndirectedGraph& graph);
std::vector<double> getRedundancy(const PGL::UndirectedGraph& graph);

std::pair<std::vector<size_t>, std::vector<size_t>> getKShellsAndOnionLayers(const UndirectedGraph& graph);
std::vector<size_t> getKShells(const UndirectedGraph& graph);
std::vector<size_t> getOnionLayers(const UndirectedGraph& graph);
std::vector<std::list<double>> getOnionSpectrum(const UndirectedGraph& graph);
std::vector<std::list<double>> getOnionSpectrum(const UndirectedGraph& graph, const std::vector<size_t>& kshells, const std::vector<size_t>& onionLayers);
std::list<size_t> getKCore(const UndirectedGraph& graph, size_t k);
std::list<size_t> getKCore(size_t k, const std::vector<size_t>& kshells);

std::list<size_t> getNeighbourhoodDegreesOfVertexIdx(const PGL::UndirectedGraph& graph, size_t vertexIdx);
std::vector<double> getNeighbourDegreeSpectrum(const PGL::UndirectedGraph& graph, bool normalized=false);

double getModularity(const UndirectedGraph& graph, const std::vector<size_t>& vertexCommunities);

} // namespace PGL

#endif
