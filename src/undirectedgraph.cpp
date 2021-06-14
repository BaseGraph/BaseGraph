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
    for (size_t i: *this)
        degrees[i] = adjacencyList[i].size();
    return degrees;
}

void UndirectedGraph::addEdgeIdx(size_t vertex1, size_t vertex2, bool force){
    if (vertex1 >= size || vertex2 >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    if (force || !isEdgeIdx(vertex1, vertex2)) {
        adjacencyList[vertex1].push_back(vertex2);
        adjacencyList[vertex2].push_back(vertex1);
        edgeNumber++;
    }
}

bool UndirectedGraph::isEdgeIdx(size_t vertex1, size_t vertex2) const {
    if (vertex1 >= size || vertex2 >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    if (adjacencyList[vertex1].size() < adjacencyList[vertex2].size())
        return DirectedGraph::isEdgeIdx(vertex1, vertex2);
    else
        return DirectedGraph::isEdgeIdx(vertex2, vertex1);
}

void UndirectedGraph::removeEdgeIdx(size_t vertex1, size_t vertex2){
    if (vertex1 >= size || vertex2 >= size) throw invalid_argument("Vertex index greater than the graph's size.");
    size_t sizeBefore = adjacencyList[vertex1].size();

    adjacencyList[vertex1].remove(vertex2);
    size_t sizeDifference = sizeBefore - adjacencyList[vertex1].size();
    
    if (sizeDifference > 0) {
        adjacencyList[vertex2].remove(vertex1);
        edgeNumber -= sizeDifference;
    }
}

void UndirectedGraph::removeVertexFromEdgeListIdx(size_t vertex) {
    if (vertex >= size) throw invalid_argument("Vertex index greater than the graph's size.");

    for (const size_t& neighbour: getNeighboursOfIdx(vertex))
        DirectedGraph::removeEdgeIdx(neighbour, vertex);  // Also takes care of edgeNumber update

    adjacencyList[vertex].clear();
}

void UndirectedGraph::removeMultiedges() {
    list<size_t> seenVertices;
    list<size_t>::iterator j;
    for (size_t i: *this) {
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (find(seenVertices.begin(), seenVertices.end(), *j) != seenVertices.end()) {
                 if (i < *j) {
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
