#include "pgl/fileio.h"

#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>


using namespace std;


namespace PGL{

int addVertexToEdgelist(const string name_str, map<string, size_t>& Name2Num)
{
  int v;
  map< string, size_t >::iterator name_it = Name2Num.find(name_str);
  if(name_it == Name2Num.end()) {  // True if this is a new vertex.
    v = Name2Num.size();
    Name2Num[name_str] = v;
  }
  else
    v = name_it->second;
  return v;
}


void addEdgeToEdgelist(string name1_str, string name2_str, multiset< pair<int,int> >& edgelist, map<string, size_t>& Name2Num) {
  int v1 = addVertexToEdgelist(name1_str, Name2Num);
  int v2 = addVertexToEdgelist(name2_str, Name2Num);
  edgelist.insert(make_pair(v1, v2));
}


map<string, size_t> loadGraphFromEdgelist(const string edgelistFilename, DirectedGraph& graph, const bool allow_multiedges, const bool allow_selfloops) {
  map<string, size_t> Name2Num;
  multiset< pair<int, int> > edgelist;
  fstream edgelistFile;
  stringstream currentLine;
  string fullLine, name1_str, name2_str;

  edgelistFile.open(edgelistFilename.c_str(), fstream::in);
  if( !edgelistFile.is_open() )
    throw runtime_error("Could not open file.");
  else
    while( !edgelistFile.eof() ) {
      getline(edgelistFile, fullLine);  // Reads a line of the file.
      edgelistFile >> ws;
      currentLine.str(fullLine);
      currentLine >> ws;
      currentLine >> name1_str >> ws;
      if(name1_str == "#")  // Skips a line of comment.
      {
        currentLine.clear();
        continue;
      }
      currentLine >> name2_str >> ws;
      currentLine.clear();

      if(!allow_selfloops)
        if(name1_str == name2_str)
          continue;

      addEdgeToEdgelist(name1_str, name2_str, edgelist, Name2Num);
    }
  edgelistFile.close();

  graph.resize(Name2Num.size());

  if(allow_multiedges)
    for(auto el: edgelist)
      graph.addEdgeIdx(el.first, el.second, true);
  else {
    set< pair<int, int> > simpleEdgelist(edgelist.begin(), edgelist.end());
    for(auto el: simpleEdgelist)
      graph.addEdgeIdx(el.first, el.second, true);
  }

  return Name2Num;
}

// VertexLabeledDirectedGraph

VertexLabeledDirectedGraph<std::string> loadDirectedEdgeListFromTextFile(const std::string& fileName){
    VertexLabeledDirectedGraph<std::string> returnedGraph;

    std::ifstream fileStream(fileName.c_str());
    returnedGraph = loadDirectedEdgeListFromTextFile(fileStream);
    fileStream.close();

    return returnedGraph;
}

VertexLabeledDirectedGraph<std::string> loadDirectedEdgeListFromTextFile(std::ifstream& fileStream){
    VertexLabeledDirectedGraph<std::string> returnedGraph;

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


// VertexLabeledUndirectedGraph

VertexLabeledUndirectedGraph<std::string> loadUndirectedEdgeListFromTextFile(const std::string& fileName){
    VertexLabeledUndirectedGraph<std::string> returnedGraph;

    std::ifstream fileStream(fileName.c_str());
    returnedGraph = loadUndirectedEdgeListFromTextFile(fileStream);
    fileStream.close();

    return returnedGraph;
}

VertexLabeledUndirectedGraph<std::string> loadUndirectedEdgeListFromTextFile(std::ifstream& fileStream){
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

// DirectedGraph

void writeEdgeListIdxInTextFile(const DirectedGraph& graph, const string& fileName, size_t starting_id) {
    ofstream fileStream(fileName);
    writeEdgeListIdxInTextFile(graph, fileStream, starting_id);
    fileStream.close();
}

void writeEdgeListIdxInTextFile(const DirectedGraph& graph, ofstream& fileStream, size_t starting_id){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (size_t i=0; i<graph.getSize(); ++i)
        for (size_t& j: graph.getOutEdgesOfIdx(i))
            fileStream << i + starting_id << " " << j + starting_id << '\n';
}

void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName, ios::out|ios::binary);
    writeEdgeListIdxInBinaryFile(graph, fileStream);
    fileStream.close();
}

void writeEdgeListIdxInBinaryFile(const DirectedGraph& graph, ofstream& fileStream){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    size_t byteSize = sizeof(size_t);

    for (size_t& i: graph){
        for (size_t& j: graph.getOutEdgesOfIdx(i)) {
            fileStream.write((char*) &i, byteSize);
            fileStream.write((char*) &j, byteSize);
        }
    }
}

DirectedGraph loadDirectedEdgeListIdxFromTextFile(const string& fileName){
    DirectedGraph returnedGraph(0);
    ifstream fileStream(fileName);
    returnedGraph = loadDirectedEdgeListIdxFromTextFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

DirectedGraph loadDirectedEdgeListIdxFromTextFile(ifstream& fileStream){
    DirectedGraph returnedGraph;

    stringstream currentLine;
    string full_line, strVertexIdx, strVertex2Idx;
    size_t vertexIdx, vertex2Idx;

    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");
    else {
        while( getline(fileStream, full_line)){
            fileStream >> std::ws;
            currentLine.str(full_line);
            currentLine >> std::ws;
            currentLine >> strVertexIdx >> std::ws;

            // Skips a line of comment.
            if(strVertexIdx == "#") {
                currentLine.clear();
                continue;
            }

            currentLine >> strVertex2Idx >> std::ws;
            currentLine.clear();

            vertexIdx = stoi(strVertexIdx);
            vertex2Idx = stoi(strVertex2Idx);
            if (vertexIdx >= returnedGraph.getSize()) returnedGraph.resize(vertexIdx + 1);
            if (vertex2Idx >= returnedGraph.getSize()) returnedGraph.resize(vertex2Idx + 1);
            returnedGraph.addEdgeIdx(vertexIdx, vertex2Idx);
        }
    }
    return returnedGraph;
}

DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(const string& fileName){
    DirectedGraph returnedGraph(0);
    ifstream fileStream(fileName, ios::out|ios::binary);
    returnedGraph = loadDirectedEdgeListIdxFromBinaryFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

DirectedGraph loadDirectedEdgeListIdxFromBinaryFile(ifstream& fileStream){
    DirectedGraph returnedGraph;

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    size_t i = 0;
    size_t vertex1, vertex2;
    size_t byteSize = sizeof(size_t);
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 >= returnedGraph.getSize()) returnedGraph.resize(vertex2 + 1);
        if (i % 2 == 1)
            returnedGraph.addEdgeIdx(vertex1, vertex2);
        ++i;
        vertex1 = vertex2;
    }
    return returnedGraph;
}


// UndirectedGraph

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName);
    writeEdgeListIdxInTextFile(graph, fileStream);
    fileStream.close();
}

void writeEdgeListIdxInTextFile(const UndirectedGraph& graph, ofstream& fileStream){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    fileStream << "# VertexIdx1,  VertexIdx2\n";

    for (size_t i=0; i<graph.getSize(); ++i)
        for (size_t& j: graph.getNeighboursOfIdx(i))
            if (i<j) fileStream << i << "   " << j << '\n'; // i < *j to write edges only once
}

void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, const string& fileName){
    ofstream fileStream(fileName, ios::out|ios::binary);
    writeEdgeListIdxInBinaryFile(graph, fileStream);
    fileStream.close();
}

void writeEdgeListIdxInBinaryFile(const UndirectedGraph& graph, ofstream& fileStream){
    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");

    size_t byteSize = sizeof(size_t);

    for (size_t i=0; i<graph.getSize(); ++i){
        for (size_t& j: graph.getNeighboursOfIdx(i)) {
            if (i <= j) { // write edges once
                fileStream.write((char*) &i, byteSize);
                fileStream.write((char*) &j, byteSize);
            }
        }
    }
}

UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(const string& fileName){
    UndirectedGraph returnedGraph;
    ifstream fileStream(fileName);
    returnedGraph = loadUndirectedEdgeListIdxFromTextFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

UndirectedGraph loadUndirectedEdgeListIdxFromTextFile(ifstream& fileStream){
    UndirectedGraph returnedGraph;

    stringstream currentLine;
    string full_line, strVertexIdx, strVertex2Idx;
    size_t vertexIdx, vertex2Idx;

    if(!fileStream.is_open())
        throw runtime_error("Could not open file.");
    else {
        while( getline(fileStream, full_line)){
            fileStream >> std::ws;
            currentLine.str(full_line);
            currentLine >> std::ws;
            currentLine >> strVertexIdx >> std::ws;

            // Skips a line of comment.
            if(strVertexIdx == "#") {
                currentLine.clear();
                continue;
            }
            currentLine >> strVertex2Idx >> std::ws;
            currentLine.clear();

            vertexIdx = stoi(strVertexIdx);
            vertex2Idx = stoi(strVertex2Idx);
            if (vertexIdx > returnedGraph.getSize()) returnedGraph.resize(vertexIdx);
            if (vertex2Idx > returnedGraph.getSize()) returnedGraph.resize(vertex2Idx);
            returnedGraph.addEdgeIdx(vertexIdx, vertex2Idx);
        }
    }
    return returnedGraph;
}

UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(const string& fileName){
    UndirectedGraph returnedGraph;
    ifstream fileStream(fileName, ios::out|ios::binary);
    returnedGraph = loadUndirectedEdgeListIdxFromBinaryFile(fileStream);
    fileStream.close();
    return returnedGraph;
}

UndirectedGraph loadUndirectedEdgeListIdxFromBinaryFile(ifstream& fileStream){
    UndirectedGraph returnedGraph;

    if(!fileStream.is_open())
        throw std::runtime_error("Could not open file.");

    size_t i = 0;
    size_t vertex1, vertex2;
    size_t byteSize = sizeof(size_t);
    while (fileStream.read((char*) &vertex2, byteSize)){
        if (vertex2 > returnedGraph.getSize()) returnedGraph.resize(vertex2);
        if (i % 2 == 1){
            returnedGraph.addEdgeIdx(vertex1, vertex2);
            returnedGraph.addEdgeIdx(vertex2, vertex1);
        }
        vertex1 = vertex2;
        ++i;
    }
    return returnedGraph;
}

} // namespace PGL
