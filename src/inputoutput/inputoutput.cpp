#include "pgl/inputoutput/inputoutput.h"

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


} // namespace PGL
