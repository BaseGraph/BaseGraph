#ifndef BASE_GRAPH_VERTEX_LABELED_GRAPH_HPP
#define BASE_GRAPH_VERTEX_LABELED_GRAPH_HPP

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


namespace BaseGraph{


template<typename GraphBase, typename VertexLabel, bool useHashTable>
class VertexLabeledGraph: public GraphBase {
    private:
        struct Void {};
        // If available, this hash table allows an O(1) complexity for searching a vertex index from a vertex label.
        template<typename U> struct __HashTable { std::unordered_map<U, VertexIndex> table; };
        typedef typename std::conditional<useHashTable, __HashTable<VertexLabel>, Void>::type HashTable;
        HashTable verticesMapping;

    protected:
        std::vector<VertexLabel> vertices;

    public:
        explicit VertexLabeledGraph(): GraphBase(0) {}
        explicit VertexLabeledGraph(const std::list<std::pair<VertexLabel, VertexLabel>>& edgeList);
        explicit VertexLabeledGraph(const GraphBase& source, const std::vector<VertexLabel>& vertices);
        const std::vector<VertexLabel>& getVertices() const { return vertices; }

        template<bool otherHashable>
            bool operator==(const VertexLabeledGraph<GraphBase, VertexLabel, otherHashable>& other) const;
        template<bool otherHashable>
            bool operator!=(const VertexLabeledGraph<GraphBase, VertexLabel, otherHashable>& other) const { return !(this->operator==(other)); };

        void addVertex(const VertexLabel& vertex, bool force=false) { _addVertex(vertex, force); }
        bool isVertex(const VertexLabel& vertex) const { return _isVertex(vertex); }
        const VertexLabel& getLabelFromIndex(VertexIndex vertexIdx) const { this->assertVertexInRange(vertexIdx); return vertices[vertexIdx]; }
        VertexIndex findVertexIndex(const VertexLabel& vertex) const { return _findVertexIndex(vertex); }
        void setVertexLabelTo(const VertexIndex& vertexIndex, const VertexLabel& newLabel) { _setVertexLabelTo(vertexIndex, newLabel); }
        void setVertexLabelTo(const VertexLabel& currentLabel, const VertexLabel& newLabel) { _setVertexLabelTo(_findVertexIndex(currentLabel), newLabel); }
        void removeVertexFromEdgeList(VertexLabel vertex) { this->removeVertexFromEdgeListIdx(findVertexIndex(vertex)); };

        /* Cannot let user modify directly vertex labels when there is a hash table. As of the current unordered_map STL implementation,
         * unordered_map::extract combined with other operations is the only way to change a key without reallocation (see
         * https://en.cppreference.com/w/cpp/container/unordered_map/extract ). Since vertex labels are kept in a separate container,
         * both objects can't stay in sync by sharing memory. The method setVertexLabelTo is currently the only way to modify a vertex label
         * when using a hash table.
         */
        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<_useHash, VertexIndex&>::type
            operator[](const VertexIndex& vertexIdx) { this->assertVertexInRange(vertexIdx); return vertices[vertexIdx]; }

        void addEdge(VertexLabel source, VertexLabel destination, bool force=false) { this->addEdgeIdx(findVertexIndex(source), findVertexIndex(destination), force); }
        bool hasEdge(VertexLabel source, VertexLabel destination) const { return this->hasEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };
        void removeEdge(VertexLabel source, VertexLabel destination) { this->removeEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };


        size_t getInDegreeOf(VertexLabel vertex) const { return this->getInDegreeOfIdx(findVertexIndex(vertex)); }
        size_t getOutDegreeOf(VertexLabel vertex) const { return this->getOutDegreeOfIdx(findVertexIndex(vertex)); }


        friend std::ostream& operator <<(std::ostream &stream, const VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>& graph) {
            stream << "Vertex labeled graph of" << typeid(GraphBase).name() << " with size " << graph.getSize() << "\n"
                   << "Vertex label type \"" << typeid(VertexLabel).name() << "\" used as " << (useHashTable ? "hashable": "not hashable") << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << graph.vertices[i] << ": ";
                auto& outEdges = graph.getOutEdgesOfIdx(i);
                for (auto& neighbour: outEdges)
                    stream << (neighbour==*outEdges.begin() ? "" : ", ")
                           << graph.vertices[neighbour];
                stream << "\n";
            }
            return stream;
        }

    private:
        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<_useHash>::type
            _addVertex(const VertexLabel& vertex, bool force=false);
        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<!_useHash>::type
            _addVertex(const VertexLabel& vertex, bool force=false);

        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<_useHash, VertexIndex>::type
            _findVertexIndex(const VertexLabel& vertex) const;
        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<!_useHash, VertexIndex>::type
            _findVertexIndex(const VertexLabel& vertex) const;

        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<_useHash, bool>::type
            _isVertex(const VertexLabel& vertex) const;
        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<!_useHash, bool>::type
            _isVertex(const VertexLabel& vertex) const;

        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<_useHash>::type
            _setVertexLabelTo(const VertexIndex& vertexIndex, const VertexLabel& newLabel);
        template<typename... Dummy, bool _useHash=useHashTable> typename std::enable_if<!_useHash>::type
            _setVertexLabelTo(const VertexIndex& vertexIndex, const VertexLabel& newLabel);
};

template <typename VertexLabel, bool useHashTable=false>
using VertexLabeledDirectedGraph = VertexLabeledGraph<DirectedGraph, VertexLabel, useHashTable>;

template <typename VertexLabel, bool useHashTable=false>
using VertexLabeledUndirectedGraph = VertexLabeledGraph<UndirectedGraph, VertexLabel, useHashTable>;

//template <typename VertexLabel, typename EdgeLabel, bool useHashTable=false>
//using VertexAndEdgeLabeledDirectedGraph = VertexLabeledGraph<EdgeLabeledDirectedGraph<EdgeLabel>, VertexLabel, useHashTable>;

//template <typename VertexLabel, typename EdgeLabel, bool useHashTable=false>
//using VertexAndEdgeLabeledUndirectedGraph = VertexLabeledGraph<EdgeLabeledUndirectedGraph<EdgeLabel>, VertexLabel, useHashTable>;



template<typename GraphBase, typename VertexLabel, bool useHashTable>
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::VertexLabeledGraph(const std::list<std::pair<VertexLabel, VertexLabel>>& edgeList) {
    for (auto& edge: edgeList) {
        // By default _addVertex does not add existing labels
        _addVertex(edge.first);
        _addVertex(edge.second);
        addEdge(edge.first, edge.second);
    }
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::VertexLabeledGraph(const GraphBase& source, const std::vector<VertexLabel>& verticesLabels) {
    if (source.getSize() != verticesLabels.size())
        throw std::invalid_argument("The vertices vector must be the size of the graph");

    this->edgeNumber = 0;
    for (auto vertex: verticesLabels) {
        if (isVertex(vertex))
            throw std::invalid_argument("Couldn't create vertex labeled graph from directed graph: duplicate in vertices names");
        else
            _addVertex(vertex, true);
    }

    for (VertexIndex& vertex: source)
        for (const VertexIndex& neighbour: source.getOutEdgesOfIdx(vertex))
            this->addEdgeIdx(vertex, neighbour);
}


template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<bool otherHashable>
bool VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::operator==(const VertexLabeledGraph<GraphBase, VertexLabel, otherHashable>& other) const{
    return vertices == other.vertices && GraphBase::operator==(other);
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<_useHash>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_addVertex(const VertexLabel& vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _addVertex");

    if (force || !isVertex(vertex)) {
        verticesMapping.table[vertex] = vertices.size();
        vertices.push_back(vertex);
        this->adjacencyList.push_back(std::list<VertexIndex>());
        this->size++;
    }
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<!_useHash>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_addVertex(const VertexLabel& vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _addVertex");

    if (force || !isVertex(vertex)) {
        vertices.push_back(vertex);
        this->adjacencyList.push_back(std::list<VertexIndex>());
        this->size++;
    }
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<_useHash, bool>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_isVertex(const VertexLabel& vertex) const{
    return verticesMapping.table.count(vertex);
}


template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<!_useHash, bool>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_isVertex(const VertexLabel& vertex) const{
    bool exists = false;

    for (VertexIndex i=0; i<this->size && !exists; ++i)
        if (vertices[i] == vertex)
            exists = true;
    return exists;
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<_useHash, VertexIndex>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_findVertexIndex(const VertexLabel& vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _findVertexIndex");

    auto it = verticesMapping.table.find(vertex);
    if (it != verticesMapping.table.end())
        return it->second;
    throw std::invalid_argument("Vertex does not exist");
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<!_useHash, VertexIndex>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_findVertexIndex(const VertexLabel& vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call _findVertexIndex");
    for (VertexIndex& i: *this)
        if (vertices[i] == vertex)
            return i;
    throw std::invalid_argument("Vertex does not exist");
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<_useHash>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_setVertexLabelTo(const VertexIndex& vertexIndex, const VertexLabel& newLabel){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call setVertexObjectTo");
    this->assertVertexInRange(vertexIndex);
    if (isVertex(newLabel)) throw std::invalid_argument("The object is already used as an attribute by another vertex.");

    verticesMapping.table.erase(vertices[vertexIndex]);
    vertices[vertexIndex] = newLabel;
    verticesMapping.table[newLabel] = vertexIndex;
}

template<typename GraphBase, typename VertexLabel, bool useHashTable>
template<typename... Dummy, bool _useHash>
typename std::enable_if<!_useHash>::type
VertexLabeledGraph<GraphBase, VertexLabel, useHashTable>::_setVertexLabelTo(const VertexIndex& vertexIndex, const VertexLabel& newLabel){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call setVertexObjectTo");
    this->assertVertexInRange(vertexIndex);
    if (isVertex(newLabel)) throw std::invalid_argument("newLabel is already used as an attribute by another vertex.");

    vertices[vertexIndex] = newLabel;
}

} // namespace BaseGraph

#endif
