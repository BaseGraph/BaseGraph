#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>

#include "pgl/undirectedgraph.h"


using namespace std;

namespace PGL{


bool UndirectedGraph::operator==(const UndirectedGraph& other) const{
    bool sameObject = size == other.size && edgeNumber == other.edgeNumber;

    list<size_t>::const_iterator it;
    for (size_t i=0; i<size && sameObject; ++i){
        for (it=adjacencyList[i].begin(); it != adjacencyList[i].end() && sameObject; ++it){
            if (!other.isEdgeIdx(i, *it))
                sameObject = false;
        }

        for (it=other.adjacencyList[i].begin(); it != other.adjacencyList[i].end() && sameObject; ++it){
            if (!isEdgeIdx(i, *it))
                sameObject = false;
        }
    }
    return sameObject;
}

vector<size_t> UndirectedGraph::getDegrees() const{
    vector<size_t> degrees(size);
    for (size_t i=0; i<size; i++)
        degrees[i] = adjacencyList[i].size();
    return degrees;
}

void UndirectedGraph::addEdgeIdx(size_t source, size_t destination, bool force){
    if (source >= size || destination >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    if (force || !isEdgeIdx(source, destination)) {
        adjacencyList[source].push_back(destination);
        adjacencyList[destination].push_back(source);
        edgeNumber++;
    }
}

void UndirectedGraph::removeEdgeIdx(size_t source, size_t destination){
    if (source >= size || destination >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    size_t sizeBefore = adjacencyList[source].size();

    adjacencyList[source].remove(destination);
    adjacencyList[destination].remove(source);

    edgeNumber -= sizeBefore - adjacencyList[source].size();
}

void UndirectedGraph::removeVertexFromEdgeListIdx(size_t vertex) {
    for (const size_t& neighbour: getNeighboursOfIdx(vertex))
        DirectedGraph::removeEdgeIdx(neighbour, vertex);

    adjacencyList[vertex].clear();
}

void UndirectedGraph::removeMultiedges() {
    list<size_t> seenVertices;
    list<size_t>::iterator j;
    for (size_t i=0; i<size; ++i){
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (find(seenVertices.begin(), seenVertices.end(), *j) != seenVertices.end()) {
                 if (i >= *j) {
                    edgeNumber--;
                 }
                adjacencyList[i].erase(j++);
            }
            else {
                seenVertices.push_back(*j);
                j++;
            }
        }
        seenVertices.clear();
    }
}


} // namespace PGL
