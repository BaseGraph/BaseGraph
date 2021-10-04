#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "BaseGraph/algorithms/layeredconfigurationmodel.h"
#include "BaseGraph/algorithms/randomgraphs.h"
#include "BaseGraph/metrics/undirected.h"

using namespace std;


namespace BaseGraph{

extern std::mt19937_64 rng;



template<typename T>
void printvector(const vector<T>& v) {

    for (auto x: v)
        cout << x << ",";
    cout << endl;

}


struct NodeProperties {

    VertexIndex nodeIdx;
    size_t core, layer, layerOfCurrentNeighbour, layerOfNewNeighbour, numberOfBlackAndRedNeighbours, numberOfRedNeighbours;
    bool isInFirstLayerOfCore;

    NodeProperties(VertexIndex nodeIdx, size_t core, size_t layer, size_t layerOfCurrentNeighbour, size_t layerOfNewNeighbour, size_t numberOfBlackAndRedNeighbours, size_t numberOfRedNeighbours, bool isInFirstLayerOfCore):
        nodeIdx(nodeIdx), core(core), layer(layer),
        layerOfCurrentNeighbour(layerOfCurrentNeighbour), layerOfNewNeighbour(layerOfNewNeighbour),
        numberOfBlackAndRedNeighbours(numberOfBlackAndRedNeighbours), numberOfRedNeighbours(numberOfRedNeighbours),
        isInFirstLayerOfCore(isInFirstLayerOfCore) {}

    NodeProperties() {}

    friend ostream& operator <<(std::ostream& stream, const NodeProperties node) {
        stream << node.nodeIdx << "," << node.core << "," << node.layer << "," << node.layerOfCurrentNeighbour << "," << node.layerOfNewNeighbour << "," << node.numberOfBlackAndRedNeighbours << "," << node.numberOfRedNeighbours << "," << node.isInFirstLayerOfCore << endl;
        return stream;
    }

};


static pair<vector<size_t>, vector<size_t>> getNumberOfBlackAndRedNeighbours(const UndirectedGraph& graph, const pair<vector<size_t>, vector<size_t>>& kShellsAndOnionLayers);
static vector<bool> getNodeIdxIsInFirstLayerOfCore(const pair<vector<size_t>, vector<size_t>>& kShellsAndOnionLayers);
static void updateNumberOfBlackAndRedNeighbours(pair<vector<size_t>, vector<size_t>>& numberOfBlackAndRedNeighbours, NodeProperties node);
static bool onionDecompositionIsChanged(NodeProperties node);


void shuffleGraphWithLayeredConfigurationModel(UndirectedGraph &graph, size_t swaps) {
    auto edgeVector = getEdgeVectorOfGraph(graph);
    shuffleGraphWithLayeredConfigurationModel(graph, edgeVector, swaps);
}


void shuffleGraphWithLayeredConfigurationModel(UndirectedGraph &graph, vector<pair<VertexIndex, VertexIndex>>& edgeVector, size_t swaps) {
    if (swaps == 0) swaps = 2*graph.getEdgeNumber();

    size_t edgeNumber = edgeVector.size();
    std::uniform_real_distribution<double> uniform01Distribution(0, 1);

    size_t edge1Idx, edge2Idx;
    Edge newEdge1, newEdge2;

    auto kShellsAndOnionLayers = getKShellsAndOnionLayers(graph);
    auto nodeIdxIsInFirstLayerOfCore = getNodeIdxIsInFirstLayerOfCore(kShellsAndOnionLayers);
    auto numberOfBlackAndRedNeighbours = getNumberOfBlackAndRedNeighbours(graph, kShellsAndOnionLayers);

    NodeProperties nodeA, nodeB, nodeC, nodeD;

    for (size_t i=0; i<swaps; i++) {
        edge1Idx = edgeNumber*uniform01Distribution(rng);

        edge2Idx = (edgeNumber-1)*uniform01Distribution(rng);
        if (edge2Idx >= edge1Idx) edge2Idx++;

        const auto& currentEdge1 = edgeVector[edge1Idx];
        const auto& currentEdge2 = edgeVector[edge2Idx];

        if (uniform01Distribution(rng) < 0.5) {
            newEdge1 = {currentEdge1.first, currentEdge2.first};
            newEdge2 = {currentEdge1.second, currentEdge2.second};

            nodeA = NodeProperties(currentEdge1.first, kShellsAndOnionLayers.first[currentEdge1.first], kShellsAndOnionLayers.second[currentEdge1.first], kShellsAndOnionLayers.second[currentEdge1.second], kShellsAndOnionLayers.second[currentEdge2.first], numberOfBlackAndRedNeighbours.first[currentEdge1.first], numberOfBlackAndRedNeighbours.second[currentEdge1.first], nodeIdxIsInFirstLayerOfCore[currentEdge1.first]);
            nodeB = NodeProperties(currentEdge1.second, kShellsAndOnionLayers.first[currentEdge1.second], kShellsAndOnionLayers.second[currentEdge1.second], kShellsAndOnionLayers.second[currentEdge1.first], kShellsAndOnionLayers.second[currentEdge2.second], numberOfBlackAndRedNeighbours.first[currentEdge1.second], numberOfBlackAndRedNeighbours.second[currentEdge1.second], nodeIdxIsInFirstLayerOfCore[currentEdge1.second]);
            nodeC = NodeProperties(currentEdge2.first, kShellsAndOnionLayers.first[currentEdge2.first], kShellsAndOnionLayers.second[currentEdge2.first], kShellsAndOnionLayers.second[currentEdge2.second], kShellsAndOnionLayers.second[currentEdge1.first], numberOfBlackAndRedNeighbours.first[currentEdge2.first], numberOfBlackAndRedNeighbours.second[currentEdge2.first], nodeIdxIsInFirstLayerOfCore[currentEdge2.first]);
            nodeD = NodeProperties(currentEdge2.second, kShellsAndOnionLayers.first[currentEdge2.second], kShellsAndOnionLayers.second[currentEdge2.second], kShellsAndOnionLayers.second[currentEdge2.first], kShellsAndOnionLayers.second[currentEdge1.second], numberOfBlackAndRedNeighbours.first[currentEdge2.second], numberOfBlackAndRedNeighbours.second[currentEdge2.second], nodeIdxIsInFirstLayerOfCore[currentEdge2.second]);

        }
        else {
            newEdge1 = {currentEdge1.first, currentEdge2.second};
            newEdge2 = {currentEdge1.second, currentEdge2.first};

            nodeA = NodeProperties(currentEdge1.first, kShellsAndOnionLayers.first[currentEdge1.first], kShellsAndOnionLayers.second[currentEdge1.first], kShellsAndOnionLayers.second[currentEdge1.second], kShellsAndOnionLayers.second[currentEdge2.second], numberOfBlackAndRedNeighbours.first[currentEdge1.first], numberOfBlackAndRedNeighbours.second[currentEdge1.first], nodeIdxIsInFirstLayerOfCore[currentEdge1.first]);
            nodeB = NodeProperties(currentEdge1.second, kShellsAndOnionLayers.first[currentEdge1.second], kShellsAndOnionLayers.second[currentEdge1.second], kShellsAndOnionLayers.second[currentEdge1.first], kShellsAndOnionLayers.second[currentEdge2.first], numberOfBlackAndRedNeighbours.first[currentEdge1.second], numberOfBlackAndRedNeighbours.second[currentEdge1.second], nodeIdxIsInFirstLayerOfCore[currentEdge1.second]);
            nodeC = NodeProperties(currentEdge2.first, kShellsAndOnionLayers.first[currentEdge2.first], kShellsAndOnionLayers.second[currentEdge2.first], kShellsAndOnionLayers.second[currentEdge2.second], kShellsAndOnionLayers.second[currentEdge1.second], numberOfBlackAndRedNeighbours.first[currentEdge2.first], numberOfBlackAndRedNeighbours.second[currentEdge2.first], nodeIdxIsInFirstLayerOfCore[currentEdge2.first]);
            nodeD = NodeProperties(currentEdge2.second, kShellsAndOnionLayers.first[currentEdge2.second], kShellsAndOnionLayers.second[currentEdge2.second], kShellsAndOnionLayers.second[currentEdge2.first], kShellsAndOnionLayers.second[currentEdge1.first], numberOfBlackAndRedNeighbours.first[currentEdge2.second], numberOfBlackAndRedNeighbours.second[currentEdge2.second], nodeIdxIsInFirstLayerOfCore[currentEdge2.second]);

        }

        if (newEdge1.first==newEdge1.second || newEdge2.first==newEdge2.second)
            continue;

        if (graph.isEdgeIdx(newEdge1) || graph.isEdgeIdx(newEdge2))
            continue;

        if (onionDecompositionIsChanged(nodeA))
            continue;

        if (onionDecompositionIsChanged(nodeB))
            continue;

        if (onionDecompositionIsChanged(nodeC))
            continue;
        
        if (onionDecompositionIsChanged(nodeD))
            continue;

        updateNumberOfBlackAndRedNeighbours(numberOfBlackAndRedNeighbours, nodeA);
        updateNumberOfBlackAndRedNeighbours(numberOfBlackAndRedNeighbours, nodeB);
        updateNumberOfBlackAndRedNeighbours(numberOfBlackAndRedNeighbours, nodeC);
        updateNumberOfBlackAndRedNeighbours(numberOfBlackAndRedNeighbours, nodeD);

        graph.removeEdgeIdx(currentEdge1);
        graph.removeEdgeIdx(currentEdge2);
        graph.addEdgeIdx(newEdge1, true);
        graph.addEdgeIdx(newEdge2, true);

        edgeVector[edge1Idx] = newEdge1;
        edgeVector[edge2Idx] = newEdge2;
    }
}


static void updateNumberOfBlackAndRedNeighbours(pair<vector<size_t>, vector<size_t>>& numberOfBlackAndRedNeighbours, NodeProperties node) {

    if (node.layerOfCurrentNeighbour>=node.layer) {

        if (node.layerOfNewNeighbour==node.layer-1)
            numberOfBlackAndRedNeighbours.second[node.nodeIdx]--;
        else if (node.layerOfNewNeighbour<node.layer-1) {
            numberOfBlackAndRedNeighbours.first[node.nodeIdx]--;
            numberOfBlackAndRedNeighbours.second[node.nodeIdx]--;
        }

    }
    else if (node.layerOfCurrentNeighbour==node.layer-1) {

        if (node.layerOfNewNeighbour>=node.layer)
            numberOfBlackAndRedNeighbours.second[node.nodeIdx]++;
        else if (node.layerOfNewNeighbour<node.layer-1)
            numberOfBlackAndRedNeighbours.first[node.nodeIdx]--;

    }
    else {

        if (node.layerOfNewNeighbour>=node.layer) {
            numberOfBlackAndRedNeighbours.first[node.nodeIdx]++;
            numberOfBlackAndRedNeighbours.second[node.nodeIdx]++;
        }
        else if (node.layerOfNewNeighbour==node.layer-1)
            numberOfBlackAndRedNeighbours.first[node.nodeIdx]++;

    }

}


static bool onionDecompositionIsChanged(NodeProperties node) {
    bool isOnionDecompositionChanged;

    if (node.isInFirstLayerOfCore) {

        if (node.layerOfCurrentNeighbour>=node.layer) {

            if (node.layerOfNewNeighbour>=node.layer)
                isOnionDecompositionChanged = false;
            else
                isOnionDecompositionChanged = true;

        }
        else if (node.layerOfCurrentNeighbour<node.layer) {

            if (node.layerOfNewNeighbour>=node.layer)
                isOnionDecompositionChanged = false;
            else
                isOnionDecompositionChanged = true;

        }

    }
    else {

        if (node.layerOfCurrentNeighbour>=node.layer) {

            if (node.numberOfBlackAndRedNeighbours==node.core+1) {

                if (node.layerOfNewNeighbour>=node.layer-1)
                    isOnionDecompositionChanged = false;
                else
                    isOnionDecompositionChanged = true;

            }
            else
                isOnionDecompositionChanged = false;

        }
        else if (node.layerOfCurrentNeighbour==node.layer-1) {

            if (node.numberOfBlackAndRedNeighbours==node.core+1) {

                if (node.layerOfNewNeighbour>=node.layer-1)
                    isOnionDecompositionChanged = false;
                else
                    isOnionDecompositionChanged = true;

            }
            else if (node.numberOfRedNeighbours==node.core) {

                if (node.layerOfNewNeighbour<node.layer)
                    isOnionDecompositionChanged = false;
                else
                    isOnionDecompositionChanged = true;

            }
            else
                isOnionDecompositionChanged = false;

        }
        else {

            if (node.numberOfRedNeighbours==node.core) {

                if (node.layerOfNewNeighbour<node.layer)
                    isOnionDecompositionChanged = false;
                else
                    isOnionDecompositionChanged = true;

            }
            else
                isOnionDecompositionChanged = false;

        }

    }

    return isOnionDecompositionChanged;

}


static pair<vector<size_t>, vector<size_t>> getNumberOfBlackAndRedNeighbours(const UndirectedGraph& graph, const pair<vector<size_t>, vector<size_t>>& kShellsAndOnionLayers) {

    vector<size_t> numberOfRedNeighbours(kShellsAndOnionLayers.first.size()), numberOfBlackAndRedNeighbours(kShellsAndOnionLayers.first.size());

    for (VertexIndex nodeIdx=0; nodeIdx<kShellsAndOnionLayers.first.size(); nodeIdx++) {

        for (VertexIndex neighbourIdx: graph.getNeighboursOfIdx(nodeIdx)) {

            if (kShellsAndOnionLayers.second[neighbourIdx]>=kShellsAndOnionLayers.second[nodeIdx])
                numberOfRedNeighbours[nodeIdx]++;
            if (kShellsAndOnionLayers.second[neighbourIdx]>=kShellsAndOnionLayers.second[nodeIdx]-1)
                numberOfBlackAndRedNeighbours[nodeIdx]++;

        }

    }

    return {numberOfBlackAndRedNeighbours, numberOfRedNeighbours};

}


static vector<bool> getNodeIdxIsInFirstLayerOfCore(const pair<vector<size_t>, vector<size_t>>& kShellsAndOnionLayers) {
    vector<bool> nodeIdxIsInFirstLayerOfCore(kShellsAndOnionLayers.first.size());
    unordered_map<size_t, size_t> firstLayerOfCoreMap;

    for (size_t i=0; i<kShellsAndOnionLayers.first.size(); i++) {
        if (firstLayerOfCoreMap.find(kShellsAndOnionLayers.first[i])==firstLayerOfCoreMap.end())
            firstLayerOfCoreMap[kShellsAndOnionLayers.first[i]] = kShellsAndOnionLayers.second[i];
        else {
            if (firstLayerOfCoreMap[kShellsAndOnionLayers.first[i]]>kShellsAndOnionLayers.second[i])
                firstLayerOfCoreMap[kShellsAndOnionLayers.first[i]] = kShellsAndOnionLayers.second[i];
        }
    }

    for (size_t i=0; i<kShellsAndOnionLayers.first.size(); i++) {
        if (firstLayerOfCoreMap[kShellsAndOnionLayers.first[i]]==kShellsAndOnionLayers.second[i])
            nodeIdxIsInFirstLayerOfCore[i] = 1;
        else
            nodeIdxIsInFirstLayerOfCore[i] = 0;
    }

    return nodeIdxIsInFirstLayerOfCore;

}

} // namespace BaseGraph
