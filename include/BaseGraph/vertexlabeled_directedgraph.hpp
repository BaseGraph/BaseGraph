#ifndef BASE_GRAPH_VERTEX_LABELED_DIRECTED_GRAPH_HPP
#define BASE_GRAPH_VERTEX_LABELED_DIRECTED_GRAPH_HPP

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/algorithms/graphpaths.h"


namespace BaseGraph{


template<typename T, bool isHashable=false>
class VertexLabeledDirectedGraph: public DirectedGraph{
    private:
        struct Void {};
        template<typename U> struct __HashTable { std::unordered_map<U, VertexIndex> table; };

        typedef typename std::conditional<isHashable, __HashTable<T>, Void>::type HashTable;
        HashTable verticesMapping;

    protected:
        std::vector<T> vertices;

    public:
        VertexLabeledDirectedGraph(): DirectedGraph(0) {};
        VertexLabeledDirectedGraph(const std::list<std::pair<T, T>>& edgeList);
        VertexLabeledDirectedGraph(const DirectedGraph& source, const std::vector<T>& vertices);

        bool operator==(const VertexLabeledDirectedGraph<T>& other) const;
        bool operator!=(const VertexLabeledDirectedGraph<T>& other) const { return !(this->operator==(other)); };


        bool isVertex(T vertex) const;
        void removeVertexFromEdgeList(T vertex) { removeVertexFromEdgeListIdx(findVertexIndex(vertex)); };
        const T& getVertexFromIdx(VertexIndex vertexIdx) const;

        template<typename... Dummy, typename U = T, bool _hashable=isHashable> typename std::enable_if<_hashable>::type
            addVertex(U vertex, bool force=false);
        template<typename... Dummy, typename U = T, bool _hashable=isHashable> typename std::enable_if<!_hashable>::type
            addVertex(U vertex, bool force=false);
        template<typename... Dummy, typename U = T, bool _hashable=isHashable> typename std::enable_if<_hashable>::type
            changeVertexObjectTo(U currentObject, U newObject);
        template<typename... Dummy, typename U = T, bool _hashable=isHashable> typename std::enable_if<!_hashable>::type
            changeVertexObjectTo(U currentObject, U newObject);

        template<typename... Dummy, typename U = T, bool _hashable=isHashable> typename std::enable_if<_hashable, const VertexIndex&>::type
            findVertexIndex(U vertex) const;
        template<typename... Dummy, typename U = T, bool _hashable=isHashable> typename std::enable_if<!_hashable, const VertexIndex>::type
            findVertexIndex(U vertex) const;

        const size_t getSize() const {return size;}
        const std::vector<T>& getVertices() const { return vertices; }
        std::list<T> getOutEdgesOf(T vertex) const;

        std::list<T> convertIndicesListToObjects(const std::list<VertexIndex>& indicesList) const;
        std::vector<T> convertIndicesVectorToObjects(const std::vector<VertexIndex>& indicesVector) const;

        void addEdge(T source, T destination, bool force=false) { addEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); }
        void removeEdge(T source, T destination) { removeEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };
        bool isEdge(T source, T destination) const { return isEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };

        size_t getInDegree(T vertex) const { return getInDegreeIdx(findVertexIndex(vertex)); }
        size_t getOutDegree(T vertex) const { return getOutDegreeIdx(findVertexIndex(vertex)); }


        friend std::ostream& operator <<(std::ostream &stream, const VertexLabeledDirectedGraph<T, isHashable>& graph) {
            stream << "Vertex labeled directed graph of size: " << graph.getSize() << "\n"
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

template<typename T, bool isHashable>
VertexLabeledDirectedGraph<T, isHashable>::VertexLabeledDirectedGraph(const std::list<std::pair<T, T>>& edgeList):
        DirectedGraph(0) {
    for (auto& edge: edgeList) {
        // By default addVertex does not add existing labels
        addVertex(edge.first);
        addVertex(edge.second);
        addEdge(edge.first, edge.second);
    }
}

template<typename T, bool isHashable>
VertexLabeledDirectedGraph<T, isHashable>::VertexLabeledDirectedGraph(const DirectedGraph& source, const std::vector<T>& verticesNames):
        DirectedGraph(0) {
    if (source.getSize() != verticesNames.size())
        throw std::logic_error("The vertices vector must be the size of the graph");

    edgeNumber = 0;
    for (auto vertex: verticesNames) {
        if (isVertex(vertex))
            throw std::logic_error("Couldn't create vertex labeled graph from directed graph: duplicate in vertices names");
        else
            addVertex(vertex, true);
    }

    for (VertexIndex& vertex: source)
        for (const VertexIndex& neighbour: source.getOutEdgesOfIdx(vertex))
            addEdgeIdx(vertex, neighbour);
}


template<typename T, bool isHashable>
bool VertexLabeledDirectedGraph<T, isHashable>::operator==(const VertexLabeledDirectedGraph<T>& other) const{
    bool sameObject = size == other.size;

    std::list<VertexIndex>::const_iterator it;
    try {
    for (VertexIndex i=0; i<size && sameObject; ++i){
        if (!other.isVertex(vertices[i])) sameObject = false;
        if (!isVertex(other.vertices[i])) sameObject = false;

        for (it=adjacencyList[i].begin(); it != adjacencyList[i].end() && sameObject; ++it){
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

template<typename T, bool isHashable>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<_hashable>::type
VertexLabeledDirectedGraph<T, isHashable>::addVertex(U vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addVertex");

    if (force || !isVertex(vertex)) {
        verticesMapping.table[vertex] = vertices.size();
        vertices.push_back(vertex);
        adjacencyList.push_back(std::list<VertexIndex>());
        size++;
    }
}

template<typename T, bool isHashable>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<!_hashable>::type
VertexLabeledDirectedGraph<T, isHashable>::addVertex(U vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addVertex");

    if (force || !isVertex(vertex)) {
        vertices.push_back(vertex);
        adjacencyList.push_back(std::list<VertexIndex>());
        size++;
    }
}


template<typename T, bool isHashable>
bool VertexLabeledDirectedGraph<T, isHashable>::isVertex(T vertex) const{
    bool exists = false;

    for (VertexIndex i=0; i<size && !exists; ++i)
        if (vertices[i] == vertex)
            exists = true;
    return exists;
}

template<typename T, bool isHashable>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<_hashable, const VertexIndex&>::type
VertexLabeledDirectedGraph<T, isHashable>::findVertexIndex(U vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call findVertexIndex");

    if (verticesMapping.table.find(vertex) != verticesMapping.table.end())
        return verticesMapping.table.at(vertex);
    throw std::logic_error("Vertex does not exist");
}

template<typename T, bool isHashable>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<!_hashable, const VertexIndex>::type
VertexLabeledDirectedGraph<T, isHashable>::findVertexIndex(U vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call findVertexIndex");
    for (VertexIndex& i: *this)
        if (vertices[i] == vertex)
            return i;
    throw std::logic_error("Vertex does not exist");
}


template<typename T, bool isHashable>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<_hashable>::type
VertexLabeledDirectedGraph<T, isHashable>::changeVertexObjectTo(U currentObject, U newObject){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call changeVertexObjectTo");
    if (isVertex(newObject)) throw std::logic_error("The object is already used as an attribute by another vertex.");

    VertexIndex vertexIndex = findVertexIndex(currentObject);
    vertices[vertexIndex] = newObject;
    verticesMapping.table.erase(currentObject);
    verticesMapping.table[newObject] = vertexIndex;
}

template<typename T, bool isHashable>
template<typename... Dummy, typename U, bool _hashable>
typename std::enable_if<!_hashable>::type
VertexLabeledDirectedGraph<T, isHashable>::changeVertexObjectTo(U currentObject, U newObject){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call changeVertexObjectTo");
    if (isVertex(newObject)) throw std::logic_error("The object is already used as an attribute by another vertex.");

    vertices[findVertexIndex(currentObject)] = newObject;
}

template<typename T, bool isHashable>
std::list<T> VertexLabeledDirectedGraph<T, isHashable>::getOutEdgesOf(T vertex) const{
    return convertIndicesListToObjects(getOutEdgesOfIdx(findVertexIndex(vertex)));
}

template<typename T, bool isHashable>
std::list<T> VertexLabeledDirectedGraph<T, isHashable>::convertIndicesListToObjects(const std::list<VertexIndex>& indicesList) const{
    std::list<T> returnedList;

    for (auto& element: indicesList) {
        if (element >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedList.push_back(vertices[element]);
    }
    return returnedList;
}

template<typename T, bool isHashable>
std::vector<T> VertexLabeledDirectedGraph<T, isHashable>::convertIndicesVectorToObjects(const std::vector<VertexIndex>& indicesVector) const{
    std::vector<T> returnedVector(indicesVector.size());

    for (auto& element: indicesVector) {
        if (element >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedVector[element] = vertices[element];
    }
    return returnedVector;
}

template<typename T, bool isHashable>
const T& VertexLabeledDirectedGraph<T, isHashable>::getVertexFromIdx(VertexIndex vertexIdx) const {
    if (vertexIdx >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
    return vertices[vertexIdx];
}

} // namespace BaseGraph

#endif
