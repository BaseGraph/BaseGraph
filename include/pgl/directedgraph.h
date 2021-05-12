#ifndef PGL_DIRECTED_GRAPH_H
#define PGL_DIRECTED_GRAPH_H

#include <fstream>
#include <string>

#include <vector>
#include <list>


namespace PGL{

class DirectedGraph{

    public:
        explicit DirectedGraph(size_t size=0):size(0), edgeNumber(0) {resize(size);};
        DirectedGraph(const DirectedGraph& source);

        void resize(size_t size);
        size_t getSize() const {return size;}

        DirectedGraph operator=(const DirectedGraph& other);
        bool operator==(const DirectedGraph& other) const;
        bool operator!=(const DirectedGraph& other) const { return !(this->operator==(other)); };

        void addEdgeIdx(size_t source, size_t destination, bool force=false);
        void addReciprocalEdgeIdx(size_t vertex1, size_t vertex2, bool force=false) { addEdgeIdx(vertex1, vertex2, force); addEdgeIdx(vertex2, vertex1); };
        bool isEdgeIdx(size_t source, size_t destination) const;
        void removeEdgeIdx(size_t source, size_t destination);
        void removeMultiedges();
        void removeSelfLoops();
        void removeVertexFromEdgeListIdx(size_t vertex);

        size_t getEdgeNumber() const { return edgeNumber; };

        std::list<size_t> getOutEdgesOfIdx(size_t vertex) const;
        std::vector<std::list<size_t> > getInEdgesOfVertices() const;
        std::vector<std::vector<size_t> > getAdjacencyMatrix() const;
        size_t getInDegreeIdx(size_t vertex) const;
        std::vector<size_t> getInDegrees() const;
        size_t getOutDegreeIdx(size_t vertex) const;
        std::vector<size_t> getOutDegrees() const;


        friend std::ostream& operator <<(std::ostream &stream, const DirectedGraph& graph) {
                for (size_t i=0; i<graph.size; ++i){
                    stream << "Vertex " << i << ": ";
                    for (auto& neighbour: graph.getOutEdgesOfIdx(i))
                        stream << neighbour << ", ";
                    stream << "\n";
                }
                return stream;
            };

        struct iterator {
            size_t position;
            iterator(size_t position) : position(position) {}
            bool operator!=(iterator rhs) {return position != rhs.position;}
            size_t& operator*() {return position;}
            iterator operator++() {++position; return *this;}
            iterator operator++(int) {iterator tmp=iterator(position); operator++(); return tmp;}
        };

        iterator begin() const {return iterator(0); }
        iterator end() const {return iterator(size);}

    protected:
        std::vector<std::list<size_t> > adjacencyList;
        size_t size;
        size_t edgeNumber;
};

} // namespace PGL

#endif
