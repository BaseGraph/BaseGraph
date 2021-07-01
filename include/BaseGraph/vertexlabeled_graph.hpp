#ifndef BASE_GRAPH_VERTEX_LABELED_DIRECTED_GRAPH_HPP
#define BASE_GRAPH_VERTEX_LABELED_DIRECTED_GRAPH_HPP

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"


namespace BaseGraph{

template<typename Label, bool isHashable, bool isDirected>
class VertexLabeledGraph: public std::conditional<isDirected, DirectedGraph, UndirectedGraph>::type {
    private:
        typename std::conditional<isDirected, DirectedGraph, UndirectedGraph>::type BaseClass;

        struct Void {};
        template<typename U> struct __HashTable { std::unordered_map<U, VertexIndex> table; };

        typedef typename std::conditional<isHashable, __HashTable<Label>, Void>::type HashTable;
        HashTable verticesMapping;

    protected:
        std::vector<Label> vertices;

    public:
        VertexLabeledGraph(): BaseClass() {};
        VertexLabeledGraph(const std::list<std::pair<Label, Label>>& edgeList);
        VertexLabeledGraph(const typename std::conditional<isDirected, DirectedGraph, UndirectedGraph>::type& source, const std::vector<Label>& vertices);

        template <bool otherHashable, bool otherDirected> 
        bool operator==(const VertexLabeledGraph<Label, otherHashable, otherDirected>& other) const;
        template <bool otherHashable, bool otherDirected> 
        bool operator!=(const VertexLabeledGraph<Label, otherHashable, otherDirected>& other) const { return !(this->operator==(other)); };


        bool isVertex(const Label& vertex) const;
        void removeVertexFromEdgeList(Label vertex) { this->removeVertexFromEdgeListIdx(findVertexIndex(vertex)); };
        const Label& getVertexFromIdx(VertexIndex vertexIdx) const;

        template<typename... Dummy, typename U = Label, bool _hashable=isHashable> typename std::enable_if<_hashable>::type
            addVertex(const U& vertex, bool force=false);
        template<typename... Dummy, typename U = Label, bool _hashable=isHashable> typename std::enable_if<!_hashable>::type
            addVertex(const U& vertex, bool force=false);
        template<typename... Dummy, typename U = Label, bool _hashable=isHashable> typename std::enable_if<_hashable>::type
            changeVertexObjectTo(const U& currentObject, const U& newObject);
        template<typename... Dummy, typename U = Label, bool _hashable=isHashable> typename std::enable_if<!_hashable>::type
            changeVertexObjectTo(const U& currentObject, const U& newObject);

        template<typename... Dummy, typename U = Label, bool _hashable=isHashable> typename std::enable_if<_hashable, const VertexIndex&>::type
            findVertexIndex(const U& vertex) const;
        template<typename... Dummy, typename U = Label, bool _hashable=isHashable> typename std::enable_if<!_hashable, const VertexIndex>::type
            findVertexIndex(const U& vertex) const;

        std::list<Label> getOutEdgesOf(Label vertex) const;

        const std::vector<Label>& getVertices() const { return vertices; }

        std::list<Label> convertIndicesListToObjects(const std::list<VertexIndex>& indicesList) const;
        std::vector<Label> convertIndicesVectorToObjects(const std::vector<VertexIndex>& indicesVector) const;

        void addEdge(Label source, Label destination, bool force=false) { this->addEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); }
        void removeEdge(Label source, Label destination) { this->removeEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };
        bool isEdge(Label source, Label destination) const { return this->isEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };

        template<typename... Dummy, typename U = Label, bool _directed=isDirected> typename std::enable_if<_directed, size_t>::type
            getInDegree(U vertex) { return this->getInDegreeIdx(findVertexIndex(vertex)); }
        template<typename... Dummy, typename U = Label, bool _directed=isDirected> typename std::enable_if<_directed, size_t>::type
            getOutDegree(U vertex) { return this->getInDegreeIdx(findVertexIndex(vertex)); }
        template<typename... Dummy, typename U = Label, bool _directed=isDirected> typename std::enable_if<!_directed, size_t>::type
            getDegree(U vertex) { return this->getDegreeIdx(findVertexIndex(vertex)); }


        friend std::ostream& operator <<(std::ostream &stream, const VertexLabeledGraph<Label, isHashable, isDirected>& graph) {
            std::string graphOrientation = isDirected ? "directed" : "undirected";
            stream << "Vertex labeled " + graphOrientation + "graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << graph.vertices[i] << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << graph.vertices[neighbour] << ", ";
                stream << "\n";
            }
            return stream;
        }
};

template <typename Label, bool isHashable=false>
using VertexLabeledDirectedGraph = VertexLabeledGraph<Label, isHashable, true>;

template <typename Label, bool isHashable=false>
using VertexLabeledUndirectedGraph = VertexLabeledGraph<Label, isHashable, false>;


template<typename Label, bool isHashable, bool isDirected>
VertexLabeledGraph<Label, isHashable, isDirected>::VertexLabeledGraph(const std::list<std::pair<Label, Label>>& edgeList) {
    for (auto& edge: edgeList) {
        // By default addVertex does not add existing labels
        addVertex(edge.first);
        addVertex(edge.second);
        addEdge(edge.first, edge.second);
    }
}

template<typename Label, bool isHashable, bool isDirected>
VertexLabeledGraph<Label, isHashable, isDirected>::VertexLabeledGraph(const typename std::conditional<isDirected, DirectedGraph, UndirectedGraph>::type& source, const std::vector<Label>& verticesNames) {
    if (source.getSize() != verticesNames.size())
        throw std::logic_error("The vertices vector must be the size of the graph");

    this->edgeNumber = 0;
    for (auto vertex: verticesNames) {
        if (isVertex(vertex))
            throw std::logic_error("Couldn't create vertex labeled graph from directed graph: duplicate in vertices names");
        else
            addVertex(vertex, true);
    }

    for (VertexIndex& vertex: source)
        for (const VertexIndex& neighbour: source.getOutEdgesOfIdx(vertex))
            this->addEdgeIdx(vertex, neighbour);
}


template<typename Label, bool isHashable, bool isDirected>
template<bool otherHashable, bool otherDirected> 
bool VertexLabeledGraph<Label, isHashable, isDirected>::operator==(const VertexLabeledGraph<Label, otherHashable, otherDirected>& other) const{
    bool sameObject = this->size == other.size;
    auto& _adjacencyList = this->adjacencyList;

    std::list<VertexIndex>::const_iterator it;
    try {
    for (VertexIndex i=0; i<this->size && sameObject; ++i){
        if (!other.isVertex(vertices[i])) sameObject = false;
        if (!isVertex(other.vertices[i])) sameObject = false;

        for (it=_adjacencyList[i].begin(); it != _adjacencyList[i].end() && sameObject; ++it){
            if (!other.isEdge(vertices[i], vertices[*it]))
                sameObject = false;
        }

        for (it=other.adjacencyList[i].begin(); it != other.adjacencyList[i].end() && sameObject; ++it){
            if (!isEdge(other.vertices[i], other.vertices[*it]))
                sameObject = false;
        }
    }
    } catch (std::logic_error&) {  // isEdge calling findVertexIndex threw "Vertex does not exist"
        sameObject = false;
    }

    return sameObject;
}

template<typename Label, bool isHashable, bool isDirected>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<_hashable>::type
VertexLabeledGraph<Label, isHashable, isDirected>::addVertex(const U& vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addVertex");

    if (force || !isVertex(vertex)) {
        verticesMapping.table[vertex] = vertices.size();
        vertices.push_back(vertex);
        this->adjacencyList.push_back(std::list<VertexIndex>());
        this->size++;
    }
}

template<typename Label, bool isHashable, bool isDirected>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<!_hashable>::type
VertexLabeledGraph<Label, isHashable, isDirected>::addVertex(const U& vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addVertex");

    if (force || !isVertex(vertex)) {
        vertices.push_back(vertex);
        this->adjacencyList.push_back(std::list<VertexIndex>());
        this->size++;
    }
}


template<typename Label, bool isHashable, bool isDirected>
bool VertexLabeledGraph<Label, isHashable, isDirected>::isVertex(const Label& vertex) const{
    bool exists = false;

    for (VertexIndex i=0; i<this->size && !exists; ++i)
        if (vertices[i] == vertex)
            exists = true;
    return exists;
}

template<typename Label, bool isHashable, bool isDirected>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<_hashable, const VertexIndex&>::type
VertexLabeledGraph<Label, isHashable, isDirected>::findVertexIndex(const U& vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call findVertexIndex");

    if (verticesMapping.table.find(vertex) != verticesMapping.table.end())
        return verticesMapping.table.at(vertex);
    throw std::logic_error("Vertex does not exist");
}

template<typename Label, bool isHashable, bool isDirected>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<!_hashable, const VertexIndex>::type
VertexLabeledGraph<Label, isHashable, isDirected>::findVertexIndex(const U& vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call findVertexIndex");
    for (VertexIndex& i: *this)
        if (vertices[i] == vertex)
            return i;
    throw std::logic_error("Vertex does not exist");
}


template<typename Label, bool isHashable, bool isDirected>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<_hashable>::type
VertexLabeledGraph<Label, isHashable, isDirected>::changeVertexObjectTo(const U& currentObject, const U& newObject){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call changeVertexObjectTo");
    if (isVertex(newObject)) throw std::logic_error("The object is already used as an attribute by another vertex.");

    VertexIndex vertexIndex = findVertexIndex(currentObject);
    vertices[vertexIndex] = newObject;
    verticesMapping.table.erase(currentObject);
    verticesMapping.table[newObject] = vertexIndex;
}

template<typename Label, bool isHashable, bool isDirected>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<!_hashable>::type
VertexLabeledGraph<Label, isHashable, isDirected>::changeVertexObjectTo(const U& currentObject, const U& newObject){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call changeVertexObjectTo");
    if (isVertex(newObject)) throw std::logic_error("The object is already used as an attribute by another vertex.");

    vertices[findVertexIndex(currentObject)] = newObject;
}

template<typename Label, bool isHashable, bool isDirected>
std::list<Label> VertexLabeledGraph<Label, isHashable, isDirected>::getOutEdgesOf(Label vertex) const{
    return convertIndicesListToObjects(this->getOutEdgesOfIdx(findVertexIndex(vertex)));
}

template<typename Label, bool isHashable, bool isDirected>
std::list<Label> VertexLabeledGraph<Label, isHashable, isDirected>::convertIndicesListToObjects(const std::list<VertexIndex>& indicesList) const{
    std::list<Label> returnedList;

    for (auto& element: indicesList) {
        if (element >= this->size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedList.push_back(vertices[element]);
    }
    return returnedList;
}

template<typename Label, bool isHashable, bool isDirected>
std::vector<Label> VertexLabeledGraph<Label, isHashable, isDirected>::convertIndicesVectorToObjects(const std::vector<VertexIndex>& indicesVector) const{
    std::vector<Label> returnedVector(indicesVector.size());

    for (auto& element: indicesVector) {
        if (element >= this->size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedVector[element] = vertices[element];
    }
    return returnedVector;
}

template<typename Label, bool isHashable, bool isDirected>
const Label& VertexLabeledGraph<Label, isHashable, isDirected>::getVertexFromIdx(VertexIndex vertexIdx) const {
    if (vertexIdx >= this->size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
    return vertices[vertexIdx];
}

} // namespace BaseGraph

#endif
