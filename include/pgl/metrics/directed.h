#ifndef PGL_DIRECTED_GRAPH_METRICS_H
#define PGL_DIRECTED_GRAPH_METRICS_H

#include <array>
#include <map>
#include <vector>

#include "pgl/directedgraph.h"


namespace PGL{

double getDensity(const DirectedGraph& graph);

std::list<std::array<VertexIndex, 3>> findAllDirectedTriangles(const DirectedGraph& graph);
std::list<std::array<VertexIndex, 3>> findAllDirectedTriangles(const DirectedGraph& graph, const std::vector<std::list<VertexIndex>>& inEdges);
std::map<std::string, size_t> getTriangleSpectrum(const DirectedGraph& graph, const std::list<std::array<VertexIndex, 3>>& triangles);
std::vector<double> getUndirectedLocalClusteringCoefficients(const DirectedGraph& graph);
std::vector<double> getUndirectedLocalClusteringCoefficients(const DirectedGraph& graph, const std::vector<std::list<VertexIndex>>& inEdges);
std::vector<double> getUndirectedLocalClusteringCoefficients(const DirectedGraph& graph, const std::list<std::array<VertexIndex, 3>>& triangles, const std::vector<std::list<VertexIndex>>& inEdges);
double getUndirectedGlobalClusteringCoefficient(const DirectedGraph& graph);
double getUndirectedGlobalClusteringCoefficient(const DirectedGraph& graph, const std::list<std::array<VertexIndex, 3>>& triangles, const std::vector<std::list<size_t>>& inDegrees);

double getReciprocity(const DirectedGraph& graph);

std::vector<size_t> getReciprocalDegrees(const DirectedGraph& graph);

std::vector<double> getJaccardReciprocities(const DirectedGraph& graph);
std::vector<double> getJaccardReciprocities(const DirectedGraph& graph, const std::vector<size_t> reciprocities, const std::vector<size_t> inDegrees);
std::vector<double> getReciprocityRatios(const DirectedGraph& graph);
std::vector<double> getReciprocityRatios(const DirectedGraph& graph, const std::vector<size_t> reciprocities, const std::vector<size_t> inDegrees);

std::map<size_t, size_t> getOutDegreeHistogram(const DirectedGraph& graph);
std::map<size_t, size_t> getInDegreeHistogram(const DirectedGraph& graph);
std::map<size_t, size_t> getInDegreeHistogram(const DirectedGraph& graph, const std::vector<size_t> inDegrees);

} // namespace PGL

#endif
