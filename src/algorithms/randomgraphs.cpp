#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "BaseGraph/algorithms/randomgraphs.h"
#include "BaseGraph/undirectedgraph.h"


using namespace std;


namespace BaseGraph{


std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());


static UndirectedGraph generateStandardErdosRenyiGraph(size_t n, double p) {
    UndirectedGraph graph(n);

    uniform_real_distribution<double> uniform01Distribution(0, 1);

    for (VertexIndex i=0; i<n; i++)
        for (VertexIndex j=i+1; j<n; j++)
            if (uniform01Distribution(rng)<p)
                graph.addEdgeIdx(i, j);

    return graph;
}

// From https://journals.aps.org/pre/abstract/10.1103/PhysRevE.71.036113
static UndirectedGraph generateSparseErdosRenyiGraph(size_t n, double p) {
    UndirectedGraph graph(n);
    VertexIndex i=0;
    VertexIndex j=0;


    uniform_real_distribution<double> uniform01Distribution(0, 1);
    double r;

    while (i<n) {
        r = uniform01Distribution(rng);
        j += 1+floor(log(1-r)/log(1-p));
        while (j >= i && i < n) {
            j -= i;
            i++;
        }
        if (i<n)
            graph.addEdgeIdx(i, j);
    }

    return graph;
}

UndirectedGraph generateErdosRenyiGraph(size_t n, double p) {
    if ( p < 1-2./(n-1) )  // Is on average faster
        return generateSparseErdosRenyiGraph(n, p);
    return generateStandardErdosRenyiGraph(n, p);
}


UndirectedGraph generateGraphWithDegreeDistributionStubMatching(const vector<size_t>& degreeDistribution) {
    size_t n = degreeDistribution.size();
    UndirectedGraph graph(n);

    vector<VertexIndex> stubs;

    for (VertexIndex i=0; i<n; i++){
        const size_t& degree = degreeDistribution[i];
        if (degree > 0)
            stubs.insert(stubs.end(), degree, i);
    }

    random_shuffle(stubs.begin(), stubs.end());

    VertexIndex vertex1;
    auto stubIterator = stubs.begin();
    while (stubIterator != stubs.end()) {

        vertex1 = *stubIterator;
        stubIterator++;
        if (stubIterator == stubs.end()) break;

        if (vertex1 != *stubIterator && !graph.isEdgeIdx(vertex1, *stubIterator))  // no loops and multiedges
            graph.addEdgeIdx(vertex1, *stubIterator);
        stubIterator++;
    }

    return graph;
}

vector<Edge> getEdgeVectorOfGraph(const UndirectedGraph& graph) {
    vector<Edge> edges;

    for (VertexIndex& vertex1: graph)
        for (VertexIndex vertex2: graph.getNeighboursOfIdx(vertex1))
            if (vertex1 < vertex2)
                edges.push_back({vertex1, vertex2});

    return edges;
}

void shuffleGraphWithConfigurationModel(UndirectedGraph &graph, size_t swaps) {
    auto edges = getEdgeVectorOfGraph(graph);
    shuffleGraphWithConfigurationModel(graph, edges, swaps);
}

void shuffleGraphWithConfigurationModel(UndirectedGraph &graph, vector<Edge>& edges, size_t swaps) {
    if (swaps == 0) swaps = 2*graph.getEdgeNumber();

    size_t edgeNumber = edges.size();
    std::uniform_real_distribution<double> uniform01Distribution(0, 1);


    size_t edge1Idx, edge2Idx;
    Edge newEdge1, newEdge2;

    for (size_t i=0; i<swaps; i++) {
        edge1Idx = edgeNumber*uniform01Distribution(rng);

        edge2Idx = (edgeNumber-1)*uniform01Distribution(rng);
        if (edge2Idx >= edge1Idx) edge2Idx++;

        const auto& currentEdge1 = edges[edge1Idx];
        const auto& currentEdge2 = edges[edge2Idx];

        if (uniform01Distribution(rng) < 0.5) {
            newEdge1 = {currentEdge1.first, currentEdge2.first};
            newEdge2 = {currentEdge1.second, currentEdge2.second};
        }
        else {
            newEdge1 = {currentEdge1.first, currentEdge2.second};
            newEdge2 = {currentEdge1.second, currentEdge2.first};
        }

        if (newEdge1.first==newEdge1.second || newEdge2.first==newEdge2.second)
            continue;

        if (graph.isEdgeIdx(newEdge1) || graph.isEdgeIdx(newEdge2))
            continue;

        graph.removeEdgeIdx(currentEdge1);
        graph.removeEdgeIdx(currentEdge2);
        graph.addEdgeIdx(newEdge1, true);
        graph.addEdgeIdx(newEdge2, true);

        edges[edge1Idx] = newEdge1;
        edges[edge2Idx] = newEdge2;
    }
}

} // namespace BaseGraph
