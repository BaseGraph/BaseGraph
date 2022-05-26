#include "directed.h"

#include <algorithm>
#include <array>
#include <map>
#include <set>
#include <unordered_set>


using namespace std;


namespace BaseGraph { namespace metrics {

template<typename T>
static list<T> intersection_of(const list<T>&, const list<T>&);
template<typename T>
static list<T> getUnionOfLists(const list<T>&, const list<T>&);


double getDensity(const DirectedGraph &graph) {
    size_t n = graph.getSize();
    return graph.getEdgeNumber() / ( (double) n*(n-1));
}

double getReciprocity(const DirectedGraph& graph) {
    size_t reciprocalEdgeNumber = 0;

    for (const VertexIndex& vertex: graph)
        for (const VertexIndex& neighbour: graph.getOutEdgesOfIdx(vertex))
            if (vertex < neighbour && graph.hasEdgeIdx(neighbour, vertex))
                reciprocalEdgeNumber += 2;

    return reciprocalEdgeNumber / (double) graph.getEdgeNumber();
}

vector<size_t> getReciprocalDegrees(const DirectedGraph& graph) {
    vector<size_t> reciprocities(graph.getSize(), 0);

    for (const VertexIndex& vertex: graph) {
        for (const VertexIndex& neighbour: graph.getOutEdgesOfIdx(vertex)) {
            if (vertex < neighbour && graph.hasEdgeIdx(neighbour, vertex)) {
                    reciprocities[vertex]++;
                    reciprocities[neighbour]++;
            }
        }
    }

    return reciprocities;
}

std::vector<double> getJaccardReciprocities(const DirectedGraph& graph)
    {  return getJaccardReciprocities(graph, getReciprocalDegrees(graph), graph.getInDegrees()); };

std::vector<double> getJaccardReciprocities(const DirectedGraph& graph, const std::vector<size_t> reciprocities, const std::vector<size_t> inDegrees) {
    if (reciprocities.size() != graph.getSize() || inDegrees.size() != graph.getSize())
        throw logic_error("The reciprocities and the in degrees must have the size of the graph");

    vector<double> jaccardReciprocities(reciprocities.begin(), reciprocities.end());


    for (const VertexIndex& vertex: graph)
        jaccardReciprocities[vertex] /= inDegrees[vertex] + graph.getOutDegreeOfIdx(vertex) - (double) reciprocities[vertex];

    return jaccardReciprocities;
}

std::vector<double> getReciprocityRatios(const DirectedGraph& graph)
    {  return getReciprocityRatios(graph, getReciprocalDegrees(graph), graph.getInDegrees()); };

std::vector<double> getReciprocityRatios(const DirectedGraph& graph, const std::vector<size_t> reciprocities, const std::vector<size_t> inDegrees) {
    if (reciprocities.size() != graph.getSize() || inDegrees.size() != graph.getSize())
        throw logic_error("The reciprocities and the in degrees must have the size of the graph");

    vector<double> reciprocityRatios(reciprocities.begin(), reciprocities.end());


    for (const VertexIndex& vertex: graph)
        reciprocityRatios[vertex] *= (double) 2/(inDegrees[vertex] + graph.getOutDegreeOfIdx(vertex));

    return reciprocityRatios;
}


vector<double> getUndirectedLocalClusteringCoefficients(const DirectedGraph& graph) {
    auto inEdges = graph.getInEdges();
    return getUndirectedLocalClusteringCoefficients(graph, findAllDirectedTriangles(graph, inEdges), inEdges);
}


vector<double> getUndirectedLocalClusteringCoefficients(const DirectedGraph& graph, const AdjacencyLists& inEdges) {
    return getUndirectedLocalClusteringCoefficients(graph, findAllDirectedTriangles(graph, inEdges), inEdges);
}


vector<double> getUndirectedLocalClusteringCoefficients(const DirectedGraph& graph, const list<array<VertexIndex, 3>>& triangles, const AdjacencyLists& inEdges) {
    if (inEdges.size() != graph.getSize()) throw logic_error("The inEdges vector must be the size of the graph");

    vector<double> localClusteringCoefficients(graph.getSize(), 0);

    for (auto& triangle: triangles) {
        localClusteringCoefficients[triangle[0]]++;
        localClusteringCoefficients[triangle[1]]++;
        localClusteringCoefficients[triangle[2]]++;
    }


    size_t undirectedDegree;

    for (const VertexIndex& vertex: graph) {
        undirectedDegree = getUnionOfLists(graph.getOutEdgesOfIdx(vertex), inEdges[vertex]).size();
        if (undirectedDegree>1)
            localClusteringCoefficients[vertex] /= undirectedDegree*(undirectedDegree-1)/2.;
    }
    return localClusteringCoefficients;
}

double getUndirectedGlobalClusteringCoefficient(const DirectedGraph& graph) {
    auto inEdges = graph.getInEdges();
    return getUndirectedGlobalClusteringCoefficient(graph, findAllDirectedTriangles(graph, inEdges), inEdges);
}

double getUndirectedGlobalClusteringCoefficient(const DirectedGraph& graph, const list<array<VertexIndex, 3>>& triangles, const AdjacencyLists& inEdges) {
    if (inEdges.size() != graph.getSize()) throw logic_error("The inEdges vector must be the size of the graph");


    size_t totalDegree, localTriangles, triadNumber(0);
    for (VertexIndex& vertex: graph) {
        totalDegree = inEdges[vertex].size() + graph.getOutDegreeOfIdx(vertex);

        localTriangles = getUnionOfLists(graph.getOutEdgesOfIdx(vertex), inEdges[vertex]).size();
        if (totalDegree > 1)
            triadNumber += localTriangles*(localTriangles-1)/2;
    }
    return (double) 3*triangles.size()/triadNumber;
}

list<array<VertexIndex, 3>> findAllDirectedTriangles(const DirectedGraph& graph) {
    return findAllDirectedTriangles(graph, graph.getInEdges());
}

list<array<VertexIndex, 3>> findAllDirectedTriangles(const DirectedGraph& graph, const AdjacencyLists& inEdges) {
    if (inEdges.size() != graph.getSize()) throw logic_error("The inEdges vector must be the size of the graph");
    list<array<VertexIndex, 3>> triangles;

    AdjacencyLists undirectedEdges(graph.getSize());

    for(const VertexIndex& vertex1: graph)
        undirectedEdges[vertex1] = getUnionOfLists(graph.getOutEdgesOfIdx(vertex1), inEdges[vertex1]);


    for(const VertexIndex& vertex1: graph)
        for (const VertexIndex& vertex2: undirectedEdges[vertex1])
            if (vertex1 < vertex2)
                for (const VertexIndex& vertex3: intersection_of(undirectedEdges[vertex1], undirectedEdges[vertex2]))
                    if (vertex2 < vertex3)
                        triangles.push_back({vertex1, vertex2, vertex3});

    return triangles;
}

static map<string, string> triangleEdgesToType = {
    {"-> -> -> ", "3cycle"},
    {"<- <- <- ", "3cycle"},

    {"<- -> -> ", "3nocycle"},
    {"-> <- -> ", "3nocycle"},
    {"-> -> <- ", "3nocycle"},
    {"-> <- <- ", "3nocycle"},
    {"<- -> <- ", "3nocycle"},
    {"<- <- -> ", "3nocycle"},

    {"<-> -> -> ", "4cycle"},
    {"-> <-> -> ", "4cycle"},
    {"-> -> <-> ", "4cycle"},
    {"<-> <- <- ", "4cycle"},
    {"<- <-> <- ", "4cycle"},
    {"<- <- <-> ", "4cycle"},

    {"<-> -> <- ", "4outward"},
    {"<- <-> -> ", "4outward"},
    {"-> <- <-> ", "4outward"},

    {"<-> <- -> ", "4inward"},
    {"-> <-> <- ", "4inward"},
    {"<- -> <-> ", "4inward"},

    {"-> <-> <-> ", "5cycle"},
    {"<-> -> <-> ", "5cycle"},
    {"<-> <-> -> ", "5cycle"},
    {"<- <-> <-> ", "5cycle"},
    {"<-> <- <-> ", "5cycle"},
    {"<-> <-> <- ", "5cycle"},

    {"<-> <-> <-> ", "6cycle"}
};

static const list<string> triangleTypes =
    {"3cycle", "3nocycle", "4cycle", "4outward", "4inward", "5cycle", "6cycle"};


map<string, size_t> getTriangleSpectrum(const DirectedGraph& graph, const list<array<VertexIndex, 3> >& triangles) {
    map<string, size_t> triangleSpectrum;

    for (const string& triangleType: triangleTypes)
        triangleSpectrum[triangleType] = 0;


    string triangleEdgesRepresentation;
    list<Edge> triangleEdges = {{0, 1}, {1, 2}, {2, 0}};
    bool ij_isEdge;
    bool ji_isEdge;
    for (auto& triangle: triangles) {
        triangleEdgesRepresentation = "";

        for (auto& edge: triangleEdges) {
            ij_isEdge = graph.hasEdgeIdx(triangle[edge.first], triangle[edge.second]);
            ji_isEdge = graph.hasEdgeIdx(triangle[edge.second], triangle[edge.first]);

            if (ij_isEdge && !ji_isEdge)
                triangleEdgesRepresentation += "-> ";
            else if (!ij_isEdge && ji_isEdge)
                triangleEdgesRepresentation += "<- ";
            else  // if edge ij and ji don't exist then the input isn't a complete triangle
                triangleEdgesRepresentation += "<-> ";
        }
        triangleSpectrum[triangleEdgesToType[triangleEdgesRepresentation]]++;
    }

    return triangleSpectrum;
}


map<size_t, size_t> getOutDegreeHistogram(const DirectedGraph& graph){
    map<size_t, size_t> outDegreeHistogram;

    auto outDegrees = graph.getOutDegrees();

    for (auto degree: outDegrees) {
        if (outDegreeHistogram.count(degree) == 0)
          outDegreeHistogram[degree] = 1;
        else
          outDegreeHistogram[degree] += 1;
    }

    return outDegreeHistogram;
}


map<size_t, size_t> getInDegreeHistogram(const DirectedGraph& graph){
    return getInDegreeHistogram(graph, graph.getInDegrees());
}


map<size_t, size_t> getInDegreeHistogram(const DirectedGraph& graph, const std::vector<size_t> inDegrees){
    map<size_t, size_t> inDegreeHistogram;

    for (auto degree: inDegrees) {
        if (inDegreeHistogram.count(degree) == 0)
            inDegreeHistogram[degree] = 1;
        else
            inDegreeHistogram[degree] += 1;
    }

    return inDegreeHistogram;
}


// From https://stackoverflow.com/questions/38993415/how-to-apply-the-intersection-between-two-lists-in-c
template<typename T>
static std::list<T> intersection_of(const std::list<T>& a, const std::list<T>& b){
    std::list<T> rtn;
    std::unordered_multiset<T> st;
    std::for_each(a.begin(), a.end(), [&st](const T& k){ st.insert(k); });
    std::for_each(b.begin(), b.end(),
        [&st, &rtn](const T& k){
            auto iter = st.find(k);
            if(iter != st.end()){
                rtn.push_back(k);
                st.erase(iter);
            }
        }
    );
    return rtn;
}

template<typename T>
static list<T> getUnionOfLists(const list<T>& list1, const list<T>& list2) {
    list<T> listUnion = list1;
    unordered_set<T> list1Set(list1.begin(), list1.end());

    for (auto& element: list2)
        if (list1Set.find(element) == list1Set.end())
            listUnion.push_back(element);

    return listUnion;
}


}} // namespace BaseGraph::metrics
