#ifndef BASE_GRAPH_DIRECTED_GRAPH_H
#define BASE_GRAPH_DIRECTED_GRAPH_H

#include <fstream>
#include <string>

#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>


namespace BaseGraph{

typedef size_t VertexIndex;
typedef std::pair<VertexIndex, VertexIndex> Edge;
typedef std::list<VertexIndex> Successors;
typedef std::vector<Successors> AdjacencyLists;
typedef std::vector<std::vector<size_t>> AdjacencyMatrix;


class DirectedGraph{

    public:
        explicit DirectedGraph(size_t size=0): size(0), edgeNumber(0) {resize(size);}

        void resize(size_t size);
        size_t getSize() const { return size; }
        size_t getEdgeNumber() const { return edgeNumber; }

        bool operator==(const DirectedGraph& other) const;
        bool operator!=(const DirectedGraph& other) const { return !(this->operator==(other)); }

        void addEdgeIdx(VertexIndex source, VertexIndex destination, bool force=false);
        void addEdgeIdx(const Edge& edge, bool force=false) { addEdgeIdx(edge.first, edge.second, force); }
        void addReciprocalEdgeIdx(VertexIndex vertex1, VertexIndex vertex2, bool force=false) { addEdgeIdx(vertex1, vertex2, force); addEdgeIdx(vertex2, vertex1, force); }
        void addReciprocalEdgeIdx(const Edge& edge, bool force=false) {addReciprocalEdgeIdx(edge, force);}
        bool isEdgeIdx(VertexIndex source, VertexIndex destination) const;
        bool isEdgeIdx(const Edge& edge) const { return isEdgeIdx(edge.first, edge.second); }
        void removeEdgeIdx(VertexIndex source, VertexIndex destination);
        void removeEdgeIdx(const Edge& edge) {removeEdgeIdx(edge.first, edge.second);}
        void removeMultiedges();
        void removeSelfLoops();
        void removeVertexFromEdgeListIdx(VertexIndex vertex);
        void clear();

        template <typename Iterator>
        DirectedGraph getSubgraph(Iterator begin, Iterator end) const { return getSubgraph(std::unordered_set<VertexIndex>(begin, end)); };
        DirectedGraph getSubgraph(const std::unordered_set<VertexIndex>& vertices) const;
        template <typename Iterator>
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemap(Iterator begin, Iterator end) const {
            return getSubgraphWithRemap(std::unordered_set<VertexIndex>(begin, end)); };
        std::pair<DirectedGraph, std::unordered_map<VertexIndex, VertexIndex>> getSubgraphWithRemap(const std::unordered_set<VertexIndex>& vertices) const;

        const Successors& getOutEdgesOfIdx(VertexIndex vertex) const;
        AdjacencyLists getInEdgesOfVertices() const;
        AdjacencyMatrix getAdjacencyMatrix() const;
        size_t getInDegreeIdx(VertexIndex vertex) const;
        std::vector<size_t> getInDegrees() const;
        size_t getOutDegreeIdx(VertexIndex vertex) const;
        std::vector<size_t> getOutDegrees() const;

        DirectedGraph getReversedGraph() const;

        friend std::ostream& operator <<(std::ostream &stream, const DirectedGraph& graph) {
            stream << "Directed graph of size: " << graph.getSize() << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << i << ": ";
                for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                    stream << neighbour << ", ";
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
        AdjacencyLists adjacencyList;
        size_t size;
        size_t edgeNumber;
};

} // namespace BaseGraph

#endif
