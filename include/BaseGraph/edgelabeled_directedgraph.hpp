#ifndef BASE_GRAPH_EDGE_LABELED_DIRECTED_GRAPH_H
#define BASE_GRAPH_EDGE_LABELED_DIRECTED_GRAPH_H

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <algorithm>

#include "BaseGraph/types.h"


namespace BaseGraph{


template<typename EdgeLabel>
class EdgeLabeledDirectedGraph{
    public:
        typedef std::list<std::pair<VertexIndex, EdgeLabel>> LabeledSuccessors;
        typedef std::vector<LabeledSuccessors> LabeledAdjacencyLists;

        explicit EdgeLabeledDirectedGraph<EdgeLabel>(size_t _size=0): size(0), totalEdgeNumber(0), distinctEdgeNumber(0) {resize(_size);}

        void resize(size_t size);
        size_t getSize() const { return size; }
        size_t getDistinctEdgeNumber() const { return distinctEdgeNumber; }
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value, long long int>::type
            getTotalEdgeNumber() const { return totalEdgeNumber; }

        bool operator==(const EdgeLabeledDirectedGraph<EdgeLabel>& other) const;
        bool operator!=(const EdgeLabeledDirectedGraph<EdgeLabel>& other) const { return !(this->operator==(other)); }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            addEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force=false);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            addEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force=false);

        void addEdgeIdx(const Edge& edge, const EdgeLabel& label, bool force=false) { addEdgeIdx(edge.first, edge.second, label, force); }
        void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false) { addEdgeIdx(vertex1, vertex2, label, force); addEdgeIdx(vertex2, vertex1, label, force); }
        void addReciprocalEdgeIdx(const Edge& edge, const EdgeLabel& label, bool force=false) { addReciprocalEdgeIdx(edge, label, force); }
        bool isEdgeIdx(VertexIndex source, VertexIndex destination) const;
        bool isEdgeIdx(const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            removeEdgeIdx(VertexIndex source, VertexIndex destination);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            removeEdgeIdx(VertexIndex source, VertexIndex destination);

        void removeEdgeIdx(const Edge& edge) { removeEdgeIdx(edge.first, edge.second); }
        const EdgeLabel& getEdgeLabelOf(VertexIndex source, VertexIndex destination) const;
        const EdgeLabel& getEdgeLabelOf(const Edge& edge) const { return getEdgeLabelOf(edge.first, edge.second); }
        void setEdgeLabelIdxTo(const Edge& edge, const EdgeLabel& label) { changeEdgeLabel(edge.first, edge.second, label); }
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            setEdgeLabelTo(VertexIndex source, VertexIndex destination, const EdgeLabel& label);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            setEdgeLabelTo(VertexIndex source, VertexIndex destination, const EdgeLabel& label);

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            removeMultiedges();
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            removeMultiedges();
        void removeSelfLoops();

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            removeVertexFromEdgeListIdx(VertexIndex vertex);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            removeVertexFromEdgeListIdx(VertexIndex vertex);
        void clearEdges();

        template<typename Iterator>
        EdgeLabeledDirectedGraph<EdgeLabel> getSubgraphOfIdx(Iterator begin, Iterator end) const { return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        EdgeLabeledDirectedGraph<EdgeLabel> getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        template<typename Iterator>
        std::pair<EdgeLabeledDirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        std::pair<EdgeLabeledDirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;

        const LabeledSuccessors& getOutEdgesOfIdx(VertexIndex vertex) const { assertVertexInRange(vertex); return adjacencyList[vertex]; }
        LabeledAdjacencyLists    getInEdges() const;
        AdjacencyMatrix          getAdjacencyMatrix() const;
        size_t                   getInDegreeOfIdx(VertexIndex vertex) const;
        std::vector<size_t>      getInDegrees() const;
        size_t                   getOutDegreeOfIdx(VertexIndex vertex) const;
        std::vector<size_t>      getOutDegrees() const;

        EdgeLabeledDirectedGraph<EdgeLabel> getReversedGraph() const;

        friend std::ostream& operator <<(std::ostream& stream, const EdgeLabeledDirectedGraph<EdgeLabel>& graph) {
            stream << "Directed graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << "(" << neighbour.first << ", " << neighbour.second << ")";
                stream << "\n";
            }
            return stream;
        }

        struct iterator {
            VertexIndex position;
            iterator(VertexIndex position) : position(position) {}
            bool operator ==(iterator rhs) {return position == rhs.position;}
            bool operator!=(iterator rhs) {return position != rhs.position;}
            VertexIndex& operator*() {return position;}
            iterator operator++() {++position; return *this;}
            iterator operator++(int) {iterator tmp=iterator(position); operator++(); return tmp;}
        };

        iterator begin() const {return iterator(0);}
        iterator end() const {return iterator(size);}

    protected:
        LabeledAdjacencyLists adjacencyList;
        size_t size;
        size_t distinctEdgeNumber;
        long long int totalEdgeNumber; // Used only when EdgeLabel is integer

        typename LabeledSuccessors::iterator findNeighbour(Edge edge) { return findNeighbour(edge.first, edge.second); }
        typename LabeledSuccessors::iterator findNeighbour(VertexIndex source, VertexIndex destination) {
            assertVertexInRange(source);
            assertVertexInRange(destination);

            return std::find_if(adjacencyList[source].begin(), adjacencyList[source].end(),
                                [&destination] (const std::pair<VertexIndex, EdgeLabel>& neighbour) { return neighbour.first == destination; });
        }
        typename LabeledSuccessors::const_iterator const_findNeighbour(Edge edge) const { return const_findNeighbour(edge.first, edge.second); }
        typename LabeledSuccessors::const_iterator const_findNeighbour(VertexIndex source, VertexIndex destination) const {
            assertVertexInRange(source);
            assertVertexInRange(destination);

            return std::find_if(adjacencyList[source].begin(), adjacencyList[source].end(),
                                [&destination] (const std::pair<VertexIndex, EdgeLabel>& neighbour) { return neighbour.first == destination; });
        }

        void assertVertexInRange(VertexIndex vertex) const{
            if (vertex >= size)
                throw std::out_of_range("Vertex index (" + std::to_string(vertex) +
                        ") greater than the graph's size("+ std::to_string(size) +").");
        }
};

template<typename EdgeLabel>
bool EdgeLabeledDirectedGraph<EdgeLabel>::operator==(const EdgeLabeledDirectedGraph<EdgeLabel>& other) const{
    bool sameObject = size == other.size
                        && totalEdgeNumber == other.totalEdgeNumber
                        && distinctEdgeNumber == other.distinctEdgeNumber;

    typename LabeledSuccessors::const_iterator it;
    for (VertexIndex i=0; i<size && sameObject; ++i){
        for (it=adjacencyList[i].begin(); it != adjacencyList[i].end() && sameObject; ++it){
            if (!other.isEdgeIdx(i, it->first))
                sameObject = false;
        }

        for (it=other.adjacencyList[i].begin(); it != other.adjacencyList[i].end() && sameObject; ++it){
            if (!isEdgeIdx(i, it->first))
                sameObject = false;
        }
    }
    return sameObject;
}

template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::resize(size_t newSize){
    if (newSize < size) throw std::invalid_argument("Graph's size cannot be reduced.");
    size = newSize;
    adjacencyList.resize(newSize);
}

template<typename EdgeLabel>
typename EdgeLabeledDirectedGraph<EdgeLabel>::LabeledAdjacencyLists EdgeLabeledDirectedGraph<EdgeLabel>::getInEdges() const{
    LabeledAdjacencyLists inEdges(size);

    for (VertexIndex i=0; i<size; i++)
        for (const auto& j: getOutEdgesOfIdx(i))
            inEdges[j.first].push_back({i, j.second});
    return inEdges;
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::addEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addEdgeIdx");
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (force || !isEdgeIdx(source, destination)) {
        adjacencyList[source].push_back({destination, label});
        distinctEdgeNumber++;
        totalEdgeNumber += label;
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
            EdgeLabeledDirectedGraph<EdgeLabel>::addEdgeIdx(VertexIndex source, VertexIndex destination, const EdgeLabel& label, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addEdgeIdx");
    assertVertexInRange(source);
    assertVertexInRange(destination);

    if (force || !isEdgeIdx(source, destination)) {
        adjacencyList[source].push_back({destination, label});
        distinctEdgeNumber++;
    }
}

template<typename EdgeLabel>
bool EdgeLabeledDirectedGraph<EdgeLabel>::isEdgeIdx(VertexIndex source, VertexIndex destination) const{
    assertVertexInRange(source);
    assertVertexInRange(destination);

    for (auto edge: getOutEdgesOfIdx(source))
        if (edge.first == destination)
            return true;
    return false;
}

template<typename EdgeLabel>
const EdgeLabel& EdgeLabeledDirectedGraph<EdgeLabel>::getEdgeLabelOf(VertexIndex source, VertexIndex destination) const {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    for (auto& neighbour: adjacencyList[source])
        if (neighbour.first == destination)
            return neighbour.second;
    throw std::invalid_argument("Edge does not exist, cannot get its label");
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::setEdgeLabelTo(VertexIndex source, VertexIndex destination, const EdgeLabel& label) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    bool found = false;
    for (auto& neighbour: adjacencyList[source]) {
        if (neighbour.first == destination) {
            totalEdgeNumber += label - neighbour.second;
            neighbour.second = label;
            found = true;
            break;
        }
    }
    if (!found)
        throw std::invalid_argument("Edge does not exist, cannot change its label");
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::setEdgeLabelTo(VertexIndex source, VertexIndex destination, const EdgeLabel& label) {
    assertVertexInRange(source);
    assertVertexInRange(destination);

    bool found = false;
    for (auto& neighbour: adjacencyList[source]) {
        if (neighbour.first == destination) {
            neighbour.second = label;
            found = true;
            break;
        }
    }
    if (!found)
        throw std::invalid_argument("Edge does not exist, cannot change its label");
}


template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::removeEdgeIdx(VertexIndex source, VertexIndex destination) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call removeEdgeIdx");
    assertVertexInRange(source);
    assertVertexInRange(destination);

    auto& successors = adjacencyList[source];
    size_t sizeBefore = successors.size();

    auto it=successors.begin();
    while (it != successors.end()) {
        if (it->first == destination) {
            totalEdgeNumber -= it->second;
            successors.erase(it++);
        }
        else
            it++;
    }
    distinctEdgeNumber -= sizeBefore - successors.size();
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::removeEdgeIdx(VertexIndex source, VertexIndex destination) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call removeEdgeIdx");
    assertVertexInRange(source);
    assertVertexInRange(destination);

    auto& successors = adjacencyList[source];
    size_t sizeBefore = successors.size();

    successors.remove_if([&] (const std::pair<VertexIndex, EdgeLabel>& neighbour) {
                             return neighbour.first == destination;
                         });
    distinctEdgeNumber -= sizeBefore-successors.size();
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::removeMultiedges(){
    std::set<VertexIndex> seenVertices;
    typename LabeledSuccessors::iterator j;

    for (VertexIndex i=0; i<size; ++i){
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (seenVertices.find(j->first) == seenVertices.end()) {
                seenVertices.insert(j->first);
                j++;
            }
            else {
                totalEdgeNumber -= j->second;
                adjacencyList[i].erase(j++);
                distinctEdgeNumber--;
            }
        }
        seenVertices.clear();
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::removeMultiedges(){
    std::set<VertexIndex> seenVertices;
    typename LabeledSuccessors::iterator j;

    for (VertexIndex i=0; i<size; ++i){
        j = adjacencyList[i].begin();

        while(j != adjacencyList[i].end()){
            if (seenVertices.find(j->first) == seenVertices.end()) {
                seenVertices.insert(j->first);
                j++;
            }
            else {
                adjacencyList[i].erase(j++);
                distinctEdgeNumber--;
            }
        }
        seenVertices.clear();
    }
}

template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::removeSelfLoops() {
    for (VertexIndex& i: *this)
        removeEdgeIdx(i, i);
}


template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::removeVertexFromEdgeListIdx(VertexIndex vertex){
    assertVertexInRange(vertex);

    auto& successors = adjacencyList[vertex];
    auto it = successors.begin();
    while (it != successors.end()) {
        totalEdgeNumber -= it->second;
        successors.erase(it++);
        distinctEdgeNumber--;
    }

    for (VertexIndex i=0; i<size; ++i)
        removeEdgeIdx(i, vertex);
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledDirectedGraph<EdgeLabel>::removeVertexFromEdgeListIdx(VertexIndex vertex){
    assertVertexInRange(vertex);
    size_t sizeBefore;

    for (VertexIndex i=0; i<size; ++i){
        if (i == vertex) {
            sizeBefore = adjacencyList[i].size();
            adjacencyList[i].clear();
            distinctEdgeNumber -= sizeBefore - adjacencyList[i].size();
        }
        else
            removeEdgeIdx(i, vertex);
    }
}

template<typename EdgeLabel>
void EdgeLabeledDirectedGraph<EdgeLabel>::clearEdges() {
    distinctEdgeNumber = 0;
    totalEdgeNumber = 0;

    for (VertexIndex i: *this)
        adjacencyList[i].clear();
}

template<typename EdgeLabel>
EdgeLabeledDirectedGraph<EdgeLabel> EdgeLabeledDirectedGraph<EdgeLabel>::getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const{
    EdgeLabeledDirectedGraph<EdgeLabel> subgraph(size);

    for (VertexIndex i: vertices) {
        assertVertexInRange(i);

        for (auto neighbour: getOutEdgesOfIdx(i))
            if (vertices.find(neighbour.first) != vertices.end())
                subgraph.addEdgeIdx(i, neighbour.first, neighbour.second, true);
    }

    return subgraph;
}

template<typename EdgeLabel>
std::pair<EdgeLabeledDirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> EdgeLabeledDirectedGraph<EdgeLabel>::getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const{
    EdgeLabeledDirectedGraph<EdgeLabel> subgraph(vertices.size());

    std::unordered_map<VertexIndex, VertexIndex> newMapping;

    VertexIndex position=0;
    for (VertexIndex vertex: vertices) {
        newMapping[vertex] = position;
        position++;
    }

    for (VertexIndex i: vertices) {
        assertVertexInRange(i);

        for (auto& neighbour: getOutEdgesOfIdx(i))
            if (vertices.find(neighbour.first) != vertices.end())
                subgraph.addEdgeIdx(newMapping[i], newMapping[neighbour.first], neighbour.second, true);
    }

    return {subgraph, newMapping};
}

template<typename EdgeLabel>
AdjacencyMatrix EdgeLabeledDirectedGraph<EdgeLabel>::getAdjacencyMatrix() const{
    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(size, std::vector<size_t>(size));

    for (VertexIndex i=0; i<size; ++i)
        for (auto& neighbour: getOutEdgesOfIdx(i))
            adjacencyMatrix[i][neighbour.first] += 1;

    return adjacencyMatrix;
}

template<typename EdgeLabel>
size_t EdgeLabeledDirectedGraph<EdgeLabel>::getInDegreeOfIdx(VertexIndex vertex) const{
    assertVertexInRange(vertex);
    size_t inDegree = 0;

    for (VertexIndex i=0; i<size; ++i)
        for (auto& neighbour: getOutEdgesOfIdx(i))
            if (neighbour.first == vertex)
                inDegree++;
    return inDegree;
}

template<typename EdgeLabel>
std::vector<size_t> EdgeLabeledDirectedGraph<EdgeLabel>::getInDegrees() const {
    std::vector<size_t> inDegrees(size, 0);

    for (VertexIndex i=0; i<size; i++){
        for (auto& neighbour: getOutEdgesOfIdx(i))
            inDegrees[neighbour.first]++;
    }

    return inDegrees;
}

template<typename EdgeLabel>
size_t EdgeLabeledDirectedGraph<EdgeLabel>::getOutDegreeOfIdx(VertexIndex vertex) const{
    assertVertexInRange(vertex);
    return adjacencyList[vertex].size();
}

template<typename EdgeLabel>
std::vector<size_t> EdgeLabeledDirectedGraph<EdgeLabel>::getOutDegrees() const {
    std::vector<size_t> outDegrees(size, 0);

    for (VertexIndex i=0; i<size; i++)
        outDegrees[i] += getOutDegreeOfIdx(i);
    return outDegrees;
}

template<typename EdgeLabel>
EdgeLabeledDirectedGraph<EdgeLabel> EdgeLabeledDirectedGraph<EdgeLabel>::getReversedGraph() const {
    EdgeLabeledDirectedGraph<EdgeLabel> reversedGraph(size);

    for (VertexIndex i: *this)
        for (auto& neighbour: getOutEdgesOfIdx(i))
            reversedGraph.addEdgeIdx(neighbour.first, i, neighbour.second);

    return reversedGraph;
}

} // namespace BaseGraph

#endif
