#include "undirected.h"

#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <algorithm>


using namespace std;


namespace BaseGraph { namespace metrics {

template<typename T>
static std::list<T> intersectionOf(const std::list<T>& a, const std::list<T>& b);
template <typename T>
static double getAverage(const T& iterable);


size_t countTrianglesAroundVertexIdx(const UndirectedGraph& graph, VertexIndex vertex1){
    size_t triangleNumber = 0;
    auto vertexNeighbourhood = graph.getNeighboursOfIdx(vertex1);

    for(VertexIndex& vertex2: vertexNeighbourhood)
        triangleNumber += intersectionOf(vertexNeighbourhood, graph.getNeighboursOfIdx(vertex2)).size();

    return triangleNumber/2;  // Triangles are all counted twice
}

list<array<VertexIndex, 3>> findAllTriangles(const UndirectedGraph& graph){
    list<array<VertexIndex, 3>> triangles;

    for(VertexIndex& vertex1: graph) {
        const list<VertexIndex>& vertex1Neighbours = graph.getNeighboursOfIdx(vertex1);

        for (const VertexIndex& vertex2: vertex1Neighbours) {

            if (vertex1 < vertex2)
                for (const VertexIndex& vertex3: intersectionOf(vertex1Neighbours, graph.getNeighboursOfIdx(vertex2)))
                    if (vertex2 < vertex3)
                        triangles.push_back({vertex1, vertex2, vertex3});
        }
    }
    return triangles;
}

size_t countTriangles(const UndirectedGraph& graph){
    size_t triangleTotal = 0;
    for(VertexIndex& vertex: graph)
        triangleTotal += countTrianglesAroundVertexIdx(graph, vertex);

    return triangleTotal/3;
}

vector<double> getDegreeDistribution(const UndirectedGraph &graph) {
    vector<double> degreeDistribution(graph.getSize());

    size_t n=graph.getSize();
    for (VertexIndex& vertex: graph)
        degreeDistribution[vertex] = (double) graph.getDegreeOfIdx(vertex)/n;
    return degreeDistribution;
}

double getGlobalClusteringCoefficient(const UndirectedGraph& graph) {
    vector<size_t> vertexTriangleNumbers;
    vertexTriangleNumbers.resize(graph.getSize());

    for (VertexIndex& vertex: graph)
        vertexTriangleNumbers[vertex] = countTrianglesAroundVertexIdx(graph, vertex);
    return getGlobalClusteringCoefficient(graph, vertexTriangleNumbers);
}

double getGlobalClusteringCoefficient(const UndirectedGraph& graph, const vector<size_t>& vertexTriangleNumbers) {
    size_t vertexDegree;
    double globalTriangleNumber = 0;
    double globalWedgeNumber = 0;

    for (VertexIndex& vertex: graph) {
        vertexDegree = graph.getDegreeOfIdx(vertex);

        if(vertexDegree > 1)
            globalWedgeNumber += (double) vertexDegree*(vertexDegree - 1) / 2;
        globalTriangleNumber += vertexTriangleNumbers[vertex];
    }
    return globalTriangleNumber/globalWedgeNumber;
}

vector<double> getRedundancy(const UndirectedGraph& graph) {
    vector<double> localClusteringCoefficients = getLocalClusteringCoefficients(graph);

    for (VertexIndex vertex: graph)
        localClusteringCoefficients[vertex] *= (double) (graph.getDegreeOfIdx(vertex)-1);
    return localClusteringCoefficients;
}

vector<double> getLocalClusteringCoefficients(const UndirectedGraph& graph) {
    vector<double> localClusteringCoefficients;
    localClusteringCoefficients.resize(graph.getSize(), 0);

    int vertexDegree;
    double triangleNumber;

    for(VertexIndex& vertex: graph) {
        vertexDegree = graph.getDegreeOfIdx(vertex);
        triangleNumber = countTrianglesAroundVertexIdx(graph, vertex);

        if(vertexDegree > 1)
          localClusteringCoefficients[vertex] = 2.0*triangleNumber / vertexDegree / (vertexDegree - 1);
        else
          localClusteringCoefficients[vertex] = 0;
    }
    return localClusteringCoefficients;
}

unordered_map<size_t, double> getClusteringSpectrum(const UndirectedGraph& graph) {
    unordered_map<size_t, double> clusteringSpectrum;
    unordered_map<size_t, size_t> numberOfSummedValues;

    vector<double> localClusteringCoefficients = getLocalClusteringCoefficients(graph);

    size_t degree;

    for (VertexIndex& vertex: graph) {
        degree = graph.getDegreeOfIdx(vertex);
        if (degree < 2) continue;

        if (clusteringSpectrum.find(degree) == clusteringSpectrum.end()) {
            clusteringSpectrum[degree] = localClusteringCoefficients[vertex];
            numberOfSummedValues[degree] = 1;
        }
        else {
            clusteringSpectrum[degree] += localClusteringCoefficients[vertex];
            numberOfSummedValues[degree]++;
        }
    }

    for (auto& degree_clustering: clusteringSpectrum)
        degree_clustering.second /= numberOfSummedValues[degree_clustering.first];

    return clusteringSpectrum;
}

vector<size_t> getKShells(const UndirectedGraph& graph){
    return getKShellsAndOnionLayers(graph).first;
}

list<VertexIndex> getKCore(const UndirectedGraph& graph, size_t k){
    return getKCore(k, getKShells(graph));
}

list<VertexIndex> getKCore(size_t k, const vector<size_t>& kshells){
    list<VertexIndex> kcore;

    for (size_t i=0; i<kshells.size(); i++)
        if (kshells[i] <= k)
            kcore.push_back(i);
    return kcore;
}

vector<size_t> getOnionLayers(const UndirectedGraph& graph){
    return getKShellsAndOnionLayers(graph).second;
}

pair<vector<size_t>, vector<size_t>> getKShellsAndOnionLayers(const UndirectedGraph& graph) {
    // Algorithm of Batagelj and Zaversnik modified by Hébert-Dufresne, Grochow and Allard.
    vector<size_t> verticesOnionLayer(graph.getSize());
    vector<size_t> verticesKShell(graph.getSize());

    vector<size_t> effectiveDegrees = graph.getDegrees();

    // set<effective degree, vertex index>: Sort vertices by degree
    set<pair<size_t, VertexIndex>> higherLayers;
    list<pair<size_t, VertexIndex>> currentLayer;

    for (VertexIndex& vertex: graph)
        higherLayers.insert({effectiveDegrees[vertex], vertex});


    size_t onionLayerDegree;
    size_t onionLayer = 0;

    while(!higherLayers.empty()) {
        onionLayer += 1;
        onionLayerDegree = higherLayers.begin()->first;

        for (auto it=higherLayers.begin(); it!=higherLayers.end() && it->first==onionLayerDegree; ) {
            const auto& vertex = it->second;
            verticesKShell[vertex] = onionLayerDegree;
            verticesOnionLayer[vertex] = onionLayer;

            currentLayer.push_back(*it);
            higherLayers.erase(it++);
        }

        // Ajust layers neighbours' effective degree
        while(!currentLayer.empty()) {
            const auto& vertex = currentLayer.begin()->second;

            for (const VertexIndex& neighbour: graph.getNeighboursOfIdx(vertex)) {
                const auto& neighbourDegree = effectiveDegrees[neighbour];

                auto it = higherLayers.find({neighbourDegree, neighbour});
                if(it != higherLayers.end() && neighbourDegree > onionLayerDegree) {
                    higherLayers.erase(it);
                    higherLayers.insert( {neighbourDegree-1, neighbour} );
                    effectiveDegrees[neighbour]--;
                }
            }
            currentLayer.erase(currentLayer.begin());
        }
    }
    return {verticesKShell, verticesOnionLayer};
}

list<size_t> getNeighbourhoodDegreesOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx) {
    list<size_t> neighbourDegrees;

    for (const VertexIndex& neighbour: graph.getNeighboursOfIdx(vertexIdx))
        neighbourDegrees.push_back( graph.getDegreeOfIdx(neighbour) );

    return neighbourDegrees;
}

vector<double> getNeighbourDegreeSpectrum(const UndirectedGraph &graph, bool normalized) {
    vector<double> degreeSpectrum(graph.getSize());

    for (VertexIndex& vertex: graph)
        degreeSpectrum[vertex] = getAverage(getNeighbourhoodDegreesOfVertexIdx(graph, vertex));

    if (normalized) {
        double firstMoment = 0;
        double secondMoment = 0;
        size_t degree;
        for (VertexIndex& vertex: graph) {
            degree = graph.getDegreeOfIdx(vertex);
            firstMoment += degree;
            secondMoment += degree*degree;
        }
        for (VertexIndex& vertex: graph)
            degreeSpectrum[vertex] *= firstMoment/secondMoment;
    }
    return degreeSpectrum;
}

unordered_map<size_t, list<double>> getOnionSpectrum(const UndirectedGraph& graph) {
    auto kshells_onionLayers = getKShellsAndOnionLayers(graph);
    return getOnionSpectrum(graph, kshells_onionLayers.first, kshells_onionLayers.second);
}

unordered_map<size_t, list<double>> getOnionSpectrum(const UndirectedGraph& graph, const vector<size_t>& kshells,
        const vector<size_t>& onionLayers) {
    if (graph.getSize()!=kshells.size() || graph.getSize()!=onionLayers.size())
        throw logic_error("The onion layers vector and the k-shells vector must be the size of the graph");

    size_t onionLayersNumber = *max_element(onionLayers.begin(), onionLayers.end());
    unordered_map<size_t, list<double>> onionSpectrum;


    vector<size_t> onionLayerToKShell(onionLayersNumber + 1);
    vector<size_t> onionLayerSizes(onionLayersNumber + 1, 0);

    for(VertexIndex& vertex: graph) {
        const size_t& layer = onionLayers[vertex];
        onionLayerToKShell[layer] = kshells[vertex];
        onionLayerSizes[layer] += 1;
    }

    for(size_t layer=1; layer<=onionLayersNumber; layer++)
        onionSpectrum[onionLayerToKShell[layer]].push_back((double) onionLayerSizes[layer] / graph.getSize());

    return onionSpectrum;
}

double getDegreeCorrelation(const UndirectedGraph& graph) {
    return getDegreeCorrelation(graph, getAverage(graph.getDegrees()));
}

double getDegreeCorrelation(const UndirectedGraph& graph, double averageDegree) {
    size_t degree;
    size_t n = graph.getSize();

    vector<double> excessDegreeDistribution(1);

    double excessDegree;
    double firstMoment = 0;
    double secondMoment = 0;

    for (VertexIndex& vertex: graph) {
        degree = graph.getDegreeOfIdx(vertex);
        if (degree == 0) continue;

        if (degree>excessDegreeDistribution.size())
            excessDegreeDistribution.resize(degree, 0);

        excessDegree = degree/(averageDegree*n);
        excessDegreeDistribution[degree-1] += excessDegree;

        firstMoment += (degree-1)*excessDegree;
        secondMoment += (degree-1)*(degree-1)*excessDegree;
    }
    double excessDegreeVariance = secondMoment - firstMoment*firstMoment;

    double degreeCorrelationCoefficient = 0;
    size_t neighbourDegree;
    size_t edgeNumber = graph.getEdgeNumber();
    for (VertexIndex& vertex: graph) {
        degree = graph.getDegreeOfIdx(vertex);
        if (degree < 2) continue;

        for (const VertexIndex& neighbour: graph.getNeighboursOfIdx(vertex)) {
            if (vertex > neighbour) {
                neighbourDegree = graph.getDegreeOfIdx(neighbour);
                degreeCorrelationCoefficient += (double) (degree-1)*(neighbourDegree-1)/edgeNumber;
            }
        }
    }

    size_t maxDegree = excessDegreeDistribution.size();
    auto& edDistribution = excessDegreeDistribution;
    for (size_t degree=1; degree<maxDegree; degree++){
        degreeCorrelationCoefficient -= degree*degree * edDistribution[degree]*edDistribution[degree];

        for (size_t degree2=degree+1; degree2<maxDegree; degree2++)
            degreeCorrelationCoefficient -= 2*degree*degree2 * edDistribution[degree]*edDistribution[degree2];
    }

    return degreeCorrelationCoefficient/excessDegreeVariance;
}

double getModularity(const UndirectedGraph& graph, const vector<size_t>& vertexCommunities) {
    if (graph.getSize() == 0) throw std::logic_error("Graph is empty");
    if (vertexCommunities.size() != graph.getSize()) throw std::logic_error("Vertex communities vector must be the size of the graph");

    size_t communityNumber = *std::max_element(vertexCommunities.begin(), vertexCommunities.end());
    size_t intraCommunityStubs = 0;
    vector<size_t> communityDegreeSum(communityNumber+1, 0);

    for (VertexIndex& vertex: graph) {
        communityDegreeSum[vertexCommunities[vertex]] += graph.getDegreeOfIdx(vertex);

        for (const VertexIndex& neighbour: graph.getNeighboursOfIdx(vertex))
            if (vertexCommunities[vertex] == vertexCommunities[neighbour])
                intraCommunityStubs++;
    }
    double modularity = 0;
    size_t edgeNumber = graph.getEdgeNumber();

    modularity += (double) intraCommunityStubs/(2*edgeNumber);

    for (size_t& degreeSum: communityDegreeSum)
        modularity -= (degreeSum/(2.*edgeNumber)) * (degreeSum/(2.*edgeNumber));

    return modularity;
}

// From https://stackoverflow.com/questions/38993415/how-to-apply-the-intersection-between-two-lists-in-c
template<typename T>
static std::list<T> intersectionOf(const std::list<T>& a, const std::list<T>& b){
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

template <typename T>
static double getAverage(const T& iterable) {
    if (iterable.size() == 0)
        return 0;

    size_t sum=0;
    for (const auto& element: iterable)
        sum += element;
    return (double) sum/iterable.size();
}


}} // namespace BaseGraph::metrics
