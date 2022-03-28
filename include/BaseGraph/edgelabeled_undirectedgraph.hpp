#ifndef BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H
#define BASE_GRAPH_EDGE_LABELED_UNDIRECTED_GRAPH_H

#include <algorithm>
#include <fstream>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "BaseGraph/types.h"
#include "BaseGraph/edgelabeled_directedgraph.hpp"


namespace BaseGraph{


template<typename EdgeLabel>
class EdgeLabeledUndirectedGraph: protected EdgeLabeledDirectedGraph<EdgeLabel>{

    public:
        typedef EdgeLabeledDirectedGraph<EdgeLabel> BaseClass;
        using BaseClass::BaseClass;

        void resize(size_t _size) { BaseClass::resize(_size); }
        size_t getSize() const { return BaseClass::getSize(); }
        size_t getDistinctEdgeNumber() const { return BaseClass::getDistinctEdgeNumber(); }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value, long long int>::type
            getTotalEdgeNumber() const { return BaseClass::getTotalEdgeNumber(); }

        bool operator==(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const;
        bool operator!=(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const { return !(this->operator==(other)); }

        void addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false) { _addEdgeIdx(vertex1, vertex2, label, force); }
        void addEdgeIdx(const Edge& edge, const EdgeLabel& label, bool force=false) { addEdgeIdx(edge.first, edge.second, label, force); }
        bool isEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) const;
        bool isEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label) const { return BaseClass::isEdgeIdx(getSmallestAdjacency(vertex1, vertex2), label); }
        bool isEdgeIdx(const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }
        bool isEdgeIdx(const Edge& edge, const EdgeLabel& label) const { return isEdgeIdx(edge.first, edge.second, label); }
        Edge getSmallestAdjacency(VertexIndex vertex1, VertexIndex vertex2) const { return getDegreeOfIdx(vertex1, false) < getDegreeOfIdx(vertex2, false) ?\ Edge{vertex1, vertex2} : Edge{vertex2, vertex1}; }

        virtual void removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) { _removeEdgeIdx(vertex1, vertex2); }
        virtual void removeEdgeIdx(const Edge& edge) { removeEdgeIdx(edge.first, edge.second); }
        const EdgeLabel& getEdgeLabelOf(VertexIndex vertex1, VertexIndex vertex2) const { return BaseClass::getEdgeLabelOf(getSmallestAdjacency(vertex1, vertex2)); }
        const EdgeLabel& getEdgeLabelOf(const Edge& edge) const { return getEdgeLabelOf(edge.first, edge.second); }
        void setEdgeLabelTo(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label) { _setEdgeLabelTo(vertex1, vertex2, label); }
        void setEdgeLabelTo(const Edge& edge, const EdgeLabel& label) { _setEdgeLabelTo(edge.first, edge.second, label); }

        void removeMultiedges() { _removeMultiedges(); }
        void removeSelfLoops() { BaseClass::removeSelfLoops(); }
        void removeVertexFromEdgeListIdx(VertexIndex vertex);
        void clearEdges() { BaseClass::clearEdges(); }

        template<typename Iterator>
        EdgeLabeledUndirectedGraph<EdgeLabel> getSubgraphOfIdx(Iterator begin, Iterator end) const { return getSubgraphOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        EdgeLabeledUndirectedGraph<EdgeLabel> getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const;
        template<typename Iterator>
        std::pair<EdgeLabeledUndirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(Iterator begin, Iterator end) const {
            return getSubgraphWithRemapOfIdx(std::unordered_set<VertexIndex>(begin, end)); };
        std::pair<EdgeLabeledUndirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const;

        const LabeledSuccessors<EdgeLabel>& getOutEdgesOfIdx(VertexIndex vertex) const { return BaseClass::getOutEdgesOfIdx(vertex); }
        const LabeledSuccessors<EdgeLabel>& getNeighboursOfIdx(VertexIndex vertex) const { return getOutEdgesOfIdx(vertex); }
        virtual AdjacencyMatrix getAdjacencyMatrix() const;
        virtual size_t getDegreeOfIdx(VertexIndex vertex, bool withSelfLoops=true) const;
        virtual std::vector<size_t> getDegrees(bool withSelfLoops=true) const;


        friend std::ostream& operator <<(std::ostream& stream, const EdgeLabeledUndirectedGraph<EdgeLabel>& graph) {
            stream << "Undirected graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << "(" << neighbour.vertexIndex << ", " << neighbour.label << ")";
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
        iterator end() const {return iterator(getSize());}

    private:
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            _addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            _addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force=false);

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            _removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            _removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2);

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            _setEdgeLabelTo(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label);
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            _setEdgeLabelTo(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label) { BaseClass::setEdgeLabelTo(vertex1, vertex2, label); BaseClass::setEdgeLabelTo(vertex2, vertex1, label); }

        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<std::is_integral<U>::value>::type
            _removeMultiedges();
        template<typename ...Dummy, typename U=EdgeLabel>
        typename std::enable_if<!std::is_integral<U>::value>::type
            _removeMultiedges();
};

template<typename EdgeLabel>
bool EdgeLabeledUndirectedGraph<EdgeLabel>::operator==(const EdgeLabeledUndirectedGraph<EdgeLabel>& other) const{
    bool isEqual = getSize() == other.getSize()
                        && BaseClass::totalEdgeNumber == other.BaseClass::totalEdgeNumber
                        && BaseClass::distinctEdgeNumber == other.BaseClass::distinctEdgeNumber;

    typename LabeledSuccessors<EdgeLabel>::const_iterator it;
    for (VertexIndex i=0; i<getSize() && isEqual; ++i){
        for (it=BaseClass::adjacencyList[i].begin(); it != BaseClass::adjacencyList[i].end() && isEqual; ++it){
            if (!other.isEdgeIdx(i, it->vertexIndex))
                isEqual = false;
        }

        for (it=other.BaseClass::adjacencyList[i].begin(); it != other.BaseClass::adjacencyList[i].end() && isEqual; ++it){
            if (!isEdgeIdx(i, it->vertexIndex))
                isEqual = false;
        }
    }
    return isEqual;
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::_addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _addEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    if (force || !isEdgeIdx(vertex1, vertex2)) {
        BaseClass::adjacencyList[vertex1].push_back({vertex2, label});
        if (vertex1 != vertex2)
            BaseClass::adjacencyList[vertex2].push_back({vertex1, label});
        BaseClass::distinctEdgeNumber++;
        BaseClass::totalEdgeNumber += label;
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
            EdgeLabeledUndirectedGraph<EdgeLabel>::_addEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _addEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    if (force || !isEdgeIdx(vertex1, vertex2)) {
        BaseClass::adjacencyList[vertex1].push_back({vertex2, label});
        if (vertex1 != vertex2)
            BaseClass::adjacencyList[vertex2].push_back({vertex1, label});
        BaseClass::distinctEdgeNumber++;
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::_removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _removeEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    auto& successors = BaseClass::adjacencyList[vertex1];
    size_t sizeBefore = successors.size();

    auto it=successors.begin();
    while (it != successors.end()) {
        if (it->vertexIndex == vertex2) {
            BaseClass::totalEdgeNumber -= it->label;
            successors.erase(it++);
        }
        else
            it++;
    }
    if (vertex2 != vertex1)
        BaseClass::adjacencyList[vertex2].remove_if([&] (const LabeledNeighbour<EdgeLabel>& neighbour) {
                                 return neighbour.vertexIndex == vertex1;
                             });

    BaseClass::distinctEdgeNumber -= sizeBefore - successors.size();
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::_removeEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _removeEdgeIdx");
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    auto& successors = BaseClass::adjacencyList[vertex1];
    size_t sizeBefore = successors.size();

    successors.remove_if([&] (const LabeledNeighbour<EdgeLabel>& neighbour) {
                             return neighbour.vertexIndex == vertex2;
                         });
    if (vertex2 != vertex1)
        BaseClass::adjacencyList[vertex2].remove_if([&] (const LabeledNeighbour<EdgeLabel>& neighbour) {
                                 return neighbour.vertexIndex == vertex1;
                             });

    BaseClass::distinctEdgeNumber -= sizeBefore-successors.size();
}

template<typename EdgeLabel>
bool EdgeLabeledUndirectedGraph<EdgeLabel>::isEdgeIdx(VertexIndex vertex1, VertexIndex vertex2) const{
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    VertexIndex smallestAdjacency(vertex1), otherVertex(vertex2);
    if (BaseClass::adjacencyList[vertex1].size() > BaseClass::adjacencyList[vertex2].size()) {
        smallestAdjacency = vertex2;
        otherVertex = vertex1;
    }

    for (auto neighbour: getOutEdgesOfIdx(smallestAdjacency))
        if (neighbour.vertexIndex == otherVertex)
            return true;
    return false;
}


template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::_setEdgeLabelTo(VertexIndex vertex1, VertexIndex vertex2, const EdgeLabel& label) {
    BaseClass::assertVertexInRange(vertex1);
    BaseClass::assertVertexInRange(vertex2);

    bool found = false;
    for (auto& neighbour: BaseClass::adjacencyList[vertex1]) {
        if (neighbour.vertexIndex == vertex2) {
            BaseClass::totalEdgeNumber += label - neighbour.label;
            neighbour.label = label;

            if (vertex1 != vertex2)
                for (auto& neighbour2: BaseClass::adjacencyList[vertex2])
                    if (neighbour2.vertexIndex == vertex1) {
                        neighbour2.label = label;
                        break;
                    }

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
        EdgeLabeledUndirectedGraph<EdgeLabel>::_removeMultiedges(){
    std::set<VertexIndex> seenVertices;
    typename LabeledSuccessors<EdgeLabel>::iterator j;

    for (VertexIndex i=0; i<BaseClass::size; ++i){
        j = BaseClass::adjacencyList[i].begin();

        while(j != BaseClass::adjacencyList[i].end()){
            if (!seenVertices.count(j->vertexIndex)) {
                seenVertices.insert(j->vertexIndex);
                j++;
            }
            else {
                if (i <= j->vertexIndex) {
                    BaseClass::totalEdgeNumber -= j->label;
                    BaseClass::distinctEdgeNumber--;
                }
                BaseClass::adjacencyList[i].erase(j++);
            }
        }
        seenVertices.clear();
    }
}

template<typename EdgeLabel>
template<typename ...Dummy, typename U>
typename std::enable_if<!std::is_integral<U>::value>::type
        EdgeLabeledUndirectedGraph<EdgeLabel>::_removeMultiedges(){
    std::set<VertexIndex> seenVertices;
    typename LabeledSuccessors<EdgeLabel>::iterator j;

    for (VertexIndex i=0; i<BaseClass::size; ++i){
        j = BaseClass::adjacencyList[i].begin();

        while(j != BaseClass::adjacencyList[i].end()){
            if (!seenVertices.count(j->vertexIndex)) {
                seenVertices.insert(j->vertexIndex);
                j++;
            }
            else {
                if (i <= j->vertexIndex)
                    BaseClass::distinctEdgeNumber--;
                BaseClass::adjacencyList[i].erase(j++);
            }
        }
        seenVertices.clear();
    }
}

template<typename EdgeLabel>
void EdgeLabeledUndirectedGraph<EdgeLabel>::removeVertexFromEdgeListIdx(VertexIndex vertex){
    BaseClass::assertVertexInRange(vertex);

    auto& successors = BaseClass::adjacencyList[vertex];
    auto it = successors.begin();
    while (it != successors.end())
        // Avoid deleting self-loops so that distinct and total edge number are adjusted by removeEdgeIdx
        if (it->vertexIndex != vertex)
            successors.erase(it++);
        else
            it++;

    for (VertexIndex i=0; i<BaseClass::size; ++i)
        removeEdgeIdx(i, vertex);
}

template<typename EdgeLabel>
std::vector<size_t> EdgeLabeledUndirectedGraph<EdgeLabel>::getDegrees(bool withSelfLoops) const{
    std::vector<size_t> degrees(getSize());
    for (VertexIndex i: *this)
        degrees[i] = getDegreeOfIdx(i, withSelfLoops);
    return degrees;
}


template<typename EdgeLabel>
size_t EdgeLabeledUndirectedGraph<EdgeLabel>::getDegreeOfIdx(VertexIndex vertex, bool withSelfLoops) const{
    BaseClass::assertVertexInRange(vertex);

    if (!withSelfLoops)
        return BaseClass::adjacencyList[vertex].size();

    size_t degree=0;
    for (auto neighbor: getNeighboursOfIdx(vertex))
        degree += neighbor.vertexIndex==vertex ? 2:1;
    return degree;
}

template<typename EdgeLabel>
AdjacencyMatrix EdgeLabeledUndirectedGraph<EdgeLabel>::getAdjacencyMatrix() const{
    const size_t& _size = EdgeLabeledDirectedGraph<EdgeLabel>::size;

    AdjacencyMatrix adjacencyMatrix;
    adjacencyMatrix.resize(_size, std::vector<size_t>(_size, 0));

    for (VertexIndex i=0; i<_size; ++i)
        for (auto& neighbour: getOutEdgesOfIdx(i)) {
            const auto& j = neighbour.vertexIndex;
            adjacencyMatrix[i][j] += i!=j ? 1:2;
        }

    return adjacencyMatrix;
}


template<typename EdgeLabel>
EdgeLabeledUndirectedGraph<EdgeLabel> EdgeLabeledUndirectedGraph<EdgeLabel>::getSubgraphOfIdx(const std::unordered_set<VertexIndex>& vertices) const{
    EdgeLabeledUndirectedGraph<EdgeLabel> subgraph(getSize());

    for (VertexIndex i: vertices) {
        BaseClass::assertVertexInRange(i);

        for (auto neighbour: getOutEdgesOfIdx(i))
            if (i <= neighbour.vertexIndex && vertices.count(neighbour.vertexIndex))
                subgraph.addEdgeIdx(i, neighbour.vertexIndex, neighbour.label, true);
    }

    return subgraph;
}

template<typename EdgeLabel>
std::pair<EdgeLabeledUndirectedGraph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>> EdgeLabeledUndirectedGraph<EdgeLabel>::getSubgraphWithRemapOfIdx(const std::unordered_set<VertexIndex>& vertices) const{
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
            if (i <= neighbour.vertexIndex && vertices.count(neighbour.vertexIndex))
                subgraph.addEdgeIdx(newMapping[i], newMapping[neighbour.vertexIndex], neighbour.label, true);
    }

    return {subgraph, newMapping};
}


} // namespace BaseGraph

#endif
