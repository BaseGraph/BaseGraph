#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <string>

#include "BaseGraph/algorithms/randomgraphs.h"
#include "BaseGraph/undirectedgraph.h"


using namespace std;


namespace BaseGraph{


std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());

inline static void validateProbability(double p) {
    if (!(p>=0 && p<=1))
        throw std::invalid_argument("Argument (" + std::to_string(p)+ ") is not a valid probability."
                                    "Value must be in [0, 1].");
}

inline static void validateEdgeNumber(size_t n, size_t m) {
    if (m > n*(n-1)*.5)
        throw std::invalid_argument("Argument (" + std::to_string(m)+ ") is not a valid edge number"
                                    "Value must be in [0,"+std::to_string(n*(n-1)*.5)+"].");
}


static UndirectedGraph generateStandardGilbertRandomGraph(size_t n, double p) {
    validateProbability(p);

    UndirectedGraph randomGraph(n);

    uniform_real_distribution<double> uniform01Distribution(0, 1);

    for (VertexIndex i=0; i<n; i++)
        for (VertexIndex j=i+1; j<n; j++)
            if (uniform01Distribution(rng)<p)
                randomGraph.addEdgeIdx(i, j, true);

    return randomGraph;
}

// Following algorithms are from
// https://journals.aps.org/pre/abstract/10.1103/PhysRevE.71.036113


static UndirectedGraph generateSparseGilbertRandomGraph(size_t n, double p) {
    validateProbability(p);

    UndirectedGraph randomGraph(n);
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
            randomGraph.addEdgeIdx(i, j, true);
    }

    return randomGraph;
}

UndirectedGraph generateGilbertRandomGraph(size_t n, double p) {
    if ( p < 1-2./(n-1) )  // Is on average faster
        return generateSparseGilbertRandomGraph(n, p);
    return generateStandardGilbertRandomGraph(n, p);
}

inline static Edge getUndirectedEdgeFromIndex(size_t index, size_t n) {
    const size_t i = 1 + floor(-.5 + sqrt(.25+2*index));
    const size_t j = index - i*(i-1)*.5;
    return {i, j};
}

static UndirectedGraph generateErdosRenyiRandomGraphWithRetries(size_t n, size_t m) {
    validateEdgeNumber(n, m);

    UndirectedGraph randomGraph(n);

    unordered_set<size_t> existingEdges;
    size_t newEdgeIndex;
    bool newEdgeFound;
    auto uniformEdgeDistribution = std::uniform_int_distribution<size_t>(0, n*(n-1)*.5-1);

    for (size_t i=0; i<m; i++) {
        newEdgeFound = false;
        while (!newEdgeFound) {
            newEdgeIndex = uniformEdgeDistribution(rng);
            newEdgeFound = existingEdges.find(newEdgeIndex) == existingEdges.end();
        }
        existingEdges.insert(newEdgeIndex);
        randomGraph.addEdgeIdx(getUndirectedEdgeFromIndex(newEdgeIndex, n), true);
    }
    return randomGraph;
}

// Slightly slower for small edge number
static UndirectedGraph generateErdosRenyiRandomGraphFisherYates(size_t n, size_t m) {
    validateEdgeNumber(n, m);

    UndirectedGraph randomGraph(n);

    unordered_map<size_t, size_t> edgeReplacements;
    size_t newEdgeIndex;
    const size_t maxEdgeNumber = .5*n*(n-1)-1;

    for (size_t i=0; i<m; i++) {
        newEdgeIndex = std::uniform_int_distribution<size_t>(i, maxEdgeNumber)(rng);

        if (edgeReplacements.find(newEdgeIndex) == edgeReplacements.end())
            randomGraph.addEdgeIdx(getUndirectedEdgeFromIndex(newEdgeIndex, n), true);
        else
            randomGraph.addEdgeIdx(getUndirectedEdgeFromIndex(edgeReplacements[newEdgeIndex], n), true);

        if (edgeReplacements.find(i) == edgeReplacements.end())
            edgeReplacements[newEdgeIndex] = i;
        else
            edgeReplacements[newEdgeIndex] = edgeReplacements[i];
    }
    return randomGraph;
}


UndirectedGraph generateErdosRenyiRandomGraph(size_t n, size_t m) {
    if (m < .25*n*(n-1))
        return generateErdosRenyiRandomGraphWithRetries(n, m);
    return generateErdosRenyiRandomGraphFisherYates(n, m);
}

UndirectedGraph generateSmallWorldRandomGraph(size_t n, size_t d, double p) {
    validateProbability(p);
    if ( !(1<=d && d<= floor(.5*(n-1))) )
        throw invalid_argument("Argument ("+std::to_string(d)+") is not a valid degree parameter for the small-world model."
                " Value must be in [1,"+std::to_string(int(floor(.5*(n-1))))+"].");

    UndirectedGraph randomGraph(n);
    auto uniform01Distribution = std::uniform_real_distribution<double>(0, 1);

    unordered_map<size_t, size_t> edgeReplacements;
    double r = uniform01Distribution(rng);
    long int k = floor(log(1-r)/log(1-p));
    size_t m = 0;
    size_t j;

    if (p < 1)
        for (size_t v=0; v<n; v++) {
            for (size_t i=1; i<=d; i++) {
                if (k>0) {
                    j = v*(v-1)*.5 + (v+i)%n;
                    randomGraph.addEdgeIdx(getUndirectedEdgeFromIndex(j, n), true);
                    k--;
                    m++;

                    if (edgeReplacements.find(m) == edgeReplacements.end())
                        edgeReplacements[j] = m;
                    else
                        edgeReplacements[j] = edgeReplacements[m];
                }
                else {
                    r = uniform01Distribution(rng);
                    k = floor(log(1-r)/log(1-p));
                }
            }
        }

    size_t r_prime;
    for (size_t i=m+1; i<=n*d; i++) {
        r_prime = std::uniform_int_distribution<size_t>(i, n*(n-1)*.5-1)(rng);

        if (edgeReplacements.find(r_prime) == edgeReplacements.end())
            randomGraph.addEdgeIdx(getUndirectedEdgeFromIndex(r_prime, n), true);
        else
            randomGraph.addEdgeIdx(getUndirectedEdgeFromIndex(edgeReplacements[r_prime], n), true);

        if (edgeReplacements.find(i) == edgeReplacements.end())
            edgeReplacements[r_prime] = i;
        else
            edgeReplacements[r_prime] = edgeReplacements[i];
    }

    return randomGraph;
}

// Preceding algorithms are from
// https://journals.aps.org/pre/abstract/10.1103/PhysRevE.71.036113

UndirectedGraph generateGraphWithDegreeDistributionStubMatching(const vector<size_t>& degreeDistribution) {
    size_t n = degreeDistribution.size();
    UndirectedGraph randomGraph(n);

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

        if (vertex1 != *stubIterator && !randomGraph.isEdgeIdx(vertex1, *stubIterator))  // no loops and multiedges
            randomGraph.addEdgeIdx(vertex1, *stubIterator, true);
        stubIterator++;
    }

    return randomGraph;
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
