#ifndef BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H
#define BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <algorithm>

#include "BaseGraph/types.h"
#include "BaseGraph/edgelabeled_directedgraph.hpp"


namespace BaseGraph{


template<typename EdgeLabel>
class EdgeLabeledUndirectedGraph: protected EdgeLabeledDirectedGraph<EdgeLabel>{
    typedef EdgeLabeledDirectedGraph<EdgeLabel> BaseClass;

    size_t& size = BaseClass::size;
    size_t& distinctEdgeNumber = BaseClass::distinctEdgeNumber;
    long long int& totalEdgeNumber = BaseClass::totalEdgeNumber;
    typename BaseClass::LabeledAdjacencyLists& adjacencyList = BaseClass::adjacencyList;

    public:
        typedef std::list<std::pair<VertexIndex, EdgeLabel>> LabeledSuccessors;
        typedef std::vector<LabeledSuccessors> LabeledAdjacencyLists;

        explicit EdgeLabeledUndirectedGraph<EdgeLabel>(size_t _size=0): BaseClass(_size) {}

        void resize(size_t size);
        size_t getSize() const { return BaseClass::getSize(); }
        size_t getDistinctEdgeNumber() const { return BaseClass::getDistinctEdgeNumber(); }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value, long long int>::type
            getTotalEdgeNumber() const { return BaseClass::getTotalEdgeNumber(); }

        bool operator==(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const;
        bool operator!=(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const { return !(this->operator==(other)); }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false);

        void addEdgeIdx(const Edge& edge, const EdgeLabel& label, bool force=false) { addEdgeIdx(edge.first, edge.second, label, force); }
        bool isEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) const;
        bool isEdgeIdx(const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);

        void removeEdgeIdx(const Edge& edge) { removeEdgeIdx(edge.first, edge.second); }
        const EdgeLabel& getEdgeLabelOf(VertexIndex vertex1, VertexIndex vertex2) { return getDegreeIdx(vertex1) < getDegreeIdx(vertex2) ? BaseClass::getEdgeLabelOf(vertex1, vertex2) : BaseClass::getEdgeLabelOf(vertex2, vertex1); }
        const EdgeLabel& getEdgeLabelOf(const Edge& edge) { getEdgeLabelOf(edge.first, edge.second); }
        void changeEdgeLabelTo(const Edge& edge, const EdgeLabel& label) { changeEdgeLabel(edge.first, edge.second, label); }
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            changeEdgeLabelTo(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            changeEdgeLabelTo(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label) { BaseClass::changeEdgeLabelTo(vertex1, vertex2, label); BaseClass::changeEdgeLabelTo(vertex2, vertex1, label); }

        void removeMultiedges() { BaseClass::removeMultiedges(); }
        void removeSelfLoops() { BaseClass::removeSelfLoops(); }
        void removeVertexFromEdgeListIdx(VertexIndex vertex) { BaseClass::removeVertexFromEdgeListIdx(vertex); }
        void clearEdges() { BaseClass::clearEdges(); }

        template<typename Iterator>
        EdgeLabeledUndirectedGraph<EdgeLabel> getSubgraph(Iterator begin, Iterator end) const { return getSubgraph(std::unordered_set<VertexIndex>(begin, end)); };
        EdgeLabeledUndirectedGraph<EdgeLabel> getSubgraph(const std::unordered_set<VertexIndex>& vertices) const;
        template<typename Iterator>
        std::pair<EdgeLabeledUndirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemap(Iterator begin, Iterator end) const {
            return getSubgraphWithRemap(std::unordered_set<VertexIndex>(begin, end)); };
        std::pair<EdgeLabeledUndirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemap(const std::unordered_set<VertexIndex>& vertices) const;

        const LabeledSuccessors& getOutEdgesOfIdx(VertexIndex vertex) const { return BaseClass::getOutEdgesOfIdx(vertex); }
        const LabeledSuccessors& getNeighboursOfIdx(VertexIndex vertex) const { return getOutEdgesOfIdx(vertex); }
        AdjacencyMatrix getAdjacencyMatrix() const { return BaseClass::getAdjacencyMatrix(); }
        size_t getDegreeIdx(VertexIndex vertex) const { return BaseClass::getOutDegreeIdx(vertex); }
        std::vector<size_t> getDegrees() const { return BaseClass::getOutDegrees(); }


        friend std::ostream& operator <<(std::ostream& stream, const EdgeLabeledUndirectedGraph<EdgeLabel>& graph) {
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
};

template<typename EdgeLabel>
bool EdgeLabeledUndirectedGraph<EdgeLabel>::operator==(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const{
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
void EdgeLabeledUndirectedGraph<EdgeLabel>::resize(size_t newSize){
    if (newSize < size) throw std::invalid_argument("Graph's size cannot be reduced.");
    size = newSize;
    adjacencyList.resize(newSize);
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    if (force || !isEdgeIdx(vertex1, vertex2)) {
        adjacencyList[vertex1].push_back({vertex2, label});
        if (vertex1 != vertex2)
            adjacencyList[vertex2].push_back({vertex1, label});
        distinctEdgeNumber++;
        totalEdgeNumber += label;
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
            EdgeLabeledUndirectedGraph<EdgeLabel>::addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    if (force || !isEdgeIdx(vertex1, vertex2)) {
        adjacencyList[vertex1].push_back({vertex2, label});
        if (vertex1 != vertex2)
            adjacencyList[vertex2].push_back({vertex1, label});
        distinctEdgeNumber++;
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call removeEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    auto& successors = adjacencyList[vertex1];
    size_t sizeBefore = successors.size();

    auto it=successors.begin();
    while (it != successors.end()) {
        if (it->first == vertex2) {
            totalEdgeNumber -= it->second;
            successors.erase(it++);
        }
        else
            it++;
    }
    if (vertex2 != vertex1)
        adjacencyList[vertex2].remove_if([&] (const std::pair<VertexIndex, EdgeLabel>& neighbour) {
                                 return neighbour.first == vertex1;
                             });

    distinctEdgeNumber -= sizeBefore - successors.size();
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call removeEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    auto& successors = adjacencyList[vertex1];
    size_t sizeBefore = successors.size();

    successors.remove_if([&] (const std::pair<VertexIndex, EdgeLabel>& neighbour) {
                             return neighbour.first == vertex2;
                         });
    if (vertex2 != vertex1)
        adjacencyList[vertex2].remove_if([&] (const std::pair<VertexIndex, EdgeLabel>& neighbour) {
                                 return neighbour.first == vertex1;
                             });

    distinctEdgeNumber -= sizeBefore-successors.size();
}

template<typename EdgeLabel>
bool EdgeLabeledUndirectedGraph<EdgeLabel>::isEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) const{
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    VertexIndex smallestAdjacency(vertex1), otherVertex(vertex2);
    if (adjacencyList[vertex1].size() > adjacencyList[vertex2].size()) {
        smallestAdjacency = vertex2;
        otherVertex = vertex1;
    }

    for (auto edge: getOutEdgesOfIdx(smallestAdjacency))
        if (edge.first == otherVertex)
            return true;
    return false;
}


template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::changeEdgeLabelTo(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label) {
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    bool found = false;
    for (auto& neighbour: adjacencyList[vertex1]) {
        if (neighbour.first == vertex2) {
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
EdgeLabeledUndirectedGraph<EdgeLabel> EdgeLabeledUndirectedGraph<EdgeLabel>::getSubgraph(const std::unordered_set<VertexIndex>& vertices) const{
    EdgeLabeledUndirectedGraph<EdgeLabel> subgraph(size);

    for (VertexIndex i: vertices) {
        BaseClass::assertVertexInRange(i);

        for (auto neighbour: getOutEdgesOfIdx(i))
            if (vertices.find(neighbour.first) != vertices.end())
                subgraph.addEdgeIdx(i, neighbour.first, neighbour.second, true);
    }

    return subgraph;
}

template<typename EdgeLabel>
std::pair<EdgeLabeledUndirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> EdgeLabeledUndirectedGraph<EdgeLabel>::getSubgraphWithRemap(const std::unordered_set<VertexIndex>& vertices) const{
    EdgeLabeledUndirectedGraph<EdgeLabel> subgraph(vertices.size());

    std::unordered_map<VertexIndex, VertexIndex> newMapping;

    VertexIndex position=0;
    for (VertexIndex vertex: vertices) {
        newMapping[vertex] = position;
        position++;
    }

    for (VertexIndex i: vertices) {
        BaseClass::assertVertexInRange(i);

        for (auto& neighbour: getOutEdgesOfIdx(i))
            if (vertices.find(neighbour.first) != vertices.end())
                subgraph.addEdgeIdx(newMapping[i], newMapping[neighbour.first], neighbour.second, true);
    }

    return {subgraph, newMapping};
}

} // namespace BaseGraph

#endif
