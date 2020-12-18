#include <random>
#include <iostream>

#include "pgl/algorithms/randomgraphs.h"


using namespace std;

namespace PGL{

static std::mt19937_64 generator;


UndirectedGraph generateErdosRenyiGraph(size_t n, double p) {
    UndirectedGraph graph(n);

    uniform_real_distribution<double> uniform01Distribution(0, 1);

    for (size_t i=0; i<n; i++)
        for (size_t j=i+1; j<n; j++)
            if (uniform01Distribution(generator)<p)
                graph.addEdgeIdx(i, j);

    return graph;
}

// From https://journals.aps.org/pre/abstract/10.1103/PhysRevE.71.036113
UndirectedGraph generateSparseErdosRenyiGraph(size_t n, double p) {
    UndirectedGraph graph(n);
    size_t i=0;
    size_t j=0;
    

    uniform_real_distribution<double> uniform01Distribution(0, 1);
    double r;

    while (i<n) {
        r = uniform01Distribution(generator);
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

static vector<pair<size_t, size_t>> getEdgeListFromGraph(const UndirectedGraph& graph) {
    vector<pair<size_t, size_t>> edgeList(graph.getEdgeNumber());

    for (size_t& vertex1: graph)
        for (size_t vertex2: graph.getNeighboursOfIdx(vertex1))
            if (vertex1 < vertex2)
                edgeList.push_back({vertex1, vertex2});

    return edgeList;
}

void rewireWithConfigurationModel(UndirectedGraph &graph, size_t requiredSwaps, size_t seed) {
    generator.seed(seed);
    if (requiredSwaps == 0) requiredSwaps = graph.getEdgeNumber();

    const auto edgeList = getEdgeListFromGraph(graph);
    size_t edgeNumber = edgeList.size();

    std::uniform_real_distribution<double> uniform01Distribution(0, 1);

    size_t swaps = 0;
    size_t remainingAttemps = requiredSwaps * (1+uniform01Distribution(generator));

    vector<bool> visitedEdges(edgeNumber, false);
    

    size_t edge1Idx, edge2Idx;
    pair<size_t, size_t> newEdge1, newEdge2;
    while (remainingAttemps > 0) {
    
        edge1Idx = edgeNumber*uniform01Distribution(generator);
        if (edge1Idx == edgeNumber) continue;  // safety check but extremely unlikely
        visitedEdges[edge1Idx] = true;

        edge2Idx = (edgeNumber-1)*uniform01Distribution(generator);
        if (edge2Idx >= edge1Idx) edge2Idx++;
        visitedEdges[edge2Idx] = true;

        const auto& currentEdge1 = edgeList[edge1Idx];
        const auto& currentEdge2 = edgeList[edge2Idx];

        if (uniform01Distribution(generator) < 0.5) {
            newEdge1 = {currentEdge1.first, currentEdge2.first};
            newEdge2 = {currentEdge1.second, currentEdge2.second};
        }
        else {
            newEdge1 = {currentEdge1.first, currentEdge2.second};
            newEdge2 = {currentEdge1.second, currentEdge2.first};
        }

        if (graph.isEdgeIdx(newEdge1.first, newEdge1.second) || graph.isEdgeIdx(newEdge2.first, newEdge2.second))
            continue; 

        graph.removeEdgeIdx(currentEdge1.first, currentEdge1.second);
        graph.removeEdgeIdx(currentEdge2.first, currentEdge2.second);

        graph.addEdgeIdx(newEdge1.first, newEdge1.second);
        graph.addEdgeIdx(newEdge2.first, newEdge2.second);

        if (swaps > requiredSwaps)
            remainingAttemps--;

        swaps++;
    }

    if (find(visitedEdges.begin(), visitedEdges.end(), false) != visitedEdges.end())
        cerr << "Warning: Not all edges have been swapped at least once." << std::endl;
}

} // namespace PGL
