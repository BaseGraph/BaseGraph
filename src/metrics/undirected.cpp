#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <utility>
#include <algorithm>

#include "pgl/metrics/undirected.h"


using namespace std;

namespace PGL{

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

size_t countTrianglesAroundVertexIdx(const UndirectedGraph& graph, VertexIndex vertex1){
    size_t triangleNumber = 0;
    auto vertexNeighbourhood = graph.getNeighboursOfIdx(vertex1);

    for(VertexIndex& vertex2: vertexNeighbourhood)
        triangleNumber += intersection_of(vertexNeighbourhood, graph.getNeighboursOfIdx(vertex2)).size();

    return triangleNumber/2;  // Triangles are all counted twice
}

list<array<VertexIndex, 3>> findAllTriangles(const UndirectedGraph& graph){
    list<array<VertexIndex, 3>> triangles;

    for(VertexIndex& vertex1: graph) {
        const list<VertexIndex>& vertex1Neighbours = graph.getNeighboursOfIdx(vertex1);

        for (const VertexIndex& vertex2: vertex1Neighbours) {

            if (vertex1 < vertex2)
                for (const VertexIndex& vertex3: intersection_of(vertex1Neighbours, graph.getNeighboursOfIdx(vertex2)))
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
        degreeDistribution[vertex] = (double) graph.getDegreeIdx(vertex)/n;
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
        vertexDegree = graph.getDegreeIdx(vertex);

        if(vertexDegree > 1)
            globalWedgeNumber += (double) vertexDegree*(vertexDegree - 1) / 2;
        globalTriangleNumber += vertexTriangleNumbers[vertex];
    }
    return globalTriangleNumber/globalWedgeNumber;
}

vector<double> getRedundancy(const UndirectedGraph& graph) {
    vector<double> localClusteringCoefficients = getLocalClusteringCoefficients(graph);

    for (VertexIndex vertex: graph)
        localClusteringCoefficients[vertex] *= (double) (graph.getDegreeIdx(vertex)-1);
    return localClusteringCoefficients;
}

vector<double> getLocalClusteringCoefficients(const UndirectedGraph& graph) {
    vector<double> localClusteringCoefficients;
    localClusteringCoefficients.resize(graph.getSize(), 0);

    int vertexDegree;
    double triangleNumber;

    for(VertexIndex& vertex: graph) {
        vertexDegree = graph.getDegreeIdx(vertex);
        triangleNumber = countTrianglesAroundVertexIdx(graph, vertex);

        if(vertexDegree > 1)
          localClusteringCoefficients[vertex] = 2.0*triangleNumber / vertexDegree / (vertexDegree - 1);
        else
          localClusteringCoefficients[vertex] = 0;
    }
    return localClusteringCoefficients;
}

vector<double> getClusteringSpectrum(const UndirectedGraph& graph) {
    vector<double> clusteringSpectrum(1, 0);
    vector<size_t> numberOfSummedValues(1, 0);

    vector<double> localClusteringCoefficients = getLocalClusteringCoefficients(graph);

    size_t degree;

    for (VertexIndex& vertex: graph) {
        degree = graph.getDegreeIdx(vertex);
        if (degree+1 > clusteringSpectrum.size()) {
            clusteringSpectrum.resize(degree+1, 0);
            numberOfSummedValues.resize(degree+1, 0);
        }

        numberOfSummedValues[degree]++;
        clusteringSpectrum[degree] += localClusteringCoefficients[vertex];
    }

    for (size_t i=0; i<clusteringSpectrum.size(); i++)
        if (numberOfSummedValues[i] > 0)
            clusteringSpectrum[i] /= numberOfSummedValues[i];

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
    vector<size_t> onionLayer(graph.getSize());
    vector<size_t> coreness(graph.getSize());

    VertexIndex vertex;
    size_t vertexDegree, neighbourDegree;
    size_t layer = 0;
    set< pair<size_t, VertexIndex> > layerSet;
    set< pair<size_t, VertexIndex> >::iterator it;

    vector<size_t> degrees = graph.getDegrees();
    set<pair<size_t, VertexIndex>> degreeSet;

    for (VertexIndex& vertex: graph)
        degreeSet.insert({degrees[vertex], vertex});


    while(degreeSet.size() > 0) {
        layer += 1;
        vertexDegree = degreeSet.begin()->first;

        for (it=degreeSet.begin(); it!=degreeSet.end() && it->first==vertexDegree; it++) {
            vertex = it->second;
            coreness[vertex] = vertexDegree;
            onionLayer[vertex] = layer;
        }
        // Adds the vertices of the layer to the set.
        layerSet.insert(degreeSet.begin(), it);
        // Removes the vertices of the current layer.
        degreeSet.erase(degreeSet.begin(), it);

        // Modifies the "effective" degree of the neighbors of the vertices in the layer.
        while(layerSet.size() > 0) {
            // Next vertex of the layer.
            vertex = layerSet.begin()->second;
            // Reduces the "effective" degree of its neighbours.
            for (const VertexIndex& neighbour: graph.getNeighboursOfIdx(vertex)) {
                neighbourDegree = degrees[neighbour];
                // Finds the neighbor in the set of "effective" degrees.
                it = degreeSet.find(make_pair(neighbourDegree, neighbour));

                if(it != degreeSet.end() && neighbourDegree > vertexDegree) {
                    degrees[neighbour]--;
                    degreeSet.erase(it);
                    degreeSet.insert( {neighbourDegree - 1, neighbour} );
                }
            }
            // Removes the vertices from the layerSet.
            layerSet.erase(layerSet.begin());
        }
    }
    return {coreness, onionLayer};
}

list<size_t> getNeighbourhoodDegreesOfVertexIdx(const UndirectedGraph& graph, VertexIndex vertexIdx) {
    list<size_t> neighbourDegrees;

    for (const VertexIndex& neighbour: graph.getNeighboursOfIdx(vertexIdx))
        neighbourDegrees.push_back( graph.getDegreeIdx(neighbour) );

    return neighbourDegrees;
}

template <typename T>
static double getAverage(const T& iterable) {
    if (iterable.size() == 0)
        return 0;

    size_t sum=0;
    for (const size_t& element: iterable)
        sum += element;
    return (double) sum/iterable.size();
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
            degree = graph.getDegreeIdx(vertex);
            firstMoment += degree;
            secondMoment += degree*degree;
        }
        for (VertexIndex& vertex: graph)
            degreeSpectrum[vertex] *= firstMoment/secondMoment;
    }
    return degreeSpectrum;
}

vector<list<double>> getOnionSpectrum(const UndirectedGraph& graph) {
    auto kshells_onionLayers = getKShellsAndOnionLayers(graph);
    return getOnionSpectrum(graph, kshells_onionLayers.first, kshells_onionLayers.second);
}

vector<list<double>> getOnionSpectrum(const UndirectedGraph& graph, const vector<size_t>& kshells,
        const vector<size_t>& onionLayers) {
    if (graph.getSize()!=kshells.size() || graph.getSize()!=onionLayers.size())
        throw logic_error("The onion layers vector and the k-shells vector must be the size of the graph");

    size_t onionLayersNumber = *max_element(onionLayers.begin(), onionLayers.end());
    size_t kshellsNumber = *max_element(kshells.begin(), kshells.end());

    vector<list<double>> onionSpectrum(kshellsNumber+1);


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
        degree = graph.getDegreeIdx(vertex);
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
        degree = graph.getDegreeIdx(vertex);
        if (degree < 2) continue;

        for (const VertexIndex& neighbour: graph.getNeighboursOfIdx(vertex)) {
            if (vertex > neighbour) {
                neighbourDegree = graph.getDegreeIdx(neighbour);
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
        communityDegreeSum[vertexCommunities[vertex]] += graph.getDegreeIdx(vertex);

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

} // namespace PGL
