#ifndef PGL_VERTEX_LABELED_UNDIRECTED_GRAPH_HPP
#define PGL_VERTEX_LABELED_UNDIRECTED_GRAPH_HPP

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "pgl/undirectedgraph.h"
#include "pgl/algorithms/graphpaths.h"


namespace PGL{

template<typename T>
class VertexLabeledUndirectedGraph: public UndirectedGraph{

    protected:
        std::vector<T> vertices;

    public:
        void debug() const;
        VertexLabeledUndirectedGraph(): UndirectedGraph(0) {};
        VertexLabeledUndirectedGraph(const std::list<std::pair<T, T>>& edgeList);
        VertexLabeledUndirectedGraph(const VertexLabeledUndirectedGraph<T>& source);
        VertexLabeledUndirectedGraph(const UndirectedGraph& source, const std::vector<T>& vertices);

        VertexLabeledUndirectedGraph<T> operator=(const VertexLabeledUndirectedGraph<T>& other);
        bool operator==(const VertexLabeledUndirectedGraph<T>& other) const;
        bool operator!=(const VertexLabeledUndirectedGraph<T>& other) const { return !(this->operator==(other)); };


        void addVertex(T vertex, bool force=false);
        bool isVertex(T vertex) const;
        void changeVertexObjectTo(T currentObject, T newObject);
        void removeVertexFromEdgeList(T vertex) { removeVertexFromEdgeListIdx(findVertexIndex(vertex)); };
        const T& getVertexFromIdx(size_t vertexIdx) const;
        size_t findVertexIndex(T vertex) const;

        const size_t getSize() const {return size;}
        const std::vector<T>& getVertices() const { return vertices; };
        const std::list<T> getNeighboursOf(T vertex) const;

        std::list<T> convertIndicesListToObjects(const std::list<size_t>& indicesList) const;
        std::vector<T> convertIndicesVectorToObjects(const std::vector<size_t>& indicesVector) const;

        void addEdge(T source, T destination, bool force=false) { addEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); }
        void removeEdge(T source, T destination) { removeEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };
        bool isEdge(T source, T destination) const { return isEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };

        size_t getDegree(T vertex) const { return getDegreeIdx(findVertexIndex(vertex)); };


        void writeEdgeListInTextFile(const std::string& fileName) const;
        void writeEdgeListInTextFile(std::ofstream& fileStream) const;
        void writeEdgeListInBinaryFile(const std::string& fileName, size_t byteSize=0) const;
        void writeEdgeListInBinaryFile(std::ofstream& fileStream, size_t byteSize=0) const;
        void writeVerticesInBinaryFile(const std::string& fileName, size_t byteSize=0) const;
        void writeVerticesInBinaryFile(std::ofstream& fileStream, size_t byteSize=0) const;

        static VertexLabeledUndirectedGraph<std::string> loadEdgeListFromTextFile(const std::string& fileName);
        static VertexLabeledUndirectedGraph<std::string> loadEdgeListFromTextFile(std::ifstream& fileStream);
        static VertexLabeledUndirectedGraph<T> loadEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize=0);
        static VertexLabeledUndirectedGraph<T> loadEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize=0);
        void addVerticesFromBinaryFile(const std::string& fileName, size_t byteSize=0);
        void addVerticesFromBinaryFile(std::ifstream& fileStream, size_t byteSize=0);

        friend std::ostream& operator <<(std::ostream &stream, const VertexLabeledUndirectedGraph<T>& graph) {
                for (size_t i=0; i<graph.size; ++i){
                    stream << "Vertex " << graph.vertices[i] << ": ";
                    for (size_t& neighbour: graph.getNeighboursOf(i))
                        stream << graph.vertices[neighbour] << ", ";
                    stream << "\n";
                }
                return stream;
            };
};

template<typename T>
VertexLabeledUndirectedGraph<T>::VertexLabeledUndirectedGraph(const VertexLabeledUndirectedGraph<T>& source){
    size = source.size;
    vertices = source.vertices;
    adjacencyList = source.adjacencyList;
    edgeNumber = source.edgeNumber;
}

template<typename T>
VertexLabeledUndirectedGraph<T>::VertexLabeledUndirectedGraph(const std::list<std::pair<T, T>>& edgeList):
        UndirectedGraph(0) {
    for (auto& edge: edgeList) {
        // By default addVertex does not add existing labels
        addVertex(edge.first);
        addVertex(edge.second);
        addEdge(edge.first, edge.second);
    }
}

template<typename T>
VertexLabeledUndirectedGraph<T>::VertexLabeledUndirectedGraph(const UndirectedGraph& source, const std::vector<T>& verticesNames):
        UndirectedGraph(source.getSize()) {
    if (source.getSize() != verticesNames.size())
        throw std::logic_error("The vertices vector must be the size of the graph");

    vertices = verticesNames;
    edgeNumber = 0;

    for (size_t& vertex: source)
        for (size_t& neighbour: source.getNeighboursOfIdx(vertex))
            addEdgeIdx(vertex, neighbour);
}


template<typename T>
VertexLabeledUndirectedGraph<T> VertexLabeledUndirectedGraph<T>::operator=(const VertexLabeledUndirectedGraph<T>& other){
    if (this != &other){
        size = other.size;
        vertices = other.vertices;
        adjacencyList = other.adjacencyList;
        edgeNumber = other.edgeNumber;
    }
    return *this;
}

template<typename T>
bool VertexLabeledUndirectedGraph<T>::operator==(const VertexLabeledUndirectedGraph<T>& other) const{
    bool sameObject = size == other.size;
    
    std::list<size_t>::const_iterator it;
    try {
    for (size_t i=0; i<size && sameObject; ++i){
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
    } catch (std::logic_error) {  // isEdge calling findVertexIndex threw "Vertex does not exist"
        sameObject = false;
    }

    return sameObject;
}

template <typename T>
void VertexLabeledUndirectedGraph<T>::addVertex(T vertex, bool force){
    if (force) {
        vertices.push_back(vertex);
        adjacencyList.push_back(std::list<size_t>());
        size++;
    }
    else if (!isVertex(vertex)) {
        vertices.push_back(vertex);
        adjacencyList.push_back(std::list<size_t>());
        size++;
    }
}

template <typename T>
bool VertexLabeledUndirectedGraph<T>::isVertex(T vertex) const{
    bool exists = false;
    for (size_t i=0; i<size && !exists; ++i)
        if (vertices[i] == vertex) exists = true;
    return exists;
}

template<typename T>
const std::list<T> VertexLabeledUndirectedGraph<T>::getNeighboursOf(T vertex) const{
    return convertIndicesListToObjects(getNeighboursOfIdx(findVertexIndex(vertex)));
}

template<typename T>
std::list<T> VertexLabeledUndirectedGraph<T>::convertIndicesListToObjects(const std::list<size_t>& indicesList) const{
    std::list<T> returnedList;

    for (auto& element: indicesList) {
        if (element >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedList.push_back(vertices[element]);
    }
    return returnedList;
}

template<typename T>
std::vector<T> VertexLabeledUndirectedGraph<T>::convertIndicesVectorToObjects(const std::vector<size_t>& indicesVector) const{
    std::vector<T> returnedVector(indicesVector.size());

    for (auto& element: indicesVector) {
        if (element >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedVector[element] = vertices[element];
    }
    return returnedVector;
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::changeVertexObjectTo(T currentObject, T newObject){
    if (isVertex(newObject)) throw std::logic_error("The object is already used as an attribute by another vertex.");
    vertices[findVertexIndex(currentObject)] = newObject;
}

template<typename T>
size_t VertexLabeledUndirectedGraph<T>::findVertexIndex(T vertex) const{
    for (size_t& i: *this)
        if (vertices[i] == vertex)
            return i;
    throw std::logic_error("Vertex does not exist");
}

template<typename T>
const T& VertexLabeledUndirectedGraph<T>::getVertexFromIdx(size_t vertexIdx) const {
    if (vertexIdx >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
    return vertices[vertexIdx];
}

// File i/o
template<typename T>
VertexLabeledUndirectedGraph<std::string> VertexLabeledUndirectedGraph<T>::loadEdgeListFromTextFile(const std::string& fileName){
    VertexLabeledUndirectedGraph<std::string> returnedGraph;

    std::ifstream fileStream(fileName.c_str());
    returnedGraph = loadEdgeListFromTextFile(fileStream);
    fileStream.close();

    return returnedGraph;
}

template<typename T>
VertexLabeledUndirectedGraph<std::string> VertexLabeledUndirectedGraph<T>::loadEdgeListFromTextFile(std::ifstream& fileStream){
    VertexLabeledUndirectedGraph<std::string> returnedGraph;

    std::stringstream currentLine;
    std::string full_line, name1_str, name2_str;

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");
    else {
        while( std::getline(fileStream, full_line)){
            fileStream >> std::ws;
            currentLine.str(full_line);
            currentLine >> std::ws;
            currentLine >> name1_str >> std::ws;

            // Skips a line of comment.
            if(name1_str == "#") {
                currentLine.clear();
                continue;
            }
            currentLine >> name2_str >> std::ws;
            currentLine.clear();

            returnedGraph.addVertex(name1_str);
            returnedGraph.addVertex(name2_str);
            returnedGraph.addEdge(name1_str, name2_str);
        }
    }
    return returnedGraph;
}

template<typename T>
VertexLabeledUndirectedGraph<T> VertexLabeledUndirectedGraph<T>::loadEdgeListFromBinaryFile(const std::string& fileName, size_t byteSize){
    VertexLabeledUndirectedGraph<T> returnedGraph;

    std::ifstream fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
    returnedGraph = loadEdgeListFromBinaryFile(fileStream);
    fileStream.close();

    return returnedGraph;
}

template<typename T>
VertexLabeledUndirectedGraph<T> VertexLabeledUndirectedGraph<T>::loadEdgeListFromBinaryFile(std::ifstream& fileStream, size_t byteSize){
    VertexLabeledUndirectedGraph<T> returnedGraph;
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    T vertex1, vertex2;
    size_t i = 0;
    while (fileStream.read((char*) &vertex2, byteSize)){
        returnedGraph.addVertex(vertex2);
        if (i % 2 == 1)
            returnedGraph.addEdge(vertex1, vertex2);
        vertex1 = vertex2;
        ++i;
    }
    return returnedGraph;
}

template<>
inline VertexLabeledUndirectedGraph<std::string> VertexLabeledUndirectedGraph<std::string>::loadEdgeListFromBinaryFile(std::ifstream& fileName, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::addVerticesFromBinaryFile(const std::string& fileName, size_t byteSize){
    std::ifstream fileStream(fileName.c_str(), std::ios::binary);
    addVerticesFromBinaryFile(fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::addVerticesFromBinaryFile(std::ifstream& fileStream, size_t byteSize){
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    T vertex;
    while (fileStream.read((char*) &vertex, byteSize)){
        addVertex(vertex);
    }
}

template<>
inline void VertexLabeledUndirectedGraph<std::string>::addVerticesFromBinaryFile(std::ifstream& fileStream, size_t byteSize){
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::writeEdgeListInTextFile(const std::string& fileName) const{
    std::ofstream fileStream(fileName.c_str());
    writeEdgeListInTextFile(fileStream);
    fileStream.close();
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::writeEdgeListInTextFile(std::ofstream& fileStream) const{
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    for (size_t& i: *this)
        for (size_t& j: getNeighboursOfIdx(i))
            if (i<j) fileStream << vertices[i] << "   " << vertices[j] << '\n';
}

template<>
inline void VertexLabeledUndirectedGraph<unsigned char>::writeEdgeListInTextFile(std::ofstream& fileStream) const{
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    for (size_t& i: *this)
        for (size_t& j: getNeighboursOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            if (i<j) fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<>
inline void VertexLabeledUndirectedGraph<signed char>::writeEdgeListInTextFile(std::ofstream& fileStream) const{
    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    fileStream << "# Vertex1,  Vertex2\n";

    for (size_t& i: *this)
        for (size_t& j: getNeighboursOfIdx(i))
            // Cast to int because operator << does not output properly otherwise
            if (i<j) fileStream << (unsigned long int) vertices[i] << "   " << (unsigned long int) vertices[j] << '\n';
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::writeEdgeListInBinaryFile(const std::string& fileName, size_t byteSize) const{
    std::ofstream fileStream(fileName.c_str(), std::ios::out | std::ios::binary);
    writeEdgeListInBinaryFile(fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::writeEdgeListInBinaryFile(std::ofstream& fileStream, size_t byteSize) const{
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    for (size_t& i: *this) {
        for (size_t& j: getNeighboursOfIdx(i)) {
            if (i <= j) { // write edges once
                fileStream.write((char*) &vertices[i], byteSize);
                fileStream.write((char*) &vertices[j], byteSize);
            }
        }
    }
}

template<>
inline void VertexLabeledUndirectedGraph<std::string>::writeEdgeListInBinaryFile(std::ofstream& fileName, size_t byteSize) const{
    throw std::logic_error("No implementation of string binary files.");
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::writeVerticesInBinaryFile(const std::string& fileName, size_t byteSize) const{
    std::ofstream fileStream(fileName, std::ios::binary);
    writeVerticesInBinaryFile(fileStream, byteSize);
    fileStream.close();
}

template<typename T>
void VertexLabeledUndirectedGraph<T>::writeVerticesInBinaryFile(std::ofstream& fileStream, size_t byteSize) const{
    if (byteSize == 0) byteSize = sizeof(T);

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    for (size_t i=0; i<size; ++i){
        fileStream.write((char*) &vertices[i], byteSize);
    }
}

template<>
inline void VertexLabeledUndirectedGraph<std::string>::writeVerticesInBinaryFile(std::ofstream& fileStream, size_t byteSize) const{
    throw std::logic_error("No implementation of string binary files.");
}

} // namespace PGL

#endif
